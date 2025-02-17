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
