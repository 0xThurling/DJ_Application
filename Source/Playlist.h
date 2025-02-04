/*
  ==============================================================================

    Playlist.h
    Created: 4 Feb 2025 6:12:32pm
    Author:  Jacques Thurling

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Playlist  : public juce::Component, public juce::TableListBoxModel, public juce::Button::Listener
{
public:
    Playlist();
    ~Playlist() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    
    void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    
    void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    
    virtual void cellClicked (int rowNumber, int columnId, const juce::MouseEvent&);
    
    juce::Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;
    
    void buttonClicked(juce::Button* button) override;
private:
    juce::TableListBox tableComponent;
    
    std::vector<std::string> trackTitle;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Playlist)
};
