/*
 ============================================================================
 
 DeckGUI.cpp
 Created: 29 Jan 2025 7:21:23am
 Author:  Jacques Thurling
 
 ==============================================================================
 */

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
/**
 * @brief Constructor for DeckGUI.
 * @param _player Pointer to the DJAudioPlayer handling playback.
 * @param formatManager Reference to an AudioFormatManager for handling formats.
 * @param cache Reference to an AudioThumbnailCache for caching waveform thumbnails.
 * @param _deckname Name of the deck.
 * @param _state Reference to the DeckState for storing deck information.
 */
DeckGUI::DeckGUI(DJAudioPlayer* _player, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache, std::string _deckname, DeckState& _state)
: djAudioPlayer(_player), waveformDisplay(formatManager, cache), deck_name(_deckname), deckDisplay(formatManager, cache), state(_state)
{
    /**
     * ==============================================================
     * Author: Jacques Thurling
     * 13 Mar 2020
     * ==============================================================
     */
    auto customLookAndFeel = std::make_unique<CustomLookAndFeel>(0.6f);

    juce::File executableFile = juce::File::getSpecialLocation(juce::File::currentExecutableFile);
    juce::File projectDir = executableFile.getParentDirectory();
    juce::String correctPath;
    while (projectDir.getFileName() != "New_DJ" && projectDir.getParentDirectory() != projectDir) {
        projectDir = projectDir.getParentDirectory();
    }
    
    if (projectDir.getFileName() == "New_DJ") {
        correctPath = projectDir.getFullPathName();
    } else {
        // Handle the case where the New_DJ folder wasn't found
        DBG("New_DJ folder not found in the directory structure");
    }
    
    std::string imagePath = correctPath.toStdString() + "/Assets/deck_spinner.png";
    juce::File imageFile(imagePath);
    deckImage = juce::ImageCache::getFromFile(imageFile);
    
    imagePath = correctPath.toStdString() + "/Assets/deck_face.png";
    juce::File deckFaceImage(imagePath);
    deck_face_image = juce::ImageCache::getFromFile(deckFaceImage);
    
    imagePath = correctPath.toStdString() + "/Assets/background_info.png";
    juce::File backgroundInfoImage(imagePath);
    backgroundImage = juce::ImageCache::getFromFile(backgroundInfoImage);
    
    imagePath = correctPath.toStdString() + "/Assets/stop.png";
    juce::File stopImage(imagePath);
    stop_image = juce::ImageCache::getFromFile(stopImage);
    
    imagePath = correctPath.toStdString() + "/Assets/play.png";
    juce::File playImage(imagePath);
    play_image = juce::ImageCache::getFromFile(playImage);
    
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
    
    imagePath = correctPath.toStdString() + "/Assets/" + deck_name + ".png";
    juce::File deckImage(imagePath);
    deck_number_image = juce::ImageCache::getFromFile(deckImage);
    
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    
    speedSlider.setLookAndFeel(customLookAndFeel.get());
    
    reverb.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    flanger.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    cut.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    
    reverb.setLookAndFeel(customLookAndFeel.get());
    flanger.setLookAndFeel(customLookAndFeel.get());
    cut.setLookAndFeel(customLookAndFeel.get());
    
    // Labels
    reverbLabel.setText("Reverb", juce::dontSendNotification);
    reverbLabel.setFont(juce::Font("Helvetica", 14.0f, juce::Font::plain));
    reverbLabel.attachToComponent(&reverb, false);
    reverbLabel.setColour(juce::Label::textColourId, juce::Colour {50,50,50});
    
    flangerLabel.setText("Flanger", juce::dontSendNotification);
    flangerLabel.setFont(juce::Font("Helvetica", 14.0f, juce::Font::plain));
    flangerLabel.attachToComponent(&flanger, false);
    flangerLabel.setColour(juce::Label::textColourId, juce::Colour {50,50,50});
    
    lfoLabel.setText("LFO Modulation", juce::dontSendNotification);
    lfoLabel.setFont(juce::Font("Helvetica", 14.0f, juce::Font::plain));
    lfoLabel.attachToComponent(&cut, false);
    lfoLabel.setColour(juce::Label::textColourId, juce::Colour {50,50,50});
    
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setFont(juce::Font("Helvetica", 14.0f, juce::Font::plain));
    speedLabel.attachToComponent(&speedSlider, false);
    speedLabel.setColour(juce::Label::textColourId, juce::Colour {50,50,50});
    
    volumeSlider.setRange(0, 1);
    positionSlider.setRange(0, 1);
    speedSlider.setRange(0.1, 2);
    
    reverb.setRange(0, 1);
    flanger.setRange(0, 1);
    cut.setRange(0, 1);
    
    playButton.setButtonText("PLAY");
    stopButton.setButtonText("STOP");
    loadButton.setButtonText("LOAD");
    
    /// ==================================================================
    
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
    
    /**
     * ==============================================================
     * Author: Jacques Thurling
     * 13 Mar 2020
     * ==============================================================
     */
    
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
    /// ============================================================
}

/**
 * @brief Destructor for DeckGUI.
 */
DeckGUI::~DeckGUI()
{
}

/**
 * @brief Renders the deck GUI components.
 * @param g JUCE Graphics context.
 */
void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    /**
     * ==============================================================
     * Author: Jacques Thurling
     * 13 Mar 2020
     * ==============================================================
     */
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
    /// ====================================================
}

/**
 * @brief Resizes and positions the components within the DeckGUI.
 */
void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    float rowH = getHeight()/8;
    
    /**
     * ==============================================================
     * Author: Jacques Thurling
     * 13 Mar 2020
     * ==============================================================
     */
    
    // Speed slider
    speedSlider.setBounds((getWidth()/8) * 7, rowH * 4, (getWidth()/8), rowH * 4);
    speedLabel.setBounds(speedSlider.getX() + 15, speedSlider.getY() - 20, 200, 20);
    
    // Effects sliders
    reverb.setBounds((getWidth()/8) * 2, rowH * 7 - 20, (getWidth()/8), rowH);
    reverbLabel.setBounds(reverb.getX() + 10, reverb.getY() + 90, 200, 20);
    
    flanger.setBounds((getWidth()/8) * 4 - 20, rowH * 7 - 20, (getWidth()/8), rowH);
    flangerLabel.setBounds(flanger.getX() + 18, flanger.getY() + 90, 200, 20);
    
    cut.setBounds((getWidth()/8) * 6 - 40, rowH * 7 - 20, (getWidth()/8), rowH);
    lfoLabel.setBounds(cut.getX() - 10, cut.getY() + 90, 200, 20);
    
    waveformDisplay.setBounds(0, 0, getWidth(), rowH);
    
    playImageButton->setBounds(10, rowH * 7 - 50, play_image.getWidth(), play_image.getHeight());
    stopImageButton->setBounds(10, rowH * 7 - 50, stop_image.getWidth(), stop_image.getHeight());
    /// ======================================================
}

/**
 * @brief Handles button clicks for playback and loading.
 * @param button Pointer to the button that was clicked.
 */
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

/**
 * @brief Handles slider value changes.
 * @param slider Pointer to the slider that was changed.
 */
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
    
    /**
     * ==============================================================
     * Author: Jacques Thurling
     * 13 Mar 2020
     * ==============================================================
     */
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

/**
 * @brief Determines if the playlist accepts dragged files.
 *
 * @param files List of dragged file paths.
 * @return Always returns true.
 */
bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files) {
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

/**
 * @brief Handles drag-and-drop file loading.
 * @param files Array of file paths.
 * @param x X coordinate of the drop location.
 * @param y Y coordinate of the drop location.
 */
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

/**
 * @brief Timer callback function that updates the deck display.
 */
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
    
    setDeckState(djAudioPlayer->getPositionRelative());
}

void DeckGUI::loadUrl(juce::URL fileURL) {
    djAudioPlayer->loadURL(fileURL);
    waveformDisplay.loadUrl(fileURL);
    deckDisplay.loadUrl(fileURL);
    
    setDeckState(fileURL.getFileName().toStdString(), djAudioPlayer->getPositionRelative());
}
/**
 * ==============================================================
 * Author: Jacques Thurling
 * 13 Mar 2020
 * ==============================================================
 */

void DeckGUI::setDeckState(std::string fileName, double newPosition) {
    state.file_name = fileName;
    state.position = newPosition;
}

void DeckGUI::setDeckState(double newPosition) {
    state.position = newPosition;
}

/// ==============================================================
