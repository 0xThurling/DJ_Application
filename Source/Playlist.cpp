/*
  ==============================================================================

    Playlist.cpp
    Created: 4 Feb 2025 6:12:32pm
    Author:  Jacques Thurling

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Playlist.h"

//==============================================================================
Playlist::Playlist(juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache, DeckGUI& deck1, DeckGUI& deck2) :
                                    audioThumbnail(cache), audioFormatManager(formatManager), deck1(deck1), deck2(deck2)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(tableComponent);
    
    tableComponent.getHeader().addColumn("Track Title", 1, 200);
    tableComponent.getHeader().addColumn("Waveform", 2, 200);
    tableComponent.getHeader().addColumn("Load Deck A", 3, 200);
    tableComponent.getHeader().addColumn("Load Deck B", 4, 200);
    tableComponent.setModel(this);
}

Playlist::~Playlist()
{
    tableComponent.setModel(nullptr);
}

void Playlist::paint (juce::Graphics& g)
{
    
}

void Playlist::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    tableComponent.setBounds(0, 0, getWidth(), getHeight());

}

int Playlist::getNumRows(){
    return playlistFiles.size();
}

void Playlist::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(juce::Colours::orange);
    } else {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void Playlist::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    if (playlistFiles.size() > 0) {
        if (columnId == 1) {
            g.drawText(playlistFiles[rowNumber].fileUrl.getFileName(), 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        }
    }
}

juce::Component* Playlist::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) {
    if (columnId == 2) {
        if (existingComponentToUpdate == nullptr) {
            auto* waveform = new WaveformDisplay(audioFormatManager, audioThumbnail);
            juce::URL fileUrl = playlistFiles[rowNumber].fileUrl;
            waveform->loadUrl(fileUrl);
            existingComponentToUpdate = waveform;
        }
    }
    
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton("Load Deck A");
            existingComponentToUpdate = btn;
            juce::String id{std::to_string(rowNumber) + "-" + std::to_string(columnId)};
            btn->setComponentID(id);
            btn->addListener(this);
        }
    }
    
    if (columnId == 4) {
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

void Playlist::cellClicked(int rowNumber, int columnId, const juce::MouseEvent& event) {
    DBG(columnId);
}

void Playlist::buttonClicked(juce::Button* button) {
    std::string id = button->getComponentID().toStdString();
    
    std::vector<std::string> temp = split(id, '-');
    
//     Check if column three is selected
    if (std::stoi(temp[1]) == 3) {
        deck1.loadUrl(playlistFiles[std::stoi(temp[0])].fileUrl);
    }
    
    if (std::stoi(temp[1]) == 4) {
        std::cout << temp[0] << std::endl;
        deck2.loadUrl(playlistFiles[std::stoi(temp[0])].fileUrl);
    }
}

bool Playlist::isInterestedInFileDrag(const juce::StringArray& files) {
    std::cout << "Playlist::isInterestedInFileDrag" << std::endl;
    return true;
}

void Playlist::filesDropped(const juce::StringArray& files, int x, int y) {
    for (juce::String file : files) {
        juce::File fl = juce::File{file};
        juce::URL fileUrl = juce::URL{juce::File{fl}};
        
        PlaylistFileInformation fileInfo {
            fl,
            fileUrl
        };
        
        playlistFiles.push_back(fileInfo);
        
        tableComponent.updateContent();
        repaint();
        return;
    }
}

std::vector<std::string> Playlist::split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    for (char c : s) {
        if (c == delimiter) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token.push_back(c);
        }
    }
    // Push the final token if it exists
    if (!token.empty())
        tokens.push_back(token);
    return tokens;
}
