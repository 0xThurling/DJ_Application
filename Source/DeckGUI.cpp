/*
  ==============================================================================

    DeckGUI.cpp
    Created: 29 Jan 2025 7:21:23am
    Author:  Jacques Thurling

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache)
                : djAudioPlayer(_player), waveformDisplay(formatManager, cache)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    juce::File appDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory().getParentDirectory();
    
    juce::File imageFile = appDir.getChildFile("Resources/deck.png");
    deckImage = juce::ImageCache::getFromFile(imageFile);
    
    
    volumeSlider.setRange(0, 1);
    positionSlider.setRange(0, 1);
    speedSlider.setRange(0, 1);
    
    playButton.setButtonText("PLAY");
    stopButton.setButtonText("STOP");
    loadButton.setButtonText("LOAD");
    
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(waveformDisplay);
    
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volumeSlider.addListener(this);
    positionSlider.addListener(this);
    speedSlider.addListener(this);
    
    startTimer(30);
}

DeckGUI::~DeckGUI()
{
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    
    if (deckImage.isValid()) {
        auto bounds = getLocalBounds().toFloat();
        auto imageBounds = deckImage.getBounds().toFloat();

        // Calculate individual scale factors.
        float scaleX = bounds.getWidth() / imageBounds.getWidth();
        float scaleY = bounds.getHeight() / imageBounds.getHeight();

        // Use the minimum scale factor to preserve the aspect ratio.
        float uniformScale = std::min(scaleX, scaleY);

        // Optionally, translate the image such that it is centered.
        auto imageCentre = imageBounds.getCentre();
        auto boundsCentre = bounds.getCentre();

        // Create a transform: first translate the image so its centre is at the origin,
        // then apply the uniform scaling and rotation,
        // and finally translate it to the component's centre.
        auto transform = juce::AffineTransform::translation(-imageCentre.x, -imageCentre.y)
                             .scaled(uniformScale)
                             .rotated(rotationAngle)
                             .translated(boundsCentre.x, boundsCentre.y);

        // Draw the transformed image.
        g.drawImageTransformed(deckImage, transform);
    }
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    float rowH = getHeight()/8;
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH * 7, getWidth(), rowH);
}

void DeckGUI::buttonClicked(juce::Button* button) {
    if (button == &playButton) {
        DBG("DeckGUI::buttonClicked: You clicked the play button");
        djAudioPlayer->setPosition(0);
        djAudioPlayer->start();
        play = true;
    }
    
    if (button == &stopButton) {
        DBG("DeckGUI::buttonClicked: You clicked the stop button");
        djAudioPlayer->stop();
        play = false;
    }
    
    if (button == &loadButton) {
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
        
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser){
            auto chosenFile = chooser.getResult();
            djAudioPlayer->loadURL(juce::URL{chosenFile});
            waveformDisplay.loadUrl(juce::URL{chosenFile});
        });
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider) {
    if (slider == &volumeSlider) {
        DBG("DeckGUI::sliderValueChanged : Gain slider value changed: " << volumeSlider.getValue());
        djAudioPlayer->setGain(slider->getValue());
    }
    
    if (slider == &positionSlider) {
        djAudioPlayer->setPositionRelative(slider->getValue());
    }
    
    if (slider == &speedSlider) {
        djAudioPlayer->setSpeed(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files) {
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y) {
    for (juce::String file : files) {
        juce::URL fileUrl = juce::URL{juce::File{file}};
        
        djAudioPlayer->loadURL(fileUrl);
        waveformDisplay.loadUrl(fileUrl);
        return;
    }
}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(djAudioPlayer->getPositionRelative());
    
    if (play) {
        rotationAngle += 0.02f;
        
        if (rotationAngle >= juce::MathConstants<float>::twoPi) {
            rotationAngle -= juce::MathConstants<float>::twoPi;
        }
        
        repaint();
    }
}
