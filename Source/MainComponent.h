#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "Playlist.h"
#include "MixerView.h"
#include "CSVReader.h"

//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class MainComponent  : public juce::AudioAppComponent
{
    public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;
    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    private:
    //==============================================================================
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache {20};
    
    CSVReader reader;
    std::vector<DeckState> states = reader.readCSV();
    
    DJAudioPlayer player1;
    DeckGUI deck1 {&player1, formatManager, thumbnailCache, "deck_a", states[0]};
    
    DJAudioPlayer player2;
    DeckGUI deck2 {&player2, formatManager, thumbnailCache, "deck_b", states[1]};
    
    MixerView mixerView{&player1, &player2};
    
    Playlist playlistComponent {formatManager, thumbnailCache, deck1, deck2, &states};
    
    juce::MixerAudioSource mixer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
