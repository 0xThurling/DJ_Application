/*
 ==============================================================================
 
 WaveformDisplay.h
 Created: 30 Jan 2025 6:39:29pm
 Author:  Jacques Thurling
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 */
class WaveformDisplay  : public juce::Component, public juce::ChangeListener
{
    public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse, juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;
    
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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
