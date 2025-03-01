/*
 ==============================================================================
 
 DJAudioPlayer.h
 Created: 28 Jan 2025 7:31:34pm
 Author:  Jacques Thurling
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>

/**
 * @class DJAudioPlayer
 * @brief A class for handling audio playback with various effects and filters.
 *
 * DJAudioPlayer provides functionalities to play, stop, and manipulate audio
 * including filtering, reverb, flanger, and tremolo effects.
 */
class DJAudioPlayer : public juce::AudioSource {
    private:
    juce::AudioFormatManager formatManager; ///< Manages available audio formats.
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource; ///< Pointer to the audio source.
    
    double hpCutoff = 100.0f; ///< High-pass filter cutoff frequency.
    double hpQualityFactor = 0.7071f; ///< High-pass filter quality factor.
    juce::dsp::IIR::Filter<float> highpassFilter; ///< High-pass filter object.
    
    double lpCutoff = 20000.0f; ///< Low-pass filter cutoff frequency.
    double lpQualityFactor = 0.7071f; ///< Low-pass filter quality factor.
    juce::dsp::IIR::Filter<float> lowpassFilter; ///< Low-pass filter object.
    
    double midCutoff = 500.0f; ///< Mid-band pass filter cutoff frequency.
    double midQualityFactor = 0.7071f; ///< Mid-band pass filter quality factor.
    double midBandPassMix = 0.0; ///< Mix amount for mid-band pass filter.
    juce::dsp::IIR::Filter<float> midBandPassFilter; ///< Mid-band pass filter object.
    
    float volumeLFOPhase = 0.0f; ///< Phase of volume LFO.
    float volumeLFOrate = 10.0f; ///< Rate of volume LFO.
    float volumeLFOdepth = 0.0f; ///< Depth of volume LFO.
    
    double djSampleRate; ///< Sample rate for processing.
    
    juce::dsp::Reverb reverb; ///< Reverb effect processor.
    juce::dsp::Reverb::Parameters reverbParams; ///< Parameters for reverb effect.
    
    double reverbWetDryMix = 0.0f; ///< Reverb wet/dry mix amount.
    
    juce::dsp::Chorus<float> flanger; ///< Flanger effect processor.
    
    double flangerWetDryMix = 0.0f; ///< Flanger wet/dry mix amount.
    
    public:
    /**
     * @brief Constructor for DJAudioPlayer.
     */
    DJAudioPlayer();
    
    /**
     * @brief Destructor for DJAudioPlayer.
     */
    ~DJAudioPlayer();
    
    juce::AudioTransportSource transportSource; ///< Handles audio transport functions.
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2}; ///< Resampler for handling pitch changes.
    
    /**
     * @brief Prepares the player to play audio.
     * @param samplesPerBlockExpected Expected number of samples per block.
     * @param sampleRate The sample rate of the audio stream.
     */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    
    /**
     * @brief Retrieves the next block of audio data.
     * @param bufferToFill The buffer to be filled with audio data.
     */
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    
    /**
     * @brief Releases audio resources when playback stops.
     */
    void releaseResources() override;
    
    /**
     * @brief Loads an audio file from a URL.
     * @param audioURL The URL of the audio file.
     */
    void loadURL(juce::URL audioURL);
    
    /**
     * @brief Sets the gain (volume) of the audio.
     * @param gain The gain value (0.0 - 1.0).
     */
    void setGain(double gain);
    
    /**
     * @brief Sets the playback speed.
     * @param ratio The speed ratio (1.0 is normal speed).
     */
    void setSpeed(double ratio);
    
    /**
     * @brief Sets the playback position in seconds.
     * @param posInSecs The position in seconds.
     */
    void setPosition(double posInSecs);
    
    /**
     * @brief Sets the relative playback position.
     * @param pos The relative position (0.0 - 1.0).
     */
    void setPositionRelative(double pos);
    
    /**
     * @brief Sets the high-pass filter cutoff frequency.
     * @param amount The cutoff frequency value.
     */
    void setHighPassFilterAmount(double amount);
    
    /**
     * @brief Sets the low-pass filter cutoff frequency.
     * @param amount The cutoff frequency value.
     */
    void setLowPassFilterAmount(double amount);
    
    /**
     * @brief Sets the mid-band pass filter parameters.
     * @param amount The filter amount.
     */
    void setMidBandPassFilterAmount(double amount);
    
    /**
     * @brief Sets the reverb effect amount.
     * @param amount The reverb mix amount.
     */
    void setReverbAmount(double amount);
    
    /**
     * @brief Sets the flanger effect amount.
     * @param amount The flanger mix amount.
     */
    void setFlangerAmount(double amount);
    
    /**
     * @brief Sets the tremolo effect amount.
     * @param amount The tremolo mix amount.
     */
    void setTremelo(double amount);
    
    /**
     * @brief Starts audio playback.
     */
    void start();
    
    /**
     * @brief Stops audio playback.
     */
    void stop();
    
    /**
     * @brief Gets the relative position of the playhead.
     * @return The relative position (0.0 - 1.0).
     */
    double getPositionRelative();
};
