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
    
    // Labels
    mixerLabel.setText("Cross-Fade", juce::dontSendNotification);
    mixerLabel.setFont(juce::Font("Helvetica", 16.0f, juce::Font::plain));
    mixerLabel.attachToComponent(&mixerSlider, false);
    mixerLabel.setColour(juce::Label::textColourId, juce::Colour {50,50,50});
    
    volumeSliderALabel.setText("Volume Deck A", juce::dontSendNotification);
    volumeSliderALabel.setFont(juce::Font("Helvetica", 16.0f, juce::Font::plain));
    volumeSliderALabel.attachToComponent(&volumeSliderA, false);
    volumeSliderALabel.setColour(juce::Label::textColourId, juce::Colour {50,50,50});
    
    volumeSliderBLabel.setText("Volume Deck B", juce::dontSendNotification);
    volumeSliderBLabel.setFont(juce::Font("Helvetica", 16.0f, juce::Font::plain));
    volumeSliderBLabel.attachToComponent(&volumeSliderB, false);
    volumeSliderBLabel.setColour(juce::Label::textColourId, juce::Colour {50,50,50});
    
    trackAHighPassSliderLabel.setText("Deck A - High", juce::dontSendNotification);
    trackAHighPassSliderLabel.setFont(juce::Font("Helvetica", 14.0f, juce::Font::plain));
    trackAHighPassSliderLabel.attachToComponent(&trackAHighPassSlider, false);
    trackAHighPassSliderLabel.setColour(juce::Label::textColourId, juce::Colour {50,50,50});
    
    trackAMidPassSliderLabel.setText("Deck A - Mid", juce::dontSendNotification);
    trackAMidPassSliderLabel.setFont(juce::Font("Helvetica", 14.0f, juce::Font::plain));
    trackAMidPassSliderLabel.attachToComponent(&trackAMidPassSlider, false);
    trackAMidPassSliderLabel.setColour(juce::Label::textColourId, juce::Colour {50,50,50});
    
    trackALowPassSliderLabel.setText("Deck A - Low", juce::dontSendNotification);
    trackALowPassSliderLabel.setFont(juce::Font("Helvetica", 14.0f, juce::Font::plain));
    trackALowPassSliderLabel.attachToComponent(&trackALowPassSlider, false);
    trackALowPassSliderLabel.setColour(juce::Label::textColourId, juce::Colour {50,50,50});
    
    trackBHighPassSliderLabel.setText("Deck B - High", juce::dontSendNotification);
    trackBHighPassSliderLabel.setFont(juce::Font("Helvetica", 14.0f, juce::Font::plain));
    trackBHighPassSliderLabel.attachToComponent(&trackBHighPassSlider, false);
    trackBHighPassSliderLabel.setColour(juce::Label::textColourId, juce::Colour {50,50,50});
    
    trackBMidPassSliderLabel.setText("Deck B - Mid", juce::dontSendNotification);
    trackBMidPassSliderLabel.setFont(juce::Font("Helvetica", 14.0f, juce::Font::plain));
    trackBMidPassSliderLabel.attachToComponent(&trackBMidPassSlider, false);
    trackBMidPassSliderLabel.setColour(juce::Label::textColourId, juce::Colour {50,50,50});
    
    trackBLowPassSliderLabel.setText("Deck B - Low", juce::dontSendNotification);
    trackBLowPassSliderLabel.setFont(juce::Font("Helvetica", 14.0f, juce::Font::plain));
    trackBLowPassSliderLabel.attachToComponent(&trackBLowPassSlider, false);
    trackBLowPassSliderLabel.setColour(juce::Label::textColourId, juce::Colour {50,50,50});
    
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
    
    volumeSliderALabel.setBounds(volumeSliderA.getX(), volumeSliderA.getY() - 20, mixerSlider.getWidth(), 20);
    volumeSliderBLabel.setBounds(volumeSliderB.getX(), volumeSliderB.getY() - 20, mixerSlider.getWidth(), 20);
    
    trackAHighPassSlider.setBounds((getWidth()/4) * 1, rowH * 1, (getWidth()/4), rowH);
    trackAHighPassSliderLabel.setBounds(trackAHighPassSlider.getX() + 10, trackAHighPassSlider.getY() + 90, mixerSlider.getWidth(), 20);
    
    trackAMidPassSlider.setBounds((getWidth()/4) * 1, rowH * 2, (getWidth()/4), rowH);
    trackAMidPassSliderLabel.setBounds(trackAMidPassSlider.getX() + 12, trackAMidPassSlider.getY() + 90, mixerSlider.getWidth(), 20);
    
    trackALowPassSlider.setBounds((getWidth()/4) * 1, rowH * 3, (getWidth()/4), rowH);
    trackALowPassSliderLabel.setBounds(trackALowPassSlider.getX() + 12, trackALowPassSlider.getY() + 90, mixerSlider.getWidth(), 20);
    
    trackBHighPassSlider.setBounds((getWidth()/4) * 2, rowH * 1, (getWidth()/4), rowH);
    trackBHighPassSliderLabel.setBounds(trackBHighPassSlider.getX() + 10, trackBHighPassSlider.getY() + 90, mixerSlider.getWidth(), 20);
    
    trackBMidPassSlider.setBounds((getWidth()/4) * 2, rowH * 2, (getWidth()/4), rowH);
    trackBMidPassSliderLabel.setBounds(trackBMidPassSlider.getX() + 12, trackBMidPassSlider.getY() + 90, mixerSlider.getWidth(), 20);
    
    trackBLowPassSlider.setBounds((getWidth()/4) * 2, rowH * 3, (getWidth()/4), rowH);
    trackBLowPassSliderLabel.setBounds(trackBLowPassSlider.getX() + 12, trackBLowPassSlider.getY() + 90, mixerSlider.getWidth(), 20);
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
