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
class CSVReader
{
public:
    CSVReader();
    
    static std::vector<std::string> tokenise(std::string csvLine, char separator);
    
    std::vector<std::string> readCSV();
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CSVReader)
};
