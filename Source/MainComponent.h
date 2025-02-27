#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "Playlist.h"
#include "MixerView.h"
#include "CSVReader.h"

//==============================================================================
/**
 * MainComponent class represents the central component of the application.
 * It manages the audio playback, GUI components, and user interactions.
 */
class MainComponent  : public juce::AudioAppComponent
{
    public:
    //==============================================================================
    /**
     * Constructor: Initializes the MainComponent and sets up the necessary components.
     */
    MainComponent();
    
    /**
     * Destructor: Cleans up resources when the MainComponent is destroyed.
     */
    ~MainComponent() override;
    
    //==============================================================================
    /**
     * Prepares the audio system for playback.
     * Called before playback starts.
     * @param samplesPerBlockExpected The expected number of samples per block.
     * @param sampleRate The audio sample rate.
     */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    
    /**
     * Processes and provides the next block of audio data.
     * @param bufferToFill The buffer that needs to be filled with audio data.
     */
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    
    /**
     * Releases any allocated audio resources when playback stops.
     */
    void releaseResources() override;
    
    //==============================================================================
    /**
     * Handles painting of the component.
     * @param g The graphics context used for drawing.
     */
    void paint (juce::Graphics& g) override;
    
    /**
     * Handles resizing of the component and repositioning of child components.
     */
    void resized() override;
    
    private:
    //==============================================================================
    // Manages audio format readers.
    juce::AudioFormatManager formatManager;
    
    // Caches audio thumbnails for faster waveform rendering.
    juce::AudioThumbnailCache thumbnailCache {20};
    
    // Reads deck states from a CSV file.
    CSVReader reader;
    std::vector<DeckState> states = reader.readCSV();
    
    // First deck and its associated player.
    DJAudioPlayer player1;
    DeckGUI deck1 {&player1, formatManager, thumbnailCache, "deck_a", states[0]};
    
    // Second deck and its associated player.
    DJAudioPlayer player2;
    DeckGUI deck2 {&player2, formatManager, thumbnailCache, "deck_b", states[1]};
    
    // Mixer view that allows volume control and crossfading between decks.
    MixerView mixerView{&player1, &player2};
    
    // Playlist component that manages track loading and display.
    Playlist playlistComponent {formatManager, thumbnailCache, deck1, deck2, &states};
    
    // Mixer that combines audio signals from different sources.
    juce::MixerAudioSource mixer;
    
    // JUCE macro to prevent copying and enable leak detection.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
