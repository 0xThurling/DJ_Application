/*
 ==============================================================================
 
 Playlist.h
 Created: 4 Feb 2025 6:12:32pm
 Author:  Jacques Thurling
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include "WaveformDisplay.h"
#include "DeckGUI.h"

//==============================================================================
/*
 */
struct PlaylistFileInformation {
    juce::File file;
    juce::URL fileUrl;
};

class Playlist  : public juce::Component, public juce::TableListBoxModel, public juce::Button::Listener, public juce::FileDragAndDropTarget
{
    public:
    Playlist(juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache, DeckGUI& deck1, DeckGUI& deck2, std::vector<DeckState> *_states);
    ~Playlist() override;
    
    void paint (juce::Graphics&) override;
    void resized() override;
    
    int getNumRows() override;
    
    void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    
    void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    
    virtual void cellClicked (int rowNumber, int columnId, const juce::MouseEvent&);
    
    juce::Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;
    
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& file, int x, int y) override;
    
    void buttonClicked(juce::Button* button) override;
    
    void parentHierarchyChanged() override
    {
        setDeckStates();
    }

    void setDeckStates();
    private:
    juce::AudioThumbnailCache& audioThumbnail;
    juce::AudioFormatManager& audioFormatManager;
    
    std::vector<DeckState> *states;
    
    DeckGUI& deck1;
    DeckGUI& deck2;
    
    juce::TableListBox tableComponent;
    
    std::vector<PlaylistFileInformation> playlistFiles;
    
    std::vector<std::string> split(const std::string &s, char delimiter);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Playlist)
};
