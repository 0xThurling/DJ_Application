/*
 ==============================================================================
 
 WaveformDisplay.h
 Created: 30 Jan 2025 6:39:29pm
 Author:  Jacques Thurling
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>

/**
 * @class WaveformDisplay
 * @brief A component that displays an audio waveform and manages playback visualization.
 */
class WaveformDisplay : public juce::Component, public juce::ChangeListener
{
public:
    /**
     * @brief Constructor for WaveformDisplay.
     * @param formatManagerToUse Reference to the audio format manager.
     * @param cacheToUse Reference to the audio thumbnail cache.
     */
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse);
    
    /**
     * @brief Destructor for WaveformDisplay.
     */
    ~WaveformDisplay() override;
    
    /**
     * @brief Paints the waveform display.
     * @param g Graphics context.
     */
    void paint(juce::Graphics&) override;
    
    /**
     * @brief Handles component resizing.
     */
    void resized() override;
    
    /**
     * @brief Callback function triggered when the audio thumbnail changes.
     * @param source Pointer to the ChangeBroadcaster.
     */
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;
    
    /**
     * @brief Loads an audio file from a URL and generates its waveform.
     * @param url URL of the audio file.
     */
    void loadUrl(juce::URL url);
    
    /**
     * @brief Sets the relative position of the playhead within the waveform.
     * @param pos New playback position (0.0 - 1.0).
     */
    void setPositionRelative(double pos);
    
private:
    juce::AudioThumbnail audioThumbnail; ///< Handles waveform visualization.
    bool fileLoaded; ///< Indicates whether an audio file is loaded.
    double position; ///< Stores the current playback position.
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
