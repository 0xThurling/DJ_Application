/**
 * =================================================================
 * @file Playlist.cpp
 * @brief Implementation of the Playlist class for managing audio tracks.
 *
 * This class handles loading, displaying, and managing an audio playlist,
 * including interactions with two decks.
 *
 * @author Jacques Thurling
 * @date 4 Feb 2025
 */

#include <JuceHeader.h>
#include "Playlist.h"

/**
 * @brief Constructs a Playlist object.
 *
 * Initializes the playlist with predefined tracks, configures the table UI,
 * and registers basic audio formats.
 *
 * @param formatManager Reference to an AudioFormatManager.
 * @param cache Reference to an AudioThumbnailCache.
 * @param deck1 Reference to the first DeckGUI.
 * @param deck2 Reference to the second DeckGUI.
 * @param _states Pointer to a vector of DeckState objects.
 */
Playlist::Playlist(juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache, DeckGUI& deck1, DeckGUI& deck2, std::vector<DeckState> *_states) :
audioThumbnail(cache), audioFormatManager(formatManager), deck1(deck1), deck2(deck2), states(_states)
{
    formatManager.registerBasicFormats();
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
    
    // Load predefined tracks into the playlist
    std::vector<std::string> trackNames = {"Escape.mp3", "Cool.mp3", "Faster.mp3", "History.mp3", "Funk.mp3", "Louder.mp3", "Love.mp3"};
    for (const auto& track : trackNames) {
        juce::File file(correctPath.toStdString() + "/Assets/" + track);
        playlistFiles.push_back({file, juce::URL(file)});
    }
    
    tableComponent.updateContent();
    repaint();
    
    // Configure table component UI
    addAndMakeVisible(tableComponent);
    tableComponent.getHeader().addColumn("Track Title", 1, 200);
    tableComponent.getHeader().addColumn("Track Length", 2, 200);
    tableComponent.getHeader().addColumn("Waveform", 3, 200);
    tableComponent.getHeader().addColumn("Load Deck A", 4, 200);
    tableComponent.getHeader().addColumn("Load Deck B", 5, 200);
    tableComponent.setModel(this);
}

/**
 * @brief Destructor for Playlist.
 *
 * Cleans up by detaching the table model.
 */
Playlist::~Playlist()
{
    tableComponent.setModel(nullptr);
}

/**
 * @brief Paints the background of the Playlist component.
 *
 * @param g Graphics context.
 */
void Playlist::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::grey);
}

/**
 * @brief Resizes and arranges the table component.
 */
void Playlist::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
    for (int i = 1; i <= 5; ++i) {
        tableComponent.getHeader().setColumnWidth(i, getWidth() / 5);
    }
    tableComponent.getHeader().setColour(juce::TableHeaderComponent::backgroundColourId, juce::Colours::white);
}

/**
 * @brief Returns the number of rows in the playlist.
 *
 * @return Number of tracks in the playlist.
 */
int Playlist::getNumRows()
{
    return playlistFiles.size();
}

/**
 * @brief Handles the background color of a row.
 *
 * @param g Graphics context.
 * @param rowNumber Index of the row.
 * @param width Row width.
 * @param height Row height.
 * @param rowIsSelected True if the row is selected.
 */
void Playlist::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    g.fillAll(rowIsSelected ? juce::Colours::lightblue : (rowNumber % 2 == 0 ? juce::Colours::darkgrey : juce::Colours::grey));
}

/**
 * @brief Paints a cell in the table.
 *
 * @param g Graphics context.
 * @param rowNumber Row index.
 * @param columnId Column index.
 * @param width Cell width.
 * @param height Cell height.
 * @param rowIsSelected True if the row is selected.
 */
void Playlist::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.setColour(juce::Colours::white);
    if (columnId == 1) {
        g.drawText(playlistFiles[rowNumber].fileUrl.getFileName(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
    } else if (columnId == 2) {
        if (auto* reader = audioFormatManager.createReaderFor(playlistFiles[rowNumber].file)) {
            double lengthInSeconds = static_cast<double>(reader->lengthInSamples) / reader->sampleRate;
            g.drawText(juce::String::formatted("%d:%02d", static_cast<int>(lengthInSeconds / 60), static_cast<int>(lengthInSeconds) % 60), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            delete reader;
        }
    }
}

/**
 * @brief Refreshes a table cell component.
 *
 * @param rowNumber Row index.
 * @param columnId Column index.
 * @param isRowSelected True if the row is selected.
 * @param existingComponentToUpdate Pointer to the existing component.
 * @return Updated component pointer.
 */
juce::Component* Playlist::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) {
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {
            auto* waveform = new WaveformDisplay(audioFormatManager, audioThumbnail);
            juce::URL fileUrl = playlistFiles[rowNumber].fileUrl;
            waveform->loadUrl(fileUrl);
            existingComponentToUpdate = waveform;
        }
    }
    
    if (columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton("Load Deck A");
            existingComponentToUpdate = btn;
            juce::String id{std::to_string(rowNumber) + "-" + std::to_string(columnId)};
            btn->setComponentID(id);
            btn->addListener(this);
        }
    }
    
    if (columnId == 5) {
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton("Load Deck B");
            existingComponentToUpdate = btn;
            juce::String id{std::to_string(rowNumber) + "-" + std::to_string(columnId)};
            btn->setComponentID(id);
            btn->addListener(this);
        }
    }
    
    return existingComponentToUpdate;
}

/**
 * @brief Handles table cell clicks.
 *
 * @param rowNumber Row index.
 * @param columnId Column index.
 * @param event Mouse event details.
 */
void Playlist::cellClicked(int rowNumber, int columnId, const juce::MouseEvent& event) {
    DBG(columnId);
}

/**
 * @brief Handles button clicks for loading tracks into decks.
 *
 * @param button Pointer to the clicked button.
 */
void Playlist::buttonClicked(juce::Button* button)
{
    std::vector<std::string> temp = split(button->getComponentID().toStdString(), '-');
    int rowIndex = std::stoi(temp[0]);
    int columnIndex = std::stoi(temp[1]);
    
    if (columnIndex == 4) {
        deck1.loadUrl(playlistFiles[rowIndex].fileUrl);
    } else if (columnIndex == 5) {
        deck2.loadUrl(playlistFiles[rowIndex].fileUrl);
    }
}

/**
 * @brief Determines if the playlist accepts dragged files.
 *
 * @param files List of dragged file paths.
 * @return Always returns true.
 */
bool Playlist::isInterestedInFileDrag(const juce::StringArray& files) {
    std::cout << "Playlist::isInterestedInFileDrag" << std::endl;
    return true;
}

/**
 * @brief Handles file drops for adding tracks to the playlist.
 *
 * @param files List of dropped file paths.
 * @param x Drop position x-coordinate.
 * @param y Drop position y-coordinate.
 */
void Playlist::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (const auto& file : files) {
        playlistFiles.push_back({juce::File(file), juce::URL(juce::File(file))});
        tableComponent.updateContent();
        repaint();
        return;
    }
}

/**
 * @brief Splits a string by a delimiter.
 *
 * @param s Input string.
 * @param delimiter Character used as the delimiter.
 * @return Vector of split substrings.
 */
std::vector<std::string> Playlist::split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::istringstream stream(s);
    std::string token;
    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * @brief Loads deck states from saved settings.
 */
void Playlist::setDeckStates() {
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
    
    for (auto const &state : *states) {
        if (state.deck_name == "deck_a") {
            juce::File file(correctPath.toStdString() + "/Assets/" + state.file_name);
            juce::URL fileUrl = juce::URL{file};
            
            deck1.loadUrl(fileUrl);
        }
        
        if (state.deck_name == "deck_b") {
            juce::File file(correctPath.toStdString() + "/Assets/" + state.file_name);
            juce::URL fileUrl = juce::URL{file};
            
            deck2.loadUrl(fileUrl);
        }
    }
}
