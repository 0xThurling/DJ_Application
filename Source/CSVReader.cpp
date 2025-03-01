/**
 * =================================================================
 * @file CSVReader.cpp
 * @brief Implementation of the CSVReader class.
 *
 * This file contains the implementation for reading CSV files,
 * tokenizing CSV lines, and converting CSV data into DeckState structures.
 *
 * Created: 24 Feb 2025 9:44:39am
 * Author: Jacques Thurling
 */

#include <JuceHeader.h>
#include "CSVReader.h"
#include <fstream>

/**
 * @brief Constructor for CSVReader.
 *
 * Initializes the CSVReader object. In this constructor, any necessary
 * initializations for reading CSV files can be added.
 */
CSVReader::CSVReader()
{
    // Constructor body left intentionally empty.
}

/**
 * @brief Reads and parses the CSV file containing deck state information.
 *
 * This method locates the CSV file within the application's Resources directory,
 * opens the file, and reads its contents line by line. Each line is tokenized,
 * and the tokens are used to construct a DeckState structure. All valid DeckState
 * objects are collected and returned in a vector.
 *
 * @return std::vector<DeckState> A vector containing all deck states parsed from the CSV file.
 */
std::vector<DeckState> CSVReader::readCSV() {
    // Determine the application's directory.
    juce::File appDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile)
                              .getParentDirectory()
                              .getParentDirectory();
    // Construct the path to the CSV file.
    juce::File stateFile = appDir.getChildFile("Resources/dj_program_state.csv");
    
    // Output the full path of the state file.
    std::cout << stateFile.getFullPathName().toStdString() << std::endl;
    
    // Log a message if the file exists.
    if (stateFile.exists()) {
        std::cerr << "File exists" << std::endl;
    }
    
    // Open the CSV file using an ifstream.
    std::ifstream csvFile{stateFile.getFullPathName().toStdString()};
    
    // Check if the file stream is valid.
    if (!csvFile) {
        std::cerr << "Failed to open file" << std::endl;
    }
    
    std::string line;
    
    // Log messages indicating the loading process.
    std::cout << "=====================" << std::endl;
    std::cout << " Loading file data" << std::endl;
    std::cout << "=====================" << std::endl;
    
    // Vector to store the parsed DeckState objects.
    std::vector<DeckState> deckStates;
    
    // Check if the file is open before processing.
    if (csvFile.is_open()) {
        // Read each line of the CSV file.
        while (std::getline(csvFile, line)) {
            try {
                // Tokenize the current line using comma as the separator.
                std::vector<std::string> tokens = tokenise(line, ',');
                
                // Debug message: Convert the second token to a double.
                DBG("" << std::stod(tokens[1]));
                
                // Construct a DeckState object from the tokens.
                DeckState state {
                    tokens[0],
                    std::stod(tokens[1]),
                    tokens[2]
                };
                
                // Add the DeckState object to the vector.
                deckStates.push_back(state);
            } catch (const std::exception &e) {
                // Log a message if there is an error parsing the line.
                std::cout << "CSVReader::readCSV bad data" << std::endl;
            }
        }
    }
    
    // Close the CSV file.
    csvFile.close();
    
    // Return the vector of DeckState objects.
    return deckStates;
}

/**
 * @brief Tokenizes a CSV line into individual fields.
 *
 * This static method splits a given CSV line into tokens using the specified separator.
 * It iterates through the line to extract each token and returns a vector of tokens.
 *
 * @param csvLine A string representing a line from a CSV file.
 * @param separator The character used to separate tokens in the CSV line.
 * @return std::vector<std::string> A vector containing the tokens extracted from the CSV line.
 */
std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator) {
    // Vector to store tokens.
    std::vector<std::string> tokens;
    
    // Variables to store the starting and ending positions of tokens.
    signed int start, end;
    // String to store each individual token.
    std::string token;
    
    // Find the first character that is not the separator.
    start = csvLine.find_first_not_of(separator, 0);
    
    // Loop to extract all tokens.
    do {
        // Find the position of the next separator.
        end = csvLine.find_first_of(separator, start);
        
        // Break out of the loop if at the end of the string or if no token is found.
        if (start == csvLine.length() || start == end)
            break;
        
        // Extract the token between start and end positions.
        if (end >= 0)
            token = csvLine.substr(start, end - start);
        // Extract the token from start to the end of the string.
        else
            token = csvLine.substr(start, csvLine.length() - start);
        
        // Add the token to the vector.
        tokens.push_back(token);
        
        // Update the start position for the next token.
        start = end + 1;
    } while (end > 0);
    
    // Return the vector of tokens.
    return tokens;
}
