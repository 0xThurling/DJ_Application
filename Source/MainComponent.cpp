#include "MainComponent.h"

/*
    MainComponent.cpp

    This file implements the main component of the application.
    It is responsible for:
      - Setting up the application's main window size.
      - Configuring audio input/output settings.
      - Adding and arranging the primary child components: two decks, a mixer view, and a playlist.

    The code uses JUCE's framework for handling audio and GUI components.
*/

//==============================================================================
// Constructor: Initializes the main component.
MainComponent::MainComponent()
{
    // Set the size of the main window to 1920x1080 pixels.
    // It is important to set the window size after the child components are added for proper layout.
    setSize (1920, 1080);
    
    // Check if the platform requires permission to record audio.
    // If recording permission is required but not yet granted, request permission.
    // Depending on the permission status, set the audio channels accordingly.
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        // Request audio recording permission. If granted, open 2 input channels; otherwise, open 0.
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // If no permission is required or it is already granted, initialize with 1 input and 1 output channel.
        setAudioChannels (1, 1);
    }
    
    // Add the child components to the main component and make them visible.
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
    addAndMakeVisible(mixerView);
    addAndMakeVisible(playlistComponent);
}

//==============================================================================
// Destructor: Cleans up resources when the main component is destroyed.
MainComponent::~MainComponent()
{
    // Shutdown the audio device and release any associated audio sources.
    shutdownAudio();
}

//==============================================================================
// prepareToPlay: Prepares audio sources for playback.
// Parameters:
//    samplesPerBlockExpected - Expected number of samples per audio block.
//    sampleRate - The audio sample rate.
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // Add two audio players (player1 and player2) as inputs to the mixer.
    // The 'false' parameter indicates that the mixer does not take ownership of these sources.
    mixer.addInputSource(&player1, false);
    mixer.addInputSource(&player2, false);
    
    // Prepare the mixer with the expected block size and sample rate.
    mixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

//==============================================================================
// getNextAudioBlock: Fetches the next block of audio data to be played.
// Parameter:
//    bufferToFill - Structure containing the audio buffer to be filled.
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Pass the task of filling the audio buffer to the mixer.
    mixer.getNextAudioBlock(bufferToFill);
}

//==============================================================================
// releaseResources: Releases any allocated audio resources.
// This is typically called when audio playback is stopped or the audio device is closed.
void MainComponent::releaseResources()
{
    // Remove all input sources from the mixer.
    mixer.removeAllInputs();
    
    // Release resources for the mixer and both audio players.
    mixer.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

//==============================================================================
// paint: Handles the drawing of the component.
// Parameter:
//    g - The graphics context used for rendering.
void MainComponent::paint (juce::Graphics& g)
{
    // Since the component is opaque, fill the entire background with a grey color.
    g.fillAll (juce::Colours::grey);
    
    // Additional custom drawing code can be added here if needed.
}

//==============================================================================
// resized: Called when the main component is resized.
// This method positions and resizes all child components based on the new dimensions.
void MainComponent::resized()
{
    // Set the bounds for deck1:
    // - Positioned at the top-left corner.
    // - Width: 3/8 of the total component width.
    // - Height: 4/5 of the total component height.
    deck1.setBounds(0, 0, (getWidth() / 8) * 3, (getHeight() / 5) * 4);
    
    // Set the bounds for deck2:
    // - Positioned at the top-right corner.
    // - Width: 3/8 of the total component width.
    // - Height: 4/5 of the total component height.
    deck2.setBounds((getWidth() / 8) * 5, 0, (getWidth() / 8) * 3, (getHeight() / 5) * 4);
    
    // Set the bounds for mixerView:
    // - Positioned in the center between deck1 and deck2.
    // - Width: 2/8 (or 1/4) of the total component width.
    // - Height: 4/5 of the total component height.
    mixerView.setBounds((getWidth() / 8) * 3, 0, (getWidth() / 8) * 2, (getHeight() / 5) * 4);
    
    // Set the bounds for playlistComponent:
    // - Positioned at the bottom of the window.
    // - Spanning the full width of the component.
    // - Height: 1/5 of the total component height.
    playlistComponent.setBounds(0, (getHeight() / 5) * 4, getWidth(), (getHeight() / 5) * 1);
}
