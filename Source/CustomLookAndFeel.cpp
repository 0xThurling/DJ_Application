/*
 ==============================================================================
 
 CustomLookAndFeel.cpp
 Created: 17 Feb 2025 4:13:42pm
 Author:  Jacques Thurling
 
 ==============================================================================
 */

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

//==============================================================================
CustomLookAndFeel::CustomLookAndFeel(float _scaleFactor) : scaleFactor(_scaleFactor)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    juce::File appDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory().getParentDirectory();
    
    juce::File imageFile = appDir.getChildFile("Resources/knob.png");
    knobImage = juce::ImageCache::getFromFile(imageFile);
    
    appDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory().getParentDirectory();
    imageFile = appDir.getChildFile("Resources/slider_thumb.png");
    
    thumbImage = juce::ImageCache::getFromFile(imageFile);
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional,
                                         float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) {
    // Calculate the rotation angle based on the slider position
    const float rotation = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    
    // Define the center and radius of the knob
    const float centerX = x + width * 0.5f;
    const float centerY = y + height * 0.5f;
    const float radius = std::min(width, height) * 0.5f;
    
    // Draw the knob image rotated around its center
    if (knobImage.isValid()) {
        juce::AffineTransform transform = juce::AffineTransform::translation(-knobImage.getWidth() * 0.5f, -knobImage.getHeight() * 0.5f)
            .scaled(scaleFactor, scaleFactor)
            .rotated(rotation)
            .translated(centerX, centerY);
        
        g.drawImageTransformed(knobImage, transform);
    }
}

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
            // Use the entire available area as the background.
            g.drawImageWithin(sliderImage, x, y, width, height,
                              juce::RectanglePlacement::fillDestination);
        }
        else // Fallback: draw a simple rectangular track if the image isn't loaded.
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
        
        // Ensure thumbImage is valid (loaded in the LookAndFeel's constructor or init)
        if (thumbImage.isValid())
        {
            // Define the desired thumb dimensions (or use thumbImage.getWidth() / getHeight())
            int thumbWidth  = thumbImage.getWidth()/2;
            int thumbHeight = thumbImage.getHeight()/2;
            
            int thumbX = 0;
            int thumbY = 0;
            
            // Calculate thumb position based on slider style
            if (style == juce::Slider::LinearHorizontal)
            {
                thumbX = static_cast<int>(sliderPos - thumbWidth / 2);
                thumbY = y + (height / 2) - (thumbHeight / 2);
            }
            else // LinearVertical
            {
                thumbX = x + (width / 2) - (thumbWidth / 2);
                thumbY = static_cast<int>(sliderPos - thumbHeight / 2);
            }
            
            // Draw the PNG thumb image within the calculated bounds
            g.drawImageWithin(thumbImage, thumbX, thumbY, thumbWidth, thumbHeight,
                              juce::RectanglePlacement::centred);
        }
    }
}
