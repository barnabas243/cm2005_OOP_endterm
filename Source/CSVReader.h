/*
  ==============================================================================

    CSVReader.h
    Created: 11 Feb 2023 4:42:59pm
    Author:  Barnabas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "TrackData.h"
#include <string>
#include <vector>

class CSVReader {
public:
    CSVReader();
    /** read csv and tokenize lines*/
    static std::vector<TrackData> readCSV(std::string csvFile);
    static void insertToCSV(std::string csvFile, std::vector<TrackData> playlists);

    /* checks whether csv file is empty*/
    static bool CSVReader::is_empty(std::ifstream& pFile);

    /* tokenise the csv line */
    static std::vector<std::string> tokenise(std::string csvLine, char separator);

    /* convert tokens to TrackData Obj */
    static TrackData stringsToTrackData(std::vector<std::string> tokens);


};