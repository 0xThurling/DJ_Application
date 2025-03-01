/*
 ============================================================================
 
 CustomLookAndFeel.h
 Created: 17 Feb 2025 4:13:42pm
 Author:  Jacques Thurling
 
 ============================================================================
 */

#pragma once

#include <JuceHeader.h>

/**
 * @class CustomLookAndFeel
 * @brief A custom LookAndFeel class for JUCE components.
 */
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    /**
     * @brief Constructor for CustomLookAndFeel.
     * @param scaleFactor Scaling factor for UI elements.
     */
    CustomLookAndFeel(float scaleFactor);
    
    /**
     * @brief Destructor for CustomLookAndFeel.
     */
    ~CustomLookAndFeel() override;
    
    /**
     * @brief Draws a rotary slider with a custom knob image.
     * @param g Graphics context to draw into.
     * @param x X position of the slider.
     * @param y Y position of the slider.
     * @param width Width of the slider.
     * @param height Height of the slider.
     * @param sliderPosProportional Normalized slider position (0.0 - 1.0).
     * @param rotaryStartAngle Starting angle of the rotary control.
     * @param rotaryEndAngle Ending angle of the rotary control.
     * @param slider Reference to the JUCE Slider component.
     */
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    
    /**
     * @brief Draws a linear slider with a custom thumb image.
     * @param g Graphics context to draw into.
     * @param x X position of the slider.
     * @param y Y position of the slider.
     * @param width Width of the slider.
     * @param height Height of the slider.
     * @param sliderPos Current position of the slider.
     * @param minSliderPos Minimum position of the slider.
     * @param maxSliderPos Maximum position of the slider.
     * @param style Slider style (horizontal or vertical).
     * @param slider Reference to the JUCE Slider component.
     */
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
    juce::Image knobImage;  ///< Image for the rotary slider knob.
    juce::Image sliderImage; ///< Image for the linear slider background.
    juce::Image thumbImage; ///< Image for the linear slider thumb.
    
    float scaleFactor; ///< Scaling factor for UI elements.
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeel)
};
