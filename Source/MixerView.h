/*
  ==============================================================================

    MixerView.h
    Created: 5 Feb 2025 5:14:06pm
    Author:  Jacques Thurling

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/*
*/
class MixerView  : public juce::Component, public juce::Slider::Listener
{
public:
    MixerView(DJAudioPlayer* _player1, DJAudioPlayer* _player2);
    ~MixerView() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(juce::Slider* slider) override;
private:
    std::vector<std::unique_ptr<juce::LookAndFeel>> lookAndFeels;
    
    DJAudioPlayer* djAudioPlayer1;
    DJAudioPlayer* djAudioPlayer2;
    
    juce::Slider mixerSlider;
    juce::Label mixerLabel;
    
    juce::Slider volumeSliderA;
    juce::Slider volumeSliderB;
    
    juce::Slider trackAHighPassSlider;
    juce::Slider trackAMidPassSlider;
    juce::Slider trackALowPassSlider;
    
    juce::Slider trackBHighPassSlider;
    juce::Slider trackBMidPassSlider;
    juce::Slider trackBLowPassSlider;
    
    juce::Image backgroundImage;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerView)
};
