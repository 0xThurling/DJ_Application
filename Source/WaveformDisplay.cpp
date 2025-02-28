/*
 ==============================================================================
 
 WaveformDisplay.cpp
 Created: 30 Jan 2025 6:39:29pm
 Author:  Jacques Thurling
 
 ==============================================================================
 */

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
/**
 * @class WaveformDisplay
 * @brief A component that displays an audio waveform using JUCE's AudioThumbnail.
 */
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cache)
    : audioThumbnail(1000, formatManagerToUse, cache), fileLoaded(false), position(0)
{
    /**
     * @brief Constructor for WaveformDisplay.
     * @param formatManagerToUse Reference to an AudioFormatManager for audio format handling.
     * @param cache Reference to an AudioThumbnailCache for caching audio thumbnails.
     */
    audioThumbnail.addChangeListener(this);
}

/**
 * @brief Destructor for WaveformDisplay.
 */
WaveformDisplay::~WaveformDisplay()
{
}

/**
 * @brief Renders the waveform display.
 * @param g JUCE Graphics context.
 */
void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour {30, 30, 30}); // Clear the background
    g.setColour (juce::Colours::grey);
    
    if(fileLoaded)
    {
        audioThumbnail.drawChannel(g,
                                   getLocalBounds(),
                                   0,
                                   audioThumbnail.getTotalLength(),
                                   0,
                                   1.0f);
        
        juce::Rectangle<int> customBounds (0, 0, position * getWidth(), getHeight());
        g.setColour (juce::Colour {0, 183, 235});
        audioThumbnail.drawChannel(g,
                                   customBounds,
                                   0,
                                   audioThumbnail.getTotalLength() * position,
                                   0,
                                   1.0f);
        g.setColour(juce::Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, 2, getHeight());
    }
    else
    {
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(),
                    juce::Justification::centred, true); // Draw placeholder text
    }
}

/**
 * @brief Called when the component is resized.
 */
void WaveformDisplay::resized()
{
    // Set the bounds of any child components if needed.
}

/**
 * @brief Loads an audio file from a given URL.
 * @param url The URL of the audio file.
 */
void WaveformDisplay::loadUrl(juce::URL url)
{
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(url));
}

/**
 * @brief Callback for change notifications from the AudioThumbnail.
 * @param source The ChangeBroadcaster that triggered the change.
 */
void WaveformDisplay::changeListenerCallback (juce::ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;
    repaint();
}

/**
 * @brief Sets the playback position relative to the waveform length.
 * @param pos The new position as a normalized value between 0 and 1.
 */
void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}
