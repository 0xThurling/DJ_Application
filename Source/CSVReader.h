/*
  ==============================================================================

    CSVReader.h
    Created: 24 Feb 2025 9:44:39am
    Author:  Jacques Thurling

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
struct DeckState {
    std::string deck_name;
    double position;
    std::string file_name;
};

class CSVReader
{
public:
    CSVReader();
    
    static std::vector<std::string> tokenise(std::string csvLine, char separator);
    
    std::vector<DeckState> readCSV();
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CSVReader)
};
