/*
 ==============================================================================
 
 DeckGUI.h
 Created: 29 Jan 2025 7:21:23am
 Author:  Jacques Thurling
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "CustomLookAndFeel.h"
#include "DeckWaveformDisplay.h"
#include "CSVReader.h"

//==============================================================================
/**
 * @class DeckGUI
 * @brief A graphical user interface for a DJ deck, handling playback, effects, and waveform display.
 */
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer
{
public:
    /**
     * @brief Constructor for DeckGUI.
     * @param _player Pointer to the DJAudioPlayer handling playback.
     * @param formatManager Reference to an AudioFormatManager for handling formats.
     * @param cache Reference to an AudioThumbnailCache for caching waveform thumbnails.
     * @param deck_name Name of the deck.
     * @param state Reference to the DeckState for storing deck information.
     */
    DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager& formatManager,
            juce::AudioThumbnailCache& cache, std::string deck_name, DeckState& state);
    
    /**
     * @brief Destructor for DeckGUI.
     */
    ~DeckGUI() override;
    
    /**
     * @brief Paints the graphical elements of the DeckGUI.
     * @param g JUCE Graphics context.
     */
    void paint(juce::Graphics&) override;
    
    /**
     * @brief Handles resizing of the component and child components.
     */
    void resized() override;
    
    /**
     * @brief Handles button click events.
     * @param button Pointer to the clicked button.
     */
    void buttonClicked(juce::Button* button) override;
    
    /**
     * @brief Handles slider value changes.
     * @param slider Pointer to the changed slider.
     */
    void sliderValueChanged(juce::Slider* slider) override;
    
    /**
     * @brief Checks if the component is interested in dragged files.
     * @param files Array of file paths.
     * @return True if interested, false otherwise.
     */
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    
    /**
     * @brief Handles file drop events.
     * @param file Array of file paths.
     * @param x X-coordinate of the drop location.
     * @param y Y-coordinate of the drop location.
     */
    void filesDropped(const juce::StringArray& file, int x, int y) override;
    
    /**
     * @brief Timer callback function for updating components.
     */
    void timerCallback() override;
    
    /**
     * @brief Handles mouse press events.
     * @param event Mouse event details.
     */
    void mouseDown(const juce::MouseEvent& event) override;
    
    /**
     * @brief Handles mouse drag events.
     * @param event Mouse event details.
     */
    void mouseDrag(const juce::MouseEvent& event) override;
    
    /**
     * @brief Loads an audio file from a given URL.
     * @param file The URL of the audio file.
     */
    void loadUrl(juce::URL file);
    
    /**
     * ==============================================================
     * Author: Jacques Thurling
     * 13 Mar 2020
     * ==============================================================
     * @brief Sets the initial playback position.
     * @param relativePosition The initial position as a normalized value between 0 and 1.
     */
    void setInitialPosition(double relativePosition);
    
    /**
     * @brief Sets the deck state with a filename and position.
     * @param fileName The name of the loaded file.
     * @param position The playback position.
     */
    void setDeckState(std::string fileName, double position);
    
    /**
     * @brief Sets the deck state with only the playback position.
     * @param position The playback position.
     */
    void setDeckState(double position);
    
    private:
    /**
     * ==============================================================
     * Author: Jacques Thurling
     * 13 Mar 2020
     * ==============================================================
     */
    
    std::vector<std::unique_ptr<juce::LookAndFeel>> lookAndFeels;
    
    std::string deck_name;
    DeckState& state;
    
    float rotationAngle = 0.0f;
    float startAngle = 0.0f;
    float initialRotationAngle = 0.0f;
    
    float initialRelativePosition = 0.0f;
    
    DJAudioPlayer* djAudioPlayer;
    
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton loadButton;
    
    juce::Slider volumeSlider;
    juce::Slider positionSlider;
    juce::Slider speedSlider;
    
    juce::Slider reverb;
    juce::Slider flanger;
    juce::Slider cut;
    
    juce::Label reverbLabel;
    juce::Label flangerLabel;
    juce::Label lfoLabel;
    juce::Label speedLabel;
    
    juce::Image deckImage;
    juce::Image knobImage;
    juce::Image backgroundImage;
    juce::Image deck_face_image;
    juce::Image deck_spinner_image;
    
    juce::Image play_image;
    juce::Image stop_image;
    
    juce::Image deck_number_image;
    juce::Image deck_overlay;
    
    juce::FileChooser fChooser {"Select a file..."};
    
    WaveformDisplay waveformDisplay;
    DeckWaveformDisplay deckDisplay;
    
    std::unique_ptr<juce::ImageButton> playImageButton;
    std::unique_ptr<juce::ImageButton> stopImageButton;
    
    bool play = false;
    /// ==============================================================
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
