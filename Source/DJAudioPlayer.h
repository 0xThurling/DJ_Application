/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 28 Jan 2025 7:31:34pm
    Author:  Jacques Thurling

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource {
    private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
    
    double hpCutoff = 100.0f;
    double hpQualityFactor = 0.7071f;
    juce::dsp::IIR::Filter<float> highpassFilter;
    
    double lpCutoff = 20000.0f;
    double lpQualityFactor = 0.7071f;
    juce::dsp::IIR::Filter<float> lowpassFilter;
    
    double midCutoff = 500.0f;
    double midQualityFactor = 0.7071f;
    double midBandPassMix = 0.0;
    juce::dsp::IIR::Filter<float> midBandPassFilter;
    
    double djSampleRate;
    public:
    DJAudioPlayer();
    ~DJAudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    void setHighPassFilterAmount(double amount);
    void setLowPassFilterAmount(double amount);
    void setMidBandPassFilterAmount(double amount);

    void start();
    void stop();
    
    /** get the relative position of the playhead */
    double getPositionRelative();
};
