/*
 ==============================================================================
 
 CSVReader.cpp
 Created: 24 Feb 2025 9:44:39am
 Author:  Jacques Thurling
 
 ==============================================================================
 */

#include <JuceHeader.h>
#include "CSVReader.h"
#include <fstream>

//==============================================================================
CSVReader::CSVReader()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
}

std::vector<std::string>  CSVReader::readCSV() {
    // Open CSV file
    juce::File appDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile).getParentDirectory().getParentDirectory();
    juce::File stateFile = appDir.getChildFile("Resources/dj_program_state.csv");
    
    std::cout << stateFile.getFullPathName().toStdString() << std::endl;
    
    if (stateFile.exists()) {
        std::cerr << "File exists" << std::endl;
    }
    
    std::ifstream csvFile{stateFile.getFullPathName().toStdString()};
    // String to store each line of the CSV file
    
    if (!csvFile) { // Correct: Checks if the stream is in a bad state
        std::cerr << "Failed to open file" << std::endl;
    }
    
    std::string line;
    // Log Message
    std::cout << "=====================" << std::endl;
    std::cout << " Loading file data" << std::endl;
    std::cout << "=====================" << std::endl;
    
    // Check if the file is open
    if (csvFile.is_open()) {
        // Read each line of the file
        while (std::getline(csvFile, line)) {
            try {
                // Tokenise the line and convert to WeatherEntry objects
                return tokenise(line, ',');
            } catch (const std::exception &e) {
                // Log Message for bad data
                std::cout << "CSVReader::readCSV bad data" << std::endl;
            }
        } // end of while
    }
}

std::vector<std::string> CSVReader::tokenise(std::string csvLine,
                                             char separator) {
    // Vector to store tokens
    std::vector<std::string> tokens;
    // Variables to store start and end positions of tokens
    signed int start, end;
    // String to store each token
    std::string token;
    // Fine the first non-seporator character
    start = csvLine.find_first_not_of(separator, 0);
    do {
        // Find the next separator char
        end = csvLine.find_first_of(separator, start);
        // Break if end of line or no more
        if (start == csvLine.length() || start == end)
            break;
        // Extract the token
        if (end >= 0)
            token = csvLine.substr(start, end - start);
        // Extract the last token
        else
            token = csvLine.substr(start, csvLine.length() - start);
        // Add the token to the vector
        tokens.push_back(token);
        // Update the start position
        start = end + 1;
    } while (end > 0);
    
    // Return the vector of tokens
    return tokens;
}
