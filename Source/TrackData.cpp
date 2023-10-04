/*
  ==============================================================================

    TrackData.cpp
    Created: 11 Feb 2023 3:06:13pm
    Author:  Barnabas

  ==============================================================================
*/

#include "TrackData.h"

TrackData::TrackData(std::string _playListName,
                     std::string _trackTitle,
                     std::string _duration,
                     std::string _type,
                     std::string _fileLocation)
    : playListName(_playListName),
      trackTitle(_trackTitle),
      duration(_duration),
      type(_type),
      fileLocation(_fileLocation) {}
