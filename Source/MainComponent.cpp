/**
 * @file MainComponent.cpp
 * @brief Implementation of the MainComponent class.
 *
 * This component sets up the main window of the application, initializes
 * audio settings, and arranges the child components.
 */

#include "MainComponent.h"

/**
 * @brief Constructs a MainComponent object.
 *
 * Sets the window size, handles audio permission requests,
 * configures audio channels, and adds the child components to the main window.
 */
MainComponent::MainComponent()
{
    // Set the size of the main component (width: 1920, height: 1080 pixels).
    setSize (1920, 1080);
    
    // Check if runtime permission is required for recording audio.
    // If required but not yet granted, request permission and set audio channels based on the result.
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // If permission is not required or is already granted, initialize with 1 input and 1 output channel.
        setAudioChannels (1, 1);
    }
    
    // Add child components and make them visible.
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
    addAndMakeVisible(mixerView);
    addAndMakeVisible(playlistComponent);
}

/**
 * @brief Destructor for MainComponent.
 *
 * Shuts down the audio device and releases audio resources.
 */
MainComponent::~MainComponent()
{
    shutdownAudio();
}

/**
 * @brief Prepares the audio system for playback.
 *
 * Called before playback starts to configure the audio mixer with expected settings.
 *
 * @param samplesPerBlockExpected The expected number of samples per audio block.
 * @param sampleRate The audio sample rate.
 */
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // Add two audio players as input sources for the mixer.
    mixer.addInputSource(&player1, false);
    mixer.addInputSource(&player2, false);
    
    // Prepare the mixer with the expected block size and sample rate.
    mixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

/**
 * @brief Provides the next block of audio data.
 *
 * Delegates the task of filling the audio buffer to the mixer.
 *
 * @param bufferToFill Structure containing the audio buffer to be filled.
 */
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixer.getNextAudioBlock(bufferToFill);
}

/**
 * @brief Releases audio resources.
 *
 * Removes all inputs from the mixer and releases resources for the mixer and audio players.
 */
void MainComponent::releaseResources()
{
    mixer.removeAllInputs();
    mixer.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

/**
 * @brief Renders the component.
 *
 * Fills the background with a solid grey color and serves as a placeholder for any additional drawing code.
 *
 * @param g The graphics context used for drawing.
 */
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::grey);
    // Additional drawing code can be added here if needed.
}

/**
 * @brief Handles component resizing.
 *
 * Adjusts the positions and sizes of all child components when the main component is resized.
 */
void MainComponent::resized()
{
    // Set bounds for deck1: positioned at the left.
    deck1.setBounds(0, 0, (getWidth() / 8) * 3, (getHeight() / 5) * 4);
    
    // Set bounds for deck2: positioned at the right.
    deck2.setBounds((getWidth() / 8) * 5, 0, (getWidth() / 8) * 3, (getHeight() / 5) * 4);
    
    // Set bounds for mixerView: positioned in the center.
    mixerView.setBounds((getWidth() / 8) * 3, 0, (getWidth() / 8) * 2, (getHeight() / 5) * 4);
    
    // Set bounds for playlistComponent: positioned at the bottom.
    playlistComponent.setBounds(0, (getHeight() / 5) * 4, getWidth(), (getHeight() / 5) * 1);
}
