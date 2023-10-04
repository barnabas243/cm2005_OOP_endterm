/*
  ==============================================================================

    audioPlaylists.cpp
    Created: 11 Feb 2023 5:50:00pm
    Author:  Barnabas

  ==============================================================================
*/
#include <JuceHeader.h>
#include "AudioPlaylists.h"
#include <algorithm>    

AudioPlaylists::AudioPlaylists(std::string _csvFile)
    : csvFile(_csvFile)
{
    playlists = CSVReader::readCSV(csvFile);
    if (playlists.size() > 0) 
    {
        playListNames = getPlayListNames();
        currentPlaylistName = playListNames[0];
    }
}
AudioPlaylists::~AudioPlaylists() {
  
    CSVReader::insertToCSV(csvFile, playlists);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<TrackData> AudioPlaylists::getPlaylist()
{
    std::vector<TrackData> playlist;

    for (TrackData& p : playlists)
    {
            if (p.playListName == currentPlaylistName)
            {
                playlist.push_back(p);
            }
    }
    return playlist;
}

std::vector<std::string> AudioPlaylists::getPlayListNames() {
    std::vector<std::string> playListNames;

    for (TrackData& p : playlists)
    {
        bool isPresent = std::find(playListNames.begin(), playListNames.end(), p.playListName) != playListNames.end();
        if (!isPresent)
        {
            playListNames.push_back(p.playListName);
        }
    }
    return playListNames;

}
std::string AudioPlaylists::getPlayListName()
{
    return currentPlaylistName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void AudioPlaylists::insertNewAudio(TrackData track)
{
    playlists.push_back(track);
}

bool compareFunction(TrackData& a, TrackData& b) {
    return a.playListName < b.playListName;
}

void AudioPlaylists::removeAudioFromPlaylist(TrackData track, int currentRowNumber)
{
    //sort the playlist so that playlistNames are grouped
    std::sort(playlists.begin(), playlists.end(), compareFunction);

    //find the first element with the specified playlistName of the track
    auto it = std::find_if(playlists.begin(),
                   playlists.end(), 
        [&](auto& track) { return track.playListName == currentPlaylistName; });

    if (it != playlists.end())
    {
        //get the index position of iterator
        int idx = it - playlists.begin();
        
        if (track.playListName == playlists[idx + currentRowNumber].playListName && track.trackTitle == playlists[idx + currentRowNumber].trackTitle)
        {
            playlists.erase(playlists.begin() + idx + currentRowNumber);
            if (getPlaylist().size() == 0)
            {
                //need this to show an empty playlist
                insertNewAudio(TrackData{currentPlaylistName,"","","",""});
            }
        }
    }
 }
   
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool AudioPlaylists::playListNameExists(std::string name)
{
    return std::find(playListNames.begin(), playListNames.end(), name) != playListNames.end();
}

void AudioPlaylists::setPlayListName(int rowNumber)
{
    currentPlaylistName = playListNames[rowNumber];

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void AudioPlaylists::removeEntirePlaylist(std::string name)
{
    //check if name is actually present in vector
    bool isPresent = std::find(playListNames.begin(), playListNames.end(), name) != playListNames.end();
    if (!isPresent) return;

    for (auto i = 0; i < playlists.size(); ++i)
    {
        if (playlists[i].playListName == name)
        {
            playlists.erase(playlists.begin() + i);
            i--;
        }
    }
    playListNames.erase(std::remove(playListNames.begin(), playListNames.end(), name), playListNames.end());

    if (playListNames.size() !=0)
    {
        if (!playListNames[playListNames.size() - 1].empty())
        {
            currentPlaylistName = playListNames[playListNames.size() - 1];
            return;
        }
    }
    currentPlaylistName = "";
}


void AudioPlaylists::renamePlaylistName(std::string oldName,std::string newName)
{
    for (auto i = 0; i < playlists.size(); ++i)
    {
        if (playlists[i].playListName == oldName)
        {
            playlists[i].playListName = newName;
        }
    }
    playListNames = getPlayListNames();
    currentPlaylistName = newName;
}

void AudioPlaylists::addNewPlayList(std::string name)
{
    //there is no playlist created
    if (playListNames.size() == 0)
    {
        playlists.push_back(TrackData{ name,"","","","" });
        playListNames.push_back(name);
        currentPlaylistName = name;
    }
    else {
        bool isPresent = std::find(playListNames.begin(), playListNames.end(), name) != playListNames.end();
        if (!isPresent)
        {
            playlists.push_back(TrackData{ name,"","","","" });
            playListNames.push_back(name);
            currentPlaylistName = name;
        }
    }
};