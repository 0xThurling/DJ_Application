/**
 * =================================================================
 * @file MixerView.cpp
 * @brief Implementation of the MixerView component.
 *
 * This file implements the MixerView component that manages the audio mixing
 * controls and filters for two DJ audio players. It includes setup for sliders,
 * labels, and a background image.
 *
 * Created: 5 Feb 2025 5:14:06pm
 * Author: Jacques Thurling
 */

#include <JuceHeader.h>
#include "MixerView.h"

//==============================================================================
/**
 * @brief Constructs a new MixerView object.
 *
 * This constructor initializes the mixer view by setting up various sliders for volume,
 * cross-fading, and filter controls. It also configures labels, applies a custom look
 * and feel, and loads a background image.
 *
 * @param _player1 Pointer to the first DJAudioPlayer object.
 * @param _player2 Pointer to the second DJAudioPlayer object.
 */
MixerView::MixerView(DJAudioPlayer* _player1, DJAudioPlayer* _player2)
    : djAudioPlayer1(_player1), djAudioPlayer2(_player2)
{
    // Create a custom look and feel with a specified transparency factor.
    auto customLookAndFeel = std::make_unique<CustomLookAndFeel>(0.6f);
    
    // Set slider styles for volume sliders.
    volumeSliderA.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    volumeSliderB.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    
    // Apply the custom look and feel to the volume and mixer sliders.
    volumeSliderA.setLookAndFeel(customLookAndFeel.get());
    volumeSliderB.setLookAndFeel(customLookAndFeel.get());
    mixerSlider.setLookAndFeel(customLookAndFeel.get());
    
    // Set slider styles for track filter controls (rotary style).
    trackAHighPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    trackAMidPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    trackALowPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    
    trackBHighPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    trackBMidPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    trackBLowPassSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    
    // Apply custom look and feel to the track filter sliders.
    trackAHighPassSlider.setLookAndFeel(customLookAndFeel.get());
    trackAMidPassSlider.setLookAndFeel(customLookAndFeel.get());
    trackALowPassSlider.setLookAndFeel(customLookAndFeel.get());
    
    trackBHighPassSlider.setLookAndFeel(customLookAndFeel.get());
    trackBMidPassSlider.setLookAndFeel(customLookAndFeel.get());
    trackBLowPassSlider.setLookAndFeel(customLookAndFeel.get());
    
    // Remove text boxes from sliders.
    mixerSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    volumeSliderA.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    volumeSliderB.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    
    trackAHighPassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    trackAMidPassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    trackALowPassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    
    trackBHighPassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    trackBMidPassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    trackBLowPassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    
    // Set ranges for the mixer and volume sliders.
    mixerSlider.setRange(0, 1);
    volumeSliderA.setRange(0, 1);
    volumeSliderB.setRange(0, 1);
    
    // Set ranges for the track filter sliders.
    trackAHighPassSlider.setRange(0.1, 1);
    trackAMidPassSlider.setRange(0, 1);
    trackALowPassSlider.setRange(0.1, 0.99);
    
    trackBHighPassSlider.setRange(0.1, 1);
    trackBMidPassSlider.setRange(0, 1);
    trackBLowPassSlider.setRange(0.1, 0.99);
    
    // Configure labels for the mixer and volume sliders.
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
    
    // Configure labels for track A filter sliders.
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
    
    // Configure labels for track B filter sliders.
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
    
    // Add sliders and labels to the component.
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
    
    // Set default slider values.
    mixerSlider.setValue(0.5f);
    volumeSliderA.setValue(0.5f);
    volumeSliderB.setValue(0.5f);
    
    // Register this object as a listener for slider events.
    volumeSliderA.addListener(this);
    volumeSliderB.addListener(this);
    mixerSlider.addListener(this);
    
    trackAHighPassSlider.addListener(this);
    trackAMidPassSlider.addListener(this);
    trackALowPassSlider.addListener(this);
    
    trackBHighPassSlider.addListener(this);
    trackBMidPassSlider.addListener(this);
    trackBLowPassSlider.addListener(this);
    
    // Store the custom look and feel for proper lifetime management.
    lookAndFeels.emplace_back(std::move(customLookAndFeel));
    
    // Load the background image from the application's resource directory.
    juce::File appDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile)
                            .getParentDirectory().getParentDirectory();
    
    juce::File imageFile = appDir.getChildFile("Resources/otodecks.png");
    otodecksImage = juce::ImageCache::getFromFile(imageFile);
}

/**
 * @brief Destructor for MixerView.
 */
MixerView::~MixerView()
{
}

/**
 * @brief Paints the MixerView component.
 *
 * This method fills the background with a solid color and draws the background image
 * centered at the top of the component.
 *
 * @param g The graphics context used for drawing.
 */
void MixerView::paint (juce::Graphics& g)
{
    // Fill the background with a light grey color.
    g.fillAll (juce::Colour {233, 233, 233});
    
    // Draw the background image centered horizontally at y-coordinate 20.
    g.drawImage(otodecksImage, (getWidth()/2) - ((otodecksImage.getWidth()/3)/2), 20,
                otodecksImage.getWidth()/3, otodecksImage.getHeight()/3,
                0, 0, otodecksImage.getWidth(), otodecksImage.getHeight());
}

/**
 * @brief Resizes and positions child components.
 *
 * This method calculates and sets the bounds for all child components (sliders and labels)
 * based on the current size of the MixerView.
 */
void MixerView::resized()
{
    // Calculate relative dimensions based on the current component size.
    float rowH = getHeight() / 8;
    float width = getWidth() / 4;
    
    // Set bounds for the mixer slider and its label.
    mixerSlider.setBounds(width, rowH * 7, width * 2, rowH);
    mixerLabel.setBounds(mixerSlider.getX() + (mixerSlider.getWidth()/2) / 2 + 10,
                         mixerSlider.getBottom() - 90,
                         mixerSlider.getWidth(), 20);
    
    // Set bounds for the volume sliders and their labels.
    volumeSliderA.setBounds(0, rowH * 1, getWidth() / 4, rowH * 5);
    volumeSliderB.setBounds((getWidth()/4) * 3, rowH * 1, getWidth()/4, rowH * 5);
    
    volumeSliderALabel.setBounds(volumeSliderA.getX(), volumeSliderA.getY() - 20,
                                 mixerSlider.getWidth(), 20);
    volumeSliderBLabel.setBounds(volumeSliderB.getX(), volumeSliderB.getY() - 20,
                                 mixerSlider.getWidth(), 20);
    
    // Set bounds for track A filter sliders and labels.
    trackAHighPassSlider.setBounds((getWidth()/4) * 1, rowH * 1, (getWidth()/4), rowH);
    trackAHighPassSliderLabel.setBounds(trackAHighPassSlider.getX() + 10,
                                        trackAHighPassSlider.getY() + 90,
                                        mixerSlider.getWidth(), 20);
    
    trackAMidPassSlider.setBounds((getWidth()/4) * 1, rowH * 2, (getWidth()/4), rowH);
    trackAMidPassSliderLabel.setBounds(trackAMidPassSlider.getX() + 12,
                                       trackAMidPassSlider.getY() + 90,
                                       mixerSlider.getWidth(), 20);
    
    trackALowPassSlider.setBounds((getWidth()/4) * 1, rowH * 3, (getWidth()/4), rowH);
    trackALowPassSliderLabel.setBounds(trackALowPassSlider.getX() + 12,
                                       trackALowPassSlider.getY() + 90,
                                       mixerSlider.getWidth(), 20);
    
    // Set bounds for track B filter sliders and labels.
    trackBHighPassSlider.setBounds((getWidth()/4) * 2, rowH * 1, (getWidth()/4), rowH);
    trackBHighPassSliderLabel.setBounds(trackBHighPassSlider.getX() + 10,
                                        trackBHighPassSlider.getY() + 90,
                                        mixerSlider.getWidth(), 20);
    
    trackBMidPassSlider.setBounds((getWidth()/4) * 2, rowH * 2, (getWidth()/4), rowH);
    trackBMidPassSliderLabel.setBounds(trackBMidPassSlider.getX() + 12,
                                       trackBMidPassSlider.getY() + 90,
                                       mixerSlider.getWidth(), 20);
    
    trackBLowPassSlider.setBounds((getWidth()/4) * 2, rowH * 3, (getWidth()/4), rowH);
    trackBLowPassSliderLabel.setBounds(trackBLowPassSlider.getX() + 12,
                                       trackBLowPassSlider.getY() + 90,
                                       mixerSlider.getWidth(), 20);
}

/**
 * @brief Handles slider value changes.
 *
 * This callback is invoked when any of the registered sliders changes its value.
 * It updates the corresponding parameters in the associated DJAudioPlayer objects.
 *
 * @param slider Pointer to the slider that triggered the event.
 */
void MixerView::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSliderA) {
        DBG("DeckGUI::sliderValueChanged : Gain slider value changed: " << volumeSliderA.getValue());
        djAudioPlayer1->setGain(slider->getValue());
    }
    
    if (slider == &volumeSliderB) {
        DBG("DeckGUI::sliderValueChanged : Gain slider value changed: " << volumeSliderB.getValue());
        djAudioPlayer2->setGain(slider->getValue());
    }
    
    if (slider == &mixerSlider) {
        // Cross-fade: Adjust the gains of both players based on the mixer slider value.
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
