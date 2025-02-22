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
DeckGUI::DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache, std::string _deckname)
: djAudioPlayer(_player), waveformDisplay(formatManager, cache), deck_name(_deckname), deckDisplay(formatManager, cache)
{
    auto customLookAndFeel = std::make_unique<CustomLookAndFeel>(0.6f);
    
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    juce::File appDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory().getParentDirectory();
    
    juce::File imageFile = appDir.getChildFile("Resources/deck_spinner.png");
    deckImage = juce::ImageCache::getFromFile(imageFile);
    
    imageFile = appDir.getChildFile("Resources/deck_face.png");
    deck_face_image = juce::ImageCache::getFromFile(imageFile);
    
    imageFile = appDir.getChildFile("Resources/background_info.png");
    backgroundImage = juce::ImageCache::getFromFile(imageFile);
    
    imageFile = appDir.getChildFile("Resources/stop.png");
    stop_image = juce::ImageCache::getFromFile(imageFile);
    
    imageFile = appDir.getChildFile("Resources/overlay.png");
    deck_overlay = juce::ImageCache::getFromFile(imageFile);
    
    imageFile = appDir.getChildFile("Resources/play.png");
    play_image = juce::ImageCache::getFromFile(imageFile);
        
    playImageButton = std::make_unique<juce::ImageButton>("playImageButton");
    
    // Set images for the button states: normal, over, and down.
    playImageButton->setImages(true, true, true,
                               play_image, 1.0f, juce::Colours::transparentBlack,  // normal state image
                               play_image, 1.0f, juce::Colours::transparentBlack,  // over state image
                               play_image, 1.0f, juce::Colours::transparentBlack); // down state image

    // Make the button visible in your component
    addAndMakeVisible(playImageButton.get());
    
    stopImageButton = std::make_unique<juce::ImageButton>("stopImageButton");
    
    // Set images for the button states: normal, over, and down.
    stopImageButton->setImages(true, true, true,
                               stop_image, 1.0f, juce::Colours::transparentBlack,  // normal state image
                               stop_image, 1.0f, juce::Colours::transparentBlack,  // over state image
                               stop_image, 1.0f, juce::Colours::transparentBlack); // down state image

    // Make the button visible in your component
    addAndMakeVisible(stopImageButton.get());
    
    imageFile = appDir.getChildFile("Resources/" + deck_name + ".png");
    deck_number_image = juce::ImageCache::getFromFile(imageFile);
    
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

    addAndMakeVisible(loadButton);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(reverb);
    addAndMakeVisible(flanger);
    addAndMakeVisible(cut);
    addAndMakeVisible(deckDisplay);
    
    loadButton.addListener(this);
    volumeSlider.addListener(this);
    positionSlider.addListener(this);
    speedSlider.addListener(this);
    playImageButton->addListener(this);
    stopImageButton->addListener(this);
    
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
        g.drawImage(deck_number_image, (getWidth()/8) * 6, (getHeight()/8) + 20,
                    deck_number_image.getWidth()/3, deck_number_image.getHeight()/3,
                    0, 0, deck_number_image.getWidth(), deck_number_image.getHeight());
        g.drawImageTransformed(deckImage, transform);
        g.drawImageTransformed(deck_face_image, deck_face_transform);
        deckDisplay.setBounds(getWidth()/2 - 100, (getHeight()/8) * 4 - 50, 200, getHeight()/8);
    }
    
    if (!play)
    {
        playImageButton->setVisible(true);
        stopImageButton->setVisible(false);
    }
    else
    {
        playImageButton->setVisible(false);
        stopImageButton->setVisible(true);
    }
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    float rowH = getHeight()/8;
    
    // Speed slider
    speedSlider.setBounds((getWidth()/8) * 7, rowH * 4, (getWidth()/8), rowH * 4);
    
    // Effects sliders
    reverb.setBounds((getWidth()/8) * 2, rowH * 7 - 20, (getWidth()/8), rowH);
    flanger.setBounds((getWidth()/8) * 4 - 20, rowH * 7 - 20, (getWidth()/8), rowH);
    cut.setBounds((getWidth()/8) * 6 - 40, rowH * 7 - 20, (getWidth()/8), rowH);
    
    waveformDisplay.setBounds(0, 0, getWidth(), rowH);
    
    playImageButton->setBounds(10, rowH * 7 - 50, play_image.getWidth(), play_image.getHeight());
    stopImageButton->setBounds(10, rowH * 7 - 50, stop_image.getWidth(), stop_image.getHeight());
}

void DeckGUI::buttonClicked(juce::Button* button) {
    if (button == playImageButton.get()) {
        djAudioPlayer->start();
        play = true;
    }
    
    if (button == stopImageButton.get()) {
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
    deckDisplay.setPositionRelative(djAudioPlayer->getPositionRelative());
    
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
    deckDisplay.loadUrl(fileURL);
}
