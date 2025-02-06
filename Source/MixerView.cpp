/*
  ==============================================================================

    MixerView.cpp
    Created: 5 Feb 2025 5:14:06pm
    Author:  Jacques Thurling

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MixerView.h"

//==============================================================================
MixerView::MixerView()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    volumeSliderA.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    volumeSliderB.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    
    trackAHighPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    trackAMidPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    trackALowPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    
    trackBHighPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    trackBMidPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    trackBLowPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    
    mixerSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    volumeSliderA.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    volumeSliderB.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    
    trackAHighPassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    trackAMidPassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    trackALowPassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    
    trackBHighPassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    trackBMidPassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    trackBLowPassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    
    mixerSlider.setRange(0, 1);
    volumeSliderA.setRange(0, 1);
    volumeSliderB.setRange(0, 1);
    
    addAndMakeVisible(mixerSlider);
    addAndMakeVisible(volumeSliderA);
    addAndMakeVisible(volumeSliderB);
    
    addAndMakeVisible(trackAHighPassSlider);
    addAndMakeVisible(trackAMidPassSlider);
    addAndMakeVisible(trackALowPassSlider);
    
    addAndMakeVisible(trackBHighPassSlider);
    addAndMakeVisible(trackBMidPassSlider);
    addAndMakeVisible(trackBLowPassSlider);
    
    mixerSlider.setValue(0.5f);
    volumeSliderA.setValue(0.5f);
    volumeSliderB.setValue(0.5f);
}

MixerView::~MixerView()
{
}

void MixerView::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void MixerView::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    float rowH = getHeight()/8;
    float width = getWidth()/4;
    
    mixerSlider.setBounds(width, rowH * 7, width * 2, rowH);
    
    volumeSliderA.setBounds(0, rowH * 1, getWidth()/4, rowH * 5);
    volumeSliderB.setBounds((getWidth()/4) * 3, rowH * 1, getWidth()/4, rowH * 5);
    
    trackAHighPassSlider.setBounds((getWidth()/4) * 1, rowH * 1, (getWidth()/4), rowH);
    trackAMidPassSlider.setBounds((getWidth()/4) * 1, rowH * 2, (getWidth()/4), rowH);
    trackALowPassSlider.setBounds((getWidth()/4) * 1, rowH * 3, (getWidth()/4), rowH);
    
    trackBHighPassSlider.setBounds((getWidth()/4) * 2, rowH * 1, (getWidth()/4), rowH);
    trackBMidPassSlider.setBounds((getWidth()/4) * 2, rowH * 2, (getWidth()/4), rowH);
    trackBLowPassSlider.setBounds((getWidth()/4) * 2, rowH * 3, (getWidth()/4), rowH);
}
