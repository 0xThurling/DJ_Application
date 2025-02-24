#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
    
    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (1, 1);
    }
    
    reader.readCSV();
    
    addAndMakeVisible(deck1);
    addAndMakeVisible(deck2);
    addAndMakeVisible(mixerView);
    addAndMakeVisible(playlistComponent);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source
    shutdownAudio();
}

//============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
    mixer.addInputSource(&player1, false);
    mixer.addInputSource(&player2, false);
    
    mixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) {
    mixer.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    mixer.removeAllInputs();
    mixer.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

//===========================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::grey);
    
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    deck1.setBounds(0, 0, (getWidth()/8) * 3, (getHeight()/5) * 4);
    deck2.setBounds((getWidth()/8) * 5, 0, (getWidth()/8) * 3, (getHeight()/5) * 4);
    
    mixerView.setBounds((getWidth()/8) * 3, 0, (getWidth()/8) * 2, (getHeight()/5) * 4);
    
    playlistComponent.setBounds(0, (getHeight()/5) * 4, getWidth(), (getHeight()/5) * 1);
}
