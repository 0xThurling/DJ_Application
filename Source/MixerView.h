/*
  ==============================================================================

    MixerView.h
    Created: 5 Feb 2025 5:14:06pm
    Author:  Jacques Thurling

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MixerView  : public juce::Component
{
public:
    MixerView();
    ~MixerView() override;

    void paint (juce::Graphics&) override;
    void resized() override;
private:
    juce::Slider mixerSlider;
    juce::Slider volumeSliderA;
    juce::Slider volumeSliderB;
    
    juce::Slider trackAHighPassSlider;
    juce::Slider trackAMidPassSlider;
    juce::Slider trackALowPassSlider;
    
    juce::Slider trackBHighPassSlider;
    juce::Slider trackBMidPassSlider;
    juce::Slider trackBLowPassSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerView)
};
