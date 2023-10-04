/*
  ==============================================================================

    audioPlaylists.h
    Created: 11 Feb 2023 5:50:00pm
    Author:  Barnabas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "TrackData.h"
#include "CSVReader.h"
#include "DeckGUI.h"

#include <string>
#include <vector>

class AudioPlaylists
{
public:
  AudioPlaylists(std::string csvFile);
  ~AudioPlaylists();

  std::vector<TrackData> getPlaylist();
  std::vector<std::string> getPlayListNames();
  std::string getPlayListName();
  
  /* change playlist*/
  bool playListNameExists(std::string name);
  void setPlayListName(int rowNumber);

  /* insert tracks */
  void insertNewAudio(TrackData track);
  /* delete tracks */
  void removeAudioFromPlaylist(TrackData track, int currentRowNumber);

  /* remove playlist */
  void removeEntirePlaylist(std::string name);
  /* rename playlist */
  void renamePlaylistName(std::string oldName, std::string newName);
  /* create new playlist */
  void addNewPlayList(std::string name);

private:
  std::string csvFile;

  std::vector<TrackData> playlists;
  std::vector<std::string> playListNames;

  std::string currentPlaylistName;
};