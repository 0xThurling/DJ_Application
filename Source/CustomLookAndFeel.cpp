/*
 ==============================================================================
 
 CustomLookAndFeel.cpp
 Created: 17 Feb 2025 4:13:42pm
 Author:  Jacques Thurling
 
 ==============================================================================
 */

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

/**
 * @class CustomLookAndFeel
 * @brief A custom LookAndFeel class for JUCE components.
 */

//==============================================================================
/**
 * @brief Constructor for CustomLookAndFeel.
 * @param _scaleFactor Scaling factor for UI elements.
 */
CustomLookAndFeel::CustomLookAndFeel(float _scaleFactor) : scaleFactor(_scaleFactor)
{
    juce::File executableFile = juce::File::getSpecialLocation(juce::File::currentExecutableFile);
    juce::File projectDir = executableFile.getParentDirectory();
    juce::String correctPath;
    while (projectDir.getFileName() != "New_DJ" && projectDir.getParentDirectory() != projectDir) {
        projectDir = projectDir.getParentDirectory();
    }
    
    if (projectDir.getFileName() == "New_DJ") {
        correctPath = projectDir.getFullPathName();
    } else {
        // Handle the case where the New_DJ folder wasn't found
        DBG("New_DJ folder not found in the directory structure");
    }
    
    // Initialize image resources
    juce::File appDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory().getParentDirectory();
    
    std::string imagePath = correctPath.toStdString() + "/Assets/knob.png";
    juce::File imageFile(imagePath);
    knobImage = juce::ImageCache::getFromFile(imageFile);
    
    appDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory().getParentDirectory();
    imagePath = correctPath.toStdString() + "/Assets/slider_thumb.png";
    juce::File thumbImageFile(imagePath);
    thumbImage = juce::ImageCache::getFromFile(thumbImageFile);
}

/**
 * @brief Destructor for CustomLookAndFeel.
 */
CustomLookAndFeel::~CustomLookAndFeel()
{
}

/**
 * @brief Draws a rotary slider using a custom knob image.
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
void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional,
                                         float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) {
    const float rotation = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    const float centerX = x + width * 0.5f;
    const float centerY = y + height * 0.5f;
    const float radius = std::min(width, height) * 0.5f;
    
    if (knobImage.isValid()) {
        juce::AffineTransform transform = juce::AffineTransform::translation(-knobImage.getWidth() * 0.5f, -knobImage.getHeight() * 0.5f)
            .scaled(scaleFactor, scaleFactor)
            .rotated(rotation)
            .translated(centerX, centerY);
        
        g.drawImageTransformed(knobImage, transform);
    }
}

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
void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g,
                                         int x,
                                         int y,
                                         int width,
                                         int height,
                                         float sliderPos,
                                         float minSliderPos,
                                         float maxSliderPos,
                                         juce::Slider::SliderStyle style,
                                         juce::Slider& slider) {
    if (style == juce::Slider::LinearHorizontal || style == juce::Slider::LinearVertical)
    {
        if (sliderImage.isValid())
        {
            g.drawImageWithin(sliderImage, x, y, width, height,
                              juce::RectanglePlacement::fillDestination);
        }
        else
        {
            g.setColour(juce::Colours::darkgrey);
            const int trackThickness = 4;
            
            if (style == juce::Slider::LinearHorizontal)
            {
                int trackY = y + (height / 2) - (trackThickness / 2);
                g.fillRect(x, trackY, width, trackThickness);
            }
            else if (style == juce::Slider::LinearVertical)
            {
                int trackX = x + (width / 2) - (trackThickness / 2);
                g.fillRect(trackX, y, trackThickness, height);
            }
        }
        
        if (thumbImage.isValid())
        {
            int thumbWidth  = thumbImage.getWidth() / 2;
            int thumbHeight = thumbImage.getHeight() / 2;
            
            int thumbX = 0;
            int thumbY = 0;
            
            if (style == juce::Slider::LinearHorizontal)
            {
                thumbX = static_cast<int>(sliderPos - thumbWidth / 2);
                thumbY = y + (height / 2) - (thumbHeight / 2);
            }
            else
            {
                thumbX = x + (width / 2) - (thumbWidth / 2);
                thumbY = static_cast<int>(sliderPos - thumbHeight / 2);
            }
            
            g.drawImageWithin(thumbImage, thumbX, thumbY, thumbWidth, thumbHeight,
                              juce::RectanglePlacement::centred);
        }
    }
}
