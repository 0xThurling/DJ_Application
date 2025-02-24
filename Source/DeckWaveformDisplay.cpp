/*
 ==============================================================================
 
 DeckWaveformDisplay.cpp
 Created: 22 Feb 2025 1:38:07pm
 Author:  Jacques Thurling
 
 ==============================================================================
 */

#include <JuceHeader.h>
#include "DeckWaveformDisplay.h"

//==============================================================================
DeckWaveformDisplay::DeckWaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                         juce::AudioThumbnailCache& cache) : audioThumbnail(1000, formatManagerToUse, cache), fileLoaded(false), position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumbnail.addChangeListener(this);
}

DeckWaveformDisplay::~DeckWaveformDisplay()
{
}

void DeckWaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
     draws some placeholder text to get you started.
     
     You should replace everything in this method with your own
     drawing code..
     */
    
    g.fillAll (juce::Colour {44, 44, 44});   // clear the background
    
    g.setColour (juce::Colours::grey);
    
    if(fileLoaded)
    {
        g.beginTransparencyLayer(0.5f);
        
        //        ColourGradient (Colour colour1, float x1, float y1,
        //                        Colour colour2, float x2, float y2,
        //                        bool isRadial);
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
                                   1.0f
                                   );
        g.endTransparencyLayer();
    }
    else
    {
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text
        
    }
}

void DeckWaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
}

void DeckWaveformDisplay::loadUrl(juce::URL url){
    audioThumbnail.clear();
    fileLoaded = audioThumbnail.setSource(new juce::URLInputSource(url));
}

void DeckWaveformDisplay::changeListenerCallback (juce::ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;
    repaint();
}

void DeckWaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
}
