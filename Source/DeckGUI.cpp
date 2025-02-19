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
    auto customLookAndFeel = std::make_unique<CustomLookAndFeel>(0.6f);
    
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    juce::File appDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory().getParentDirectory();
    
    juce::File imageFile = appDir.getChildFile("Resources/deck_spinner.png");
    deckImage = juce::ImageCache::getFromFile(imageFile);
    
    appDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory().getParentDirectory();
    
    imageFile = appDir.getChildFile("Resources/deck_face.png");
    deck_face_image = juce::ImageCache::getFromFile(imageFile);
    
    appDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory().getParentDirectory();
    
    imageFile = appDir.getChildFile("Resources/background_info.png");
    backgroundImage = juce::ImageCache::getFromFile(imageFile);
    
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    
    speedSlider.setLookAndFeel(customLookAndFeel.get());
    
    reverb.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    flanger.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    cut.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    
    reverb.setLookAndFeel(customLookAndFeel.get());
    flanger.setLookAndFeel(customLookAndFeel.get());
    cut.setLookAndFeel(customLookAndFeel.get());
    
    volumeSlider.setRange(0, 1);
    positionSlider.setRange(0, 1);
    speedSlider.setRange(0, 2);
    
    reverb.setRange(0, 1);
    flanger.setRange(0, 1);
    cut.setRange(0, 1);
    
    playButton.setButtonText("PLAY");
    stopButton.setButtonText("STOP");
    loadButton.setButtonText("LOAD");
    
    speedSlider.setValue(1.0f);
    
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(reverb);
    addAndMakeVisible(flanger);
    addAndMakeVisible(cut);
    
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    volumeSlider.addListener(this);
    positionSlider.addListener(this);
    speedSlider.addListener(this);
    
    reverb.addListener(this);
    flanger.addListener(this);
    cut.addListener(this);
    
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    
    reverb.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    flanger.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    cut.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    
    startTimer(10);
    
    lookAndFeels.emplace_back(std::move(customLookAndFeel));
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
        auto faceBounds = deck_face_image.getBounds().toFloat();

        // Calculate individual scale factors.
        float scaleX = bounds.getWidth() / (imageBounds.getWidth() * 2);
        float scaleY = bounds.getHeight() / (imageBounds.getHeight() * 2);

        // Use the minimum scale factor to preserve the aspect ratio.
        float uniformScale = std::min(scaleX, scaleY);

        // Optionally, translate the image such that it is centered.
        auto imageCentre = imageBounds.getCentre();
        auto boundsCentre = bounds.getCentre();
        auto faceCentre = faceBounds.getCentre();
        
        // Create a transform: first translate the image so its centre is at the origin,
        // then apply the uniform scaling and rotation,
        // and finally translate it to the component's centre.
        auto transform = juce::AffineTransform::translation(-imageCentre.x, -imageCentre.y)
                             .scaled(uniformScale)
                             .rotated(rotationAngle)
                             .translated(boundsCentre.x, boundsCentre.y);
        
        auto deck_face_transform = juce::AffineTransform::translation(-faceCentre.x, -faceCentre.y)
            .scaled(uniformScale)
            .translated(boundsCentre.x, boundsCentre.y);

        // Draw the transformed image.
        g.drawImage(backgroundImage, getLocalBounds().toFloat(), juce::RectanglePlacement::stretchToFit);
        g.drawImageTransformed(deckImage, transform);
        g.drawImageTransformed(deck_face_image, deck_face_transform);
    }
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    float rowH = getHeight()/8;
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    
    // Speed slider
    speedSlider.setBounds((getWidth()/8) * 7, rowH * 4, (getWidth()/8), rowH * 3);
    
    // Effects sliders
    reverb.setBounds(0, rowH * 6, (getWidth()/8) * 2, rowH);
    flanger.setBounds((getWidth()/8) * 2, rowH * 6, (getWidth()/8) * 2, rowH);
    cut.setBounds((getWidth()/8) * 4, rowH * 6, (getWidth()/8) * 2, rowH);
    
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
    
    if (slider == &reverb) {
        djAudioPlayer->setReverbAmount(slider->getValue());
    }
    
    if (slider == &flanger) {
        djAudioPlayer->setFlangerAmount(slider->getValue());
    }
    
    if (slider == &cut) {
        djAudioPlayer->setTremelo(slider->getValue());
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

void DeckGUI::mouseDown(const juce::MouseEvent& event) {
    DBG("Mouse is down");
    auto centre = getLocalBounds().toFloat().getCentre();
    startAngle = std::atan2(event.position.y - centre.y, event.position.x - centre.x);
    initialRotationAngle = rotationAngle;
    
    initialRelativePosition = djAudioPlayer->getPositionRelative();
}

void DeckGUI::mouseDrag(const juce::MouseEvent& event) {
    auto centre = getLocalBounds().toFloat().getCentre();
    float currentAngle = std::atan2(event.position.y - centre.y, event.position.x - centre.x);
    float deltaAngle = currentAngle - startAngle;
    rotationAngle = initialRotationAngle + deltaAngle;
    
    float deltaRelative = deltaAngle / (2 * juce::MathConstants<float>::pi);
    float newRelative = juce::jlimit(0.0f, 1.0f, initialRelativePosition + deltaRelative);
    
    djAudioPlayer->setPositionRelative(newRelative);
    
    repaint();
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

void DeckGUI::loadUrl(juce::URL fileURL) {
    djAudioPlayer->loadURL(fileURL);
    waveformDisplay.loadUrl(fileURL);
}
