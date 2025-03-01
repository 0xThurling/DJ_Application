/**
 * =================================================================
 * @file CSVReader.h
 * @brief Header file for the CSVReader class and DeckState structure.
 *
 * Created: 24 Feb 2025 9:44:39am
 * Author: Jacques Thurling
 */

#pragma once

#include <JuceHeader.h>

/**
 * @brief Structure representing the state of a deck.
 *
 * Contains the deck's name, current position, and associated file name.
 */
struct DeckState {
    std::string deck_name;   /**< The name identifier for the deck. */
    double position;         /**< The current position in the deck (e.g., time or progress). */
    std::string file_name;   /**< The name of the file associated with the deck. */
};

/**
 * @brief Class for reading and parsing CSV files.
 *
 * The CSVReader class provides functionality to tokenize a CSV line and
 * read a CSV file, converting each row into a DeckState structure.
 */
class CSVReader
{
public:
    /**
     * @brief Constructor for CSVReader.
     *
     * Initializes the CSVReader object.
     */
    CSVReader();
    
    /**
     * @brief Splits a CSV line into individual tokens.
     *
     * This static method takes a CSV line and a separator character, and returns
     * a vector containing each token extracted from the line.
     *
     * @param csvLine A string representing a line from a CSV file.
     * @param separator The character used to delimit fields in the CSV line.
     * @return std::vector<std::string> A vector of tokens parsed from the CSV line.
     */
    static std::vector<std::string> tokenise(std::string csvLine, char separator);
    
    /**
     * @brief Reads and parses a CSV file.
     *
     * Processes the CSV file and converts its content into a vector of DeckState structures.
     *
     * @return std::vector<DeckState> A vector containing the deck states read from the CSV file.
     */
    std::vector<DeckState> readCSV();
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CSVReader)
};
