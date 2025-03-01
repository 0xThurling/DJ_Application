/**
 * =================================================================
 * @file DeckWaveformDisplay.cpp
 * @brief Implementation of the DeckWaveformDisplay component.
 *
 * This file implements the waveform display for a deck, handling painting,
 * audio file loading, and change notifications.
 *
 * Created: 22 Feb 2025 1:38:07pm
 * Author: Jacques Thurling
 */

#include <JuceHeader.h>
#include "DeckWaveformDisplay.h"

//==============================================================================
/**
 * @brief Constructs a new DeckWaveformDisplay object.
 *
 * This constructor initializes the audio thumbnail with a resolution of 1000 samples,
 * registers a change listener, and sets up the initial state.
 *
 * @param formatManagerToUse Reference to the AudioFormatManager used to manage audio formats.
 * @param cache Reference to the AudioThumbnailCache used for caching thumbnail data.
 */
DeckWaveformDisplay::DeckWaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                         juce::AudioThumbnailCache& cache)
    : audioThumbnail(1000, formatManagerToUse, cache),
      fileLoaded(false),
      position(0)
{
    audioThumbnail.addChangeListener(this);
}

/**
 * @brief Destructor for DeckWaveformDisplay.
 */
DeckWaveformDisplay::~DeckWaveformDisplay()
{
}

/**
 * @brief Paints the component.
 *
 * This function clears the background and either draws the waveform
 * or displays placeholder text if no file has been loaded. When a file is loaded,
 * it draws a partially transparent gradient over the waveform.
 *
 * @param g The graphics context used for drawing.
 */
void DeckWaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour {44, 44, 44});   // clear the background
    g.setColour (juce::Colours::grey);

    if (fileLoaded)
    {
        g.beginTransparencyLayer(0.5f);

        // Define a gradient for the waveform display
        juce::ColourGradient gradient {
            juce::Colour {0, 183, 235}.withAlpha(0.0f), getWidth() / 1.0f, getHeight() / 1.0f,
            juce::Colour {0, 183, 235}.withAlpha(1.0f), 0.0f, 0.0f,
            true
        };

        g.setGradientFill(gradient);
        g.setColour (juce::Colour {0, 183, 235});
        audioThumbnail.drawChannel(g,
                                   getLocalBounds(),
                                   audioThumbnail.getTotalLength() * position,
                                   audioThumbnail.getTotalLength() * position + .5,
                                   0,
                                   1.0f);
        g.endTransparencyLayer();
    }
    else
    {
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(),
                    juce::Justification::centred, true);
    }
}

/**
 * @brief Handles component resizing.
 *
 * This method is called when the component is resized. It should be used
 * to adjust the bounds of any child components.
 */
void DeckWaveformDisplay::resized()
{
    // Adjust the bounds of child components here, if any.
}

/**
 * @brief Loads an audio file from the specified URL.
 *
 * This function clears the current audio thumbnail and attempts to load a new
 * audio source from the provided URL.
 *
 * @param url The URL from which to load the audio file.
 */
void DeckWaveformDisplay::loadUrl(juce::URL url)
{
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(url));
}

/**
 * @brief Callback for change events.
 *
 * This function is called when a change event is broadcasted by the audio thumbnail.
 * It triggers a repaint of the component.
 *
 * @param source Pointer to the ChangeBroadcaster that triggered the event.
 */
void DeckWaveformDisplay::changeListenerCallback (juce::ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;
    repaint();
}

/**
 * @brief Sets the playback position relative to the audio length.
 *
 * This function updates the internal position (if it has changed) and repaints
 * the component to reflect the new position.
 *
 * @param pos The new relative position (expected range is 0.0 to 1.0).
 */
void DeckWaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}
