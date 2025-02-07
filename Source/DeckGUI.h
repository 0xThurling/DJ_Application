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
private:
    float rotationAngle = 0.0f;
    DJAudioPlayer* djAudioPlayer;
    
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton loadButton;
    
    juce::Slider volumeSlider;
    juce::Slider positionSlider;
    juce::Slider speedSlider;
    
    juce::Image deckImage;
    
    juce::FileChooser fChooser {"Select a file..."};
    
    WaveformDisplay waveformDisplay;
    
    bool play = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
