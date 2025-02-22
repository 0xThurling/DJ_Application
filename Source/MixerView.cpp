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
MixerView::MixerView(DJAudioPlayer* _player1, DJAudioPlayer* _player2) : djAudioPlayer1(_player1), djAudioPlayer2(_player2)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    auto customLookAndFeel = std::make_unique<CustomLookAndFeel>(0.6f);
    
    volumeSliderA.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    volumeSliderB.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    
    volumeSliderA.setLookAndFeel(customLookAndFeel.get());
    volumeSliderB.setLookAndFeel(customLookAndFeel.get());
    mixerSlider.setLookAndFeel(customLookAndFeel.get());
    
    trackAHighPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    trackAMidPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    trackALowPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    
    trackBHighPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    trackBMidPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    trackBLowPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    
    trackAHighPassSlider.setLookAndFeel(customLookAndFeel.get());
    trackAMidPassSlider.setLookAndFeel(customLookAndFeel.get());
    trackALowPassSlider.setLookAndFeel(customLookAndFeel.get());
    
    trackBHighPassSlider.setLookAndFeel(customLookAndFeel.get());
    trackBMidPassSlider.setLookAndFeel(customLookAndFeel.get());
    trackBLowPassSlider.setLookAndFeel(customLookAndFeel.get());
    
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
    
    trackAHighPassSlider.setRange(0.1, 1);
    trackAMidPassSlider.setRange(0, 1);
    trackALowPassSlider.setRange(0.1, 0.99);
    
    trackBHighPassSlider.setRange(0.1, 1);
    trackBMidPassSlider.setRange(0, 1);
    trackBLowPassSlider.setRange(0.1, 0.99);
    
    mixerLabel.setText("Cross-Fade", juce::dontSendNotification);
    mixerLabel.setFont(juce::Font("Helvetica", 16.0f, juce::Font::plain));
    mixerLabel.attachToComponent(&mixerSlider, false);
    mixerLabel.setColour(juce::Label::textColourId, juce::Colour {50,50,50});
    
    addAndMakeVisible(mixerSlider);
    addAndMakeVisible(mixerLabel);
    
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
    
    volumeSliderA.addListener(this);
    volumeSliderB.addListener(this);
    mixerSlider.addListener(this);
    
    trackAHighPassSlider.addListener(this);
    trackAMidPassSlider.addListener(this);
    trackALowPassSlider.addListener(this);
    
    trackBHighPassSlider.addListener(this);
    trackBMidPassSlider.addListener(this);
    trackBLowPassSlider.addListener(this);
    
    lookAndFeels.emplace_back(std::move(customLookAndFeel));
    
    juce::File appDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory().getParentDirectory();
    
    juce::File imageFile = appDir.getChildFile("Resources/background_info.png");
    backgroundImage = juce::ImageCache::getFromFile(imageFile);
    
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

    g.fillAll (juce::Colour {233, 233, 233});

    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void MixerView::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    float rowH = getHeight()/8;
    float width = getWidth()/4;
    
    mixerSlider.setBounds(width, rowH * 7, width * 2, rowH);
    mixerLabel.setBounds(mixerSlider.getX() + (mixerSlider.getWidth()/2) / 2 + 10, mixerSlider.getBottom() - 90, mixerSlider.getWidth(), 20);
    
    volumeSliderA.setBounds(0, rowH * 1, getWidth()/4, rowH * 5);
    volumeSliderB.setBounds((getWidth()/4) * 3, rowH * 1, getWidth()/4, rowH * 5);
    
    trackAHighPassSlider.setBounds((getWidth()/4) * 1, rowH * 1, (getWidth()/4), rowH);
    trackAMidPassSlider.setBounds((getWidth()/4) * 1, rowH * 2, (getWidth()/4), rowH);
    trackALowPassSlider.setBounds((getWidth()/4) * 1, rowH * 3, (getWidth()/4), rowH);
    
    trackBHighPassSlider.setBounds((getWidth()/4) * 2, rowH * 1, (getWidth()/4), rowH);
    trackBMidPassSlider.setBounds((getWidth()/4) * 2, rowH * 2, (getWidth()/4), rowH);
    trackBLowPassSlider.setBounds((getWidth()/4) * 2, rowH * 3, (getWidth()/4), rowH);
}

void MixerView::sliderValueChanged(juce::Slider* slider) {
    if (slider == &volumeSliderA) {
        DBG("DeckGUI::sliderValueChanged : Gain slider value changed: " << volumeSliderA.getValue());
        djAudioPlayer1->setGain(slider->getValue());
    }
    
    if (slider == &volumeSliderB) {
        DBG("DeckGUI::sliderValueChanged : Gain slider value changed: " << volumeSliderB.getValue());
        djAudioPlayer2->setGain(slider->getValue());
    }
    
    if (slider == &mixerSlider) {
        djAudioPlayer1->setGain(1.0f - slider->getValue());
        djAudioPlayer2->setGain(slider->getValue());
    }
    
    if (slider == &trackAHighPassSlider) {
        djAudioPlayer1->setHighPassFilterAmount(slider->getValue());
    }
    
    if (slider == &trackAMidPassSlider) {
        djAudioPlayer1->setMidBandPassFilterAmount(slider->getValue());
    }
    
    if (slider == &trackALowPassSlider) {
        djAudioPlayer1->setLowPassFilterAmount(1.0f - slider->getValue());
    }
    
    if (slider == &trackBHighPassSlider) {
        djAudioPlayer2->setHighPassFilterAmount(slider->getValue());
    }
    
    if (slider == &trackBMidPassSlider) {
        djAudioPlayer2->setMidBandPassFilterAmount(slider->getValue());
    }
    
    if (slider == &trackBLowPassSlider) {
        djAudioPlayer2->setLowPassFilterAmount(1.0f - slider->getValue());
    }
}
