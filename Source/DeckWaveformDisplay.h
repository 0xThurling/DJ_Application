/*
 ==============================================================================
 
 DeckWaveformDisplay.h
 Created: 22 Feb 2025 1:38:07pm
 Author:  Jacques Thurling
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 */
class DeckWaveformDisplay  : public juce::Component, public juce::ChangeListener
{
    public:
    DeckWaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                        juce::AudioThumbnailCache& cache);
    
    DeckWaveformDisplay();
    
    ~DeckWaveformDisplay() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void changeListenerCallback (juce::ChangeBroadcaster *source) override;
    
    void loadUrl(juce::URL url);
    
    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);
    private:
    juce::AudioThumbnail audioThumbnail;
    
    bool fileLoaded;
    double position;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckWaveformDisplay)
};
