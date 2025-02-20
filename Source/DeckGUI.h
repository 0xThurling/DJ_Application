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

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component, public juce::Button::Listener, public juce::Slider::Listener, public juce::FileDragAndDropTarget, public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& file, int x, int y) override;
    
    void timerCallback() override;
    
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    
    void loadUrl(juce::URL file);
private:
    std::vector<std::unique_ptr<juce::LookAndFeel>> lookAndFeels;
    
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
    
    juce::Image deckImage;
    juce::Image knobImage;
    juce::Image backgroundImage;
    juce::Image deck_face_image;
    juce::Image deck_spinner_image;
    
    juce::Image play_image;
    juce::Image stop_image;
    
    juce::FileChooser fChooser {"Select a file..."};
    
    WaveformDisplay waveformDisplay;
    
    std::unique_ptr<juce::ImageButton> playImageButton;
    std::unique_ptr<juce::ImageButton> stopImageButton;
    
    bool play = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
