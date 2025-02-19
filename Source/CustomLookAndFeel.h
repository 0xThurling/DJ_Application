/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 17 Feb 2025 4:13:42pm
    Author:  Jacques Thurling

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class CustomLookAndFeel  : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel(float scaleFactor);
    ~CustomLookAndFeel() override;

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    
    void drawLinearSlider(juce::Graphics& g,
                          int x,
                          int y,
                          int width,
                          int height,
                          float sliderPos,
                          float minSliderPos,
                          float maxSliderPos,
                          juce::Slider::SliderStyle style,
                          juce::Slider& slider) override;
private:
    juce::Image knobImage;
    juce::Image sliderImage;
    juce::Image thumbImage;
    
    float scaleFactor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeel)
};
