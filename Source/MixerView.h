/**
 * =================================================================
 * @file MixerView.h
 * @brief Header file for the MixerView component.
 *
 * This file declares the MixerView class, which provides a graphical interface for
 * mixing audio from two DJAudioPlayer instances. It includes controls for volume,
 * cross-fading, and filter adjustments, along with their corresponding labels.
 *
 * Created: 5 Feb 2025 5:14:06pm
 * Author: Jacques Thurling
 */

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "CustomLookAndFeel.h"
#include "CSVReader.h"

/**
 * @class MixerView
 * @brief Component for mixing audio between two DJAudioPlayer objects.
 *
 * The MixerView class creates a user interface for audio mixing, providing sliders for
 * volume control, cross-fading, and various filters. Each slider is accompanied by a label,
 * and a custom look and feel is applied for consistent styling.
 */
class MixerView  : public juce::Component, public juce::Slider::Listener
{
public:
    /**
     * @brief Constructs a new MixerView object.
     *
     * @param _player1 Pointer to the first DJAudioPlayer.
     * @param _player2 Pointer to the second DJAudioPlayer.
     */
    MixerView(DJAudioPlayer* _player1, DJAudioPlayer* _player2);

    /**
     * @brief Destroys the MixerView object.
     */
    ~MixerView() override;
    
    /**
     * @brief Paints the MixerView component.
     *
     * This method is responsible for drawing the background and any images associated
     * with the MixerView.
     *
     * @param g The graphics context used for drawing.
     */
    void paint (juce::Graphics&) override;
    
    /**
     * @brief Resizes the MixerView component.
     *
     * This method sets the bounds of all child components when the MixerView is resized.
     */
    void resized() override;
    
    /**
     * @brief Handles slider value changes.
     *
     * This callback is invoked when any of the registered sliders change value, updating
     * the corresponding parameters in the associated DJAudioPlayer instances.
     *
     * @param slider Pointer to the slider that triggered the event.
     */
    void sliderValueChanged(juce::Slider* slider) override;
    
private:
    /// Vector storing custom look and feel objects for managing component styling.
    std::vector<std::unique_ptr<juce::LookAndFeel>> lookAndFeels;
    
    /// Pointer to the first DJAudioPlayer.
    DJAudioPlayer* djAudioPlayer1;
    
    /// Pointer to the second DJAudioPlayer.
    DJAudioPlayer* djAudioPlayer2;
    
    /// Slider used for cross-fading between the two decks.
    juce::Slider mixerSlider;
    
    /// Label for the cross-fade slider.
    juce::Label mixerLabel;
    
    /// Volume sliders for Deck A and Deck B.
    juce::Slider volumeSliderA;
    juce::Slider volumeSliderB;
    
    /// Labels for the volume sliders.
    juce::Label volumeSliderALabel;
    juce::Label volumeSliderBLabel;
    
    /// Sliders for Deck A filter adjustments (High, Mid, Low).
    juce::Slider trackAHighPassSlider;
    juce::Slider trackAMidPassSlider;
    juce::Slider trackALowPassSlider;
    
    /// Labels for the Deck A filter sliders.
    juce::Label trackAHighPassSliderLabel;
    juce::Label trackAMidPassSliderLabel;
    juce::Label trackALowPassSliderLabel;
    
    /// Sliders for Deck B filter adjustments (High, Mid, Low).
    juce::Slider trackBHighPassSlider;
    juce::Slider trackBMidPassSlider;
    juce::Slider trackBLowPassSlider;
    
    /// Labels for the Deck B filter sliders.
    juce::Label trackBHighPassSliderLabel;
    juce::Label trackBMidPassSliderLabel;
    juce::Label trackBLowPassSliderLabel;
    
    /// Background image used in the MixerView.
    juce::Image otodecksImage;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerView)
};
