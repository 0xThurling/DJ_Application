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
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cache) : audioThumbnail(1000, formatManagerToUse, cache), fileLoaded(false), position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumbnail.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
     draws some placeholder text to get you started.
     
     You should replace everything in this method with your own
     drawing code..
     */
    
    g.fillAll (juce::Colour {30, 30, 30});   // clear the background
    
    g.setColour (juce::Colours::grey);
    
    if(fileLoaded)
    {
        audioThumbnail.drawChannel(g,
                                   getLocalBounds(),
                                   0,
                                   audioThumbnail.getTotalLength(),
                                   0,
                                   1.0f
                                   );
        
        juce::Rectangle<int> customBounds (0, 0, position * getWidth(), getHeight());
        g.setColour (juce::Colour {0, 183, 235});
        audioThumbnail.drawChannel(g,
                                   customBounds,
                                   0,
                                   audioThumbnail.getTotalLength() * position,
                                   0,
                                   1.0f
                                   );
        g.setColour(juce::Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, 2, getHeight());
    }
    else
    {
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text
        
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
}

void WaveformDisplay::loadUrl(juce::URL url){
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(url));
}

void WaveformDisplay::changeListenerCallback (juce::ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }  
}
