/*
==============================================================================

DJAudioPlayer.cpp
Created: 13 Mar 2020 4:22:22pm
Author:  matthew

==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer()
{
    reverbParams.roomSize = 0.9f;
    reverbParams.damping = 0.5f;
    reverbParams.wetLevel = reverbWetDryMix;
    reverbParams.dryLevel = 1.0f - reverbWetDryMix;
    reverbParams.width = 5.0f;
    reverbParams.freezeMode = 0.0f;
    
    reverb.setParameters(reverbParams);
    
    // Flanger
    flanger.setCentreDelay(0.001f);
    flanger.setRate(0.1f);
    flanger.setDepth(1.0f);
    flanger.setFeedback(0.7f);
    flanger.setMix(flangerWetDryMix);
}
DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    formatManager.registerBasicFormats();
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    // HighPassfilter setup
    highpassFilter.reset();
    auto hpCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, hpCutoff, hpQualityFactor);
    *highpassFilter.coefficients = *hpCoeffs;
    
    // Lowpass filter setup
    lowpassFilter.reset();
    auto lpCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, lpCutoff, lpQualityFactor);
    *lowpassFilter.coefficients = *lpCoeffs;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = 1;
    
    midBandPassFilter.reset();
    midBandPassFilter.prepare(spec);
    auto midCoeffs = juce::dsp::IIR::Coefficients<float>::makeBandPass(sampleRate, midCutoff, midQualityFactor);
    *midBandPassFilter.coefficients = *midCoeffs;
    
    // Setup reverb processing
    juce::dsp::ProcessSpec reverbSpec;
    reverbSpec.sampleRate = sampleRate;
    reverbSpec.maximumBlockSize = samplesPerBlockExpected;
    reverbSpec.numChannels = 1;
    reverb.prepare(reverbSpec);
    
    // Flanger spec
    juce::dsp::ProcessSpec flangerSpec;
    flangerSpec.sampleRate = sampleRate;
    flangerSpec.maximumBlockSize = samplesPerBlockExpected;
    flangerSpec.numChannels = 1;
    flanger.prepare(flangerSpec);
    
    // Store sample rate for later processing needed
    djSampleRate = sampleRate;
}

void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // First get the next Audio Block to process
    resampleSource.getNextAudioBlock(bufferToFill);
    
    juce::AudioBuffer<float> dryBuffer;
    dryBuffer.makeCopyOf(*bufferToFill.buffer);
    
    // Wrap the buffer in a dsp::AudioBlock to use the DSP module
    auto dryBlock = juce::dsp::AudioBlock<float>(dryBuffer);
    // Create a processing context that tells the filter to process in-place
    juce::dsp::ProcessContextReplacing<float> dryContext(dryBlock);
    
    // ================ HIGHPASS ===================
    highpassFilter.process(dryContext);
    // =============================================
    
    // ================ LOWPASS ====================
    lowpassFilter.process(dryContext);
    // =============================================
    
    // ================ BANDPASS ===================
    // Wrap the buffer in a dsp::AudioBlock to use the DSP module
    auto wetBlock = juce::dsp::AudioBlock<float>(*bufferToFill.buffer).getSubBlock(bufferToFill.startSample, bufferToFill.numSamples);
    // Create a processing context that tells the filter to process in-place
    juce::dsp::ProcessContextReplacing<float> wetContext(wetBlock);
    midBandPassFilter.process(wetContext);
    // =============================================
    
    // Blend the dry (original) and wet (filtered) signals based on bandpassMix:
    // A bandpassMix of 0.0 means fully dry, 1.0 means fully wet.
    const int numChannels = bufferToFill.buffer->getNumChannels();
    const int numSamples  = bufferToFill.buffer->getNumSamples();
    
    for (int channel = 0; channel < numChannels; ++channel)
    {
        const float* dry  = dryBuffer.getReadPointer(channel);
        float* wet = bufferToFill.buffer->getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample)
        {
            wet[sample] = dry[sample] * (1.0f - (float)midBandPassMix)
                            + wet[sample] * (float)midBandPassMix;
        }
    }
    
    // ================ REVERB =====================
    // Process the buffer through the reverb
    juce::dsp::AudioBlock<float> reverbBlock(*bufferToFill.buffer);
    juce::dsp::ProcessContextReplacing<float> reverbContext(reverbBlock);
    reverb.process(reverbContext);
    // =============================================
    
    // ================ FILTER =====================
    auto flangerBlock = juce::dsp::AudioBlock<float>(*bufferToFill.buffer).getSubBlock(bufferToFill.startSample, bufferToFill.numSamples);
    juce::dsp::ProcessContextReplacing<float> flangerContext(flangerBlock);
    flanger.process(flangerContext);
    // =============================================
    
    // ================ TREMOLO =====================
    const int samples = bufferToFill.numSamples;
    const int channels = bufferToFill.buffer->getNumChannels();
    
    for (int sample = 0; sample < numSamples; ++sample) {
        float lfoValue = (std::sin(volumeLFOPhase) + 1.0f) * 0.5f;
        
        float currentGain = (1.0f - volumeLFOdepth) + (volumeLFOdepth * lfoValue);
        
        for (int channel = 0; channel < channels; ++channel) {
            float* channelData = bufferToFill.buffer->getWritePointer(channel);
            channelData[sample] *= currentGain;
        }
        
        volumeLFOPhase += juce::MathConstants<float>::twoPi * volumeLFOrate / djSampleRate;
        
        if (volumeLFOPhase >= juce::MathConstants<float>::twoPi) {
            volumeLFOPhase -= juce::MathConstants<float>::twoPi;
        }
    }
    // =============================================
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource (new juce::AudioFormatReaderSource (reader,
true));
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset (newSource.release());
    }
}
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
   
}
void DJAudioPlayer::setSpeed(double ratio)
{
  if (ratio < 0 || ratio > 100.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
     if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}


void DJAudioPlayer::start()
{
    transportSource.start();
}

void DJAudioPlayer::stop()
{
  transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

void DJAudioPlayer::setHighPassFilterAmount(double amount) {
    hpCutoff = 2000 * amount;
    
    if (djSampleRate > 0) {
        auto coeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass(djSampleRate, hpCutoff, hpQualityFactor);
        *highpassFilter.coefficients = *coeffs;
    }
    
    std::cout << hpCutoff << " " << amount << std::endl;
}

void DJAudioPlayer::setLowPassFilterAmount(double amount) {
    lpCutoff = 20000.0f * amount;
    
    if (djSampleRate > 0) {
        auto lpCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass(djSampleRate, lpCutoff, lpQualityFactor);
        *lowpassFilter.coefficients = *lpCoeffs;
    }
    
    std::cout << lpCutoff << " " << amount << " " << djSampleRate * 0.5 << std::endl;
}

void DJAudioPlayer::setMidBandPassFilterAmount(double amount) {
    midBandPassMix = amount;
}

void DJAudioPlayer::setReverbAmount(double amount) {
    reverbParams.wetLevel = amount;
    reverbParams.dryLevel = 1.0f - amount;
    reverb.setParameters(reverbParams);
}

void DJAudioPlayer::setFlangerAmount(double amount) {
    flanger.setMix(amount);
}

void DJAudioPlayer::setTremelo(double amount) {
    volumeLFOdepth = amount;
}
