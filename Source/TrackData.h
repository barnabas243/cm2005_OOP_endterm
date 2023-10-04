/*
  ==============================================================================

    TrackData.h
    Created: 11 Feb 2023 3:06:13pm
    Author:  Barnabas

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include <string>

class TrackData
{
public:
    TrackData(std::string _playListName,
              std::string _trackTitle,
              std::string _duration,
              std::string _type,
              std::string _fileLocation
        );

    std::string playListName;
    std::string trackTitle;
    std::string duration;
    std::string type;
    std::string fileLocation;

};