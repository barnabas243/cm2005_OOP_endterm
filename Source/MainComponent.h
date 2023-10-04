/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioPlaylists.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "FolderComponent.h"
#include "msgPopUp.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent,
                      public DragAndDropContainer
{
public:
  //==============================================================================
  MainComponent();
  ~MainComponent();

  //==============================================================================
  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
  void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
  void releaseResources() override;

  //==============================================================================
  void paint(Graphics &g) override;
  void resized() override;

  void handleCommandMessage(int command) override;

private:
  //==============================================================================
  // Your private member variables go here...

  AudioFormatManager formatManager;
  AudioThumbnailCache thumbCache{100};

  DJAudioPlayer player1{formatManager};
  DeckGUI deckGUI1{&player1, formatManager, thumbCache};

  DJAudioPlayer player2{formatManager};
  DeckGUI deckGUI2{&player2, formatManager, thumbCache};
  MixerAudioSource mixerSource;

  // set default directory for saving csv 
  std::string csvFile = File::getCurrentWorkingDirectory().getFullPathName().toStdString() + "\\playlists.csv";
  AudioPlaylists playlists{csvFile};

  PlaylistComponent playlistComponent{&deckGUI1, &deckGUI2, &playlists, formatManager};
  FolderComponent folderComponent{&playlists};

  MsgPopUp msgPopup;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
