/*
 ============================================================================
 
 Playlist.h
 Created: 4 Feb 2025 6:12:32pm
 Author:  Jacques Thurling
 
 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>
#include "WaveformDisplay.h"
#include "DeckGUI.h"

/**
 * @struct PlaylistFileInformation
 * @brief Stores file information for playlist items.
 */
struct PlaylistFileInformation {
    juce::File file; ///< File object representing the playlist item.
    juce::URL fileUrl; ///< URL to the file.
};

/**
 * @class Playlist
 * @brief Manages a playlist of audio files, providing UI and drag-and-drop support.
 */
class Playlist : public juce::Component, public juce::TableListBoxModel, public juce::Button::Listener, public juce::FileDragAndDropTarget
{
public:
    /**
     * @brief Constructor for the Playlist class.
     * @param formatManager Reference to the audio format manager.
     * @param cache Reference to the audio thumbnail cache.
     * @param deck1 Reference to the first deck.
     * @param deck2 Reference to the second deck.
     * @param _states Pointer to the vector storing deck states.
     */
    Playlist(juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache, DeckGUI& deck1, DeckGUI& deck2, std::vector<DeckState> *_states);
    
    /**
     * @brief Destructor for the Playlist class.
     */
    ~Playlist() override;
    
    /**
     * @brief Paints the component.
     * @param g Graphics context.
     */
    void paint (juce::Graphics&) override;
    
    /**
     * @brief Handles component resizing.
     */
    void resized() override;
    
    /**
     * @brief Returns the number of rows in the playlist table.
     * @return Number of rows.
     */
    int getNumRows() override;
    
    /**
     * @brief Paints the background of a row.
     * @param g Graphics context.
     * @param rowNumber Row index.
     * @param width Row width.
     * @param height Row height.
     * @param rowIsSelected Whether the row is selected.
     */
    void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    
    /**
     * @brief Paints a cell in the table.
     * @param g Graphics context.
     * @param rowNumber Row index.
     * @param columnId Column ID.
     * @param width Cell width.
     * @param height Cell height.
     * @param rowIsSelected Whether the row is selected.
     */
    void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    
    /**
     * @brief Handles cell clicks.
     * @param rowNumber Row index.
     * @param columnId Column ID.
     * @param event Mouse event.
     */
    virtual void cellClicked (int rowNumber, int columnId, const juce::MouseEvent&);
    
    /**
     * @brief Refreshes the component for a table cell.
     * @param rowNumber Row index.
     * @param columnId Column ID.
     * @param isRowSelected Whether the row is selected.
     * @param existingComponentToUpdate Existing component to update.
     * @return Pointer to the updated component.
     */
    juce::Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;
    
    /**
     * @brief Checks if the component is interested in file drag events.
     * @param files List of dragged files.
     * @return True if interested, false otherwise.
     */
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    
    /**
     * @brief Handles files being dropped onto the component.
     * @param file List of dropped files.
     * @param x X position.
     * @param y Y position.
     */
    void filesDropped (const juce::StringArray& file, int x, int y) override;
    
    /**
     * @brief Handles button clicks.
     * @param button Pointer to the clicked button.
     */
    void buttonClicked(juce::Button* button) override;
    
    /**
     * @brief Called when the parent hierarchy changes.
     */
    void parentHierarchyChanged() override
    {
        setDeckStates();
    }
    
    /**
     * @brief Sets the deck states based on the playlist.
     */
    void setDeckStates();
    
private:
    juce::AudioThumbnailCache& audioThumbnail; ///< Reference to the audio thumbnail cache.
    juce::AudioFormatManager& audioFormatManager; ///< Reference to the audio format manager.
    
    std::vector<DeckState> *states; ///< Pointer to the vector of deck states.
    
    DeckGUI& deck1; ///< Reference to the first deck.
    DeckGUI& deck2; ///< Reference to the second deck.
    
    juce::TableListBox tableComponent; ///< Table component for displaying the playlist.
    
    std::vector<PlaylistFileInformation> playlistFiles; ///< List of files in the playlist.
    
    /**
     * @brief Splits a string by a given delimiter.
     * @param s String to split.
     * @param delimiter Character delimiter.
     * @return Vector of split strings.
     */
    std::vector<std::string> split(const std::string &s, char delimiter);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Playlist)
};
