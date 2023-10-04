/*
  ==============================================================================

    CSVReader.cpp
    Created: 11 Feb 2023 4:42:59pm
    Author:  Barnabas

  ==============================================================================
*/

#include <iostream>
#include <fstream>
#include <regex>
#include "CSVreader.h"

CSVReader::CSVReader() {};

std::vector<TrackData> CSVReader::readCSV(std::string csvFilename)
{
    std::vector<TrackData> playlist;
    std::ifstream csvFile(csvFilename);
    std::string line;
    // file is empty or does not exist
    if (is_empty(csvFile) || !csvFile.is_open())
    {
        return playlist;
    }

    if (csvFile.is_open())
    {
        while (std::getline(csvFile, line))
        {
            try {
                playlist.push_back(stringsToTrackData(tokenise(line, ',')));
            }
            catch (const std::exception& e)
            {
                DBG("CSVReader::readCSV " << e.what());
            }
        }
    }

    return playlist;
};

std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    std::vector<std::string> tokens;
    std::string token;

    std::string::size_type start = 0;

    // find position of the first delimiter
    std::string::size_type end = csvLine.find_first_of(separator, start);

    while (std::string::npos != end && std::string::npos != start)
    {
        // allow empty string to be appended to tokens
        token = csvLine.substr(start, end - start);
        tokens.push_back(token);

        start = end + 1;
        // find the position of the next delimiter
        end = csvLine.find_first_of(separator, start);
    }
    // push the last token
    tokens.push_back(csvLine.substr(start, end - start));

    // insert empty string at the back if tokens size is too little
    if (tokens.size() < 5)
    {
        for (int i = tokens.size(); i < 5; ++i)
        {
            tokens.push_back("");
        }
    }
    return tokens;
};
TrackData CSVReader::stringsToTrackData(std::vector<std::string> tokens)
{ 
    //check whether playlistName is not null
    if (tokens[0].empty() || tokens[1].empty())
    {
        throw std::exception{};
    }

    //checks whether duration is in mm:ss format
    //sometimes csv will format time as m:ss, so we need to accept single m digit.
    std::regex re("[0-9]{1,2}\\:[0-9]{2}");
    std::cmatch m;
    if (!std::regex_match(tokens[2].c_str(), m, re))
    {
        throw std::exception{};
    }
  
    //check whether fileLocation exists
    File fileLoc = File{ tokens[4] };
    if (!fileLoc.exists())
    {
        throw std::exception{};
    }

    return TrackData{ tokens[0], tokens[1], tokens[2], tokens[3], tokens[4] };
}

/*check whether csv file size is empty*/
bool CSVReader::is_empty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSVReader::insertToCSV(std::string csvFilename, std::vector<TrackData> playlists)
{
    std::ofstream myfile;
    // truncate file to remove existing data
    myfile.open(csvFilename, std::ofstream::out | std::ofstream::trunc);
    for (TrackData& p : playlists)
    {
        std::string row = p.playListName + "," + p.trackTitle + "," + p.duration + "," + p.type + "," + p.fileLocation + "\n";
        myfile << row;
    }
    myfile.close();
}