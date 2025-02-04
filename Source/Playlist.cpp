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
Playlist::Playlist()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(tableComponent);
    
    tableComponent.getHeader().addColumn("Track Title", 1, 400);
    tableComponent.getHeader().addColumn("", 2, 200);
    tableComponent.setModel(this);
    
    trackTitle.push_back("Track 1");
    trackTitle.push_back("Track 2");
    trackTitle.push_back("Track 3");
    trackTitle.push_back("Track 4");
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
    return trackTitle.size();
}

void Playlist::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(juce::Colours::orange);
    } else {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void Playlist::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    g.drawText(trackTitle[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
}

juce::Component* Playlist::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) {
    if (columnId == 2) {
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton("Play");
            existingComponentToUpdate = btn;
            juce::String id{std::to_string(rowNumber)};
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
    int id = std::stoi(button->getComponentID().toStdString());
    DBG("PlaylistComponent::buttonClicked " << trackTitle[id]);
}
