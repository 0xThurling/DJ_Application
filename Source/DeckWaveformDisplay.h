/**
 * @file DeckWaveformDisplay.h
 * @brief Declaration of the DeckWaveformDisplay class.
 *
 * This file declares the DeckWaveformDisplay class which is responsible for displaying
 * an audio waveform for decks and handling related user interface events.
 *
 * Created: 22 Feb 2025 1:38:07pm
 * Author: Jacques Thurling
 */

#pragma once

#include <JuceHeader.h>

/**
 * @class DeckWaveformDisplay
 * @brief A component that displays an audio waveform.
 *
 * The DeckWaveformDisplay class inherits from juce::Component for graphical interface
 * capabilities and from juce::ChangeListener to react to change events.
 */
class DeckWaveformDisplay  : public juce::Component, public juce::ChangeListener
{
public:
    /**
     * @brief Constructs a DeckWaveformDisplay using an audio format manager and a thumbnail cache.
     *
     * This constructor initializes the waveform display with the necessary audio processing
     * resources.
     *
     * @param formatManagerToUse Reference to a juce::AudioFormatManager for audio file handling.
     * @param cache Reference to a juce::AudioThumbnailCache for caching audio thumbnails.
     */
    DeckWaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                        juce::AudioThumbnailCache& cache);
    
    /**
     * @brief Default constructor for DeckWaveformDisplay.
     *
     * Constructs a DeckWaveformDisplay with default settings.
     */
    DeckWaveformDisplay();
    
    /**
     * @brief Destructor for DeckWaveformDisplay.
     */
    ~DeckWaveformDisplay() override;
    
    /**
     * @brief Paints the waveform display.
     *
     * This method is called whenever the component needs to be redrawn.
     *
     * @param g The juce::Graphics context used for drawing.
     */
    void paint (juce::Graphics& g) override;
    
    /**
     * @brief Handles component resizing.
     *
     * This method is invoked when the component's dimensions change, allowing for
     * necessary adjustments to its layout or content.
     */
    void resized() override;
    
    /**
     * @brief Callback for change events.
     *
     * Invoked when a change event is broadcasted by a juce::ChangeBroadcaster.
     *
     * @param source Pointer to the ChangeBroadcaster that triggered the event.
     */
    void changeListenerCallback (juce::ChangeBroadcaster *source) override;
    
    /**
     * @brief Loads an audio file from a given URL.
     *
     * This method loads the audio file specified by the URL and updates the waveform display accordingly.
     *
     * @param url The URL from which to load the audio file.
     */
    void loadUrl(juce::URL url);
    
    /**
     * @brief Sets the relative position of the playhead.
     *
     * Adjusts the display to indicate the current play position as a fraction of the total duration.
     *
     * @param pos A double value between 0.0 and 1.0 representing the relative playhead position.
     */
    void setPositionRelative(double pos);
    
private:
    juce::AudioThumbnail audioThumbnail;  ///< Audio thumbnail for rendering the waveform.
    
    bool fileLoaded;   ///< Flag indicating whether an audio file has been successfully loaded.
    double position;   ///< The current relative position of the playhead.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckWaveformDisplay) ///< Prevents copying and enables leak detection.
};
