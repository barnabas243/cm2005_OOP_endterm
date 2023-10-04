/*
  ==============================================================================

    DeckGUI.h
    Created: 28 Jan 2023 3:14:42pm
    Author:  Barnabas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "TrackDragComponent.h"
//==============================================================================
/*
 */
class DeckGUI : public Component,
                public Button::Listener,
                public Slider::Listener,
                public FileDragAndDropTarget,
                public DragAndDropTarget,
                public Timer
{
public:
  DeckGUI(DJAudioPlayer *player,
          AudioFormatManager &formatManagerToUse,
          AudioThumbnailCache &cacheToUse);
  ~DeckGUI();

  void paint(Graphics &) override;
  void resized() override;

  /** implement Button::Listener */
  void buttonClicked(Button*) override;
  void buttonStateChanged(Button*) override;

  /** implement Slider::Listener */
  void sliderDragStarted(Slider* slider) override;
  void sliderDragEnded(Slider* slider) override;
  void sliderValueChanged(Slider* slider) override;

  /** FileDragAndDropTarget methods */
  bool isInterestedInFileDrag(const StringArray &files) override;
  void filesDropped(const StringArray &files, int x, int y) override;

  /** DragAndDropTarget methods */
  bool isInterestedInDragSource(const SourceDetails &dragSourceDetails);
  void itemDropped(const SourceDetails &dragSourceDetails);

  /** Mouse listeners*/
  void mouseDown(const MouseEvent &) override;
  void mouseDrag(const MouseEvent&) override;
  void mouseUp(const MouseEvent &event) override;

  /** receive audio url from playlistComponent */
  void sendURL(URL url);

  bool isPlayingAudio();

  void timerCallback() override;

private:

  TextButton playButton{"PLAY"};
  TextButton cueButton{"CUE"};
  ImageButton loopBtn;

  Label volLabel{"volLabel", "volume"};
  Label speedLabel{"speedLabel", "speed"};

  FileChooser fChooser{"Select a File"};

  Slider volSlider;
  Slider speedSlider;
  Point<int> sliderMousePos;

  String currentAudioFileString;

  double cuePos = 0.0;
  bool cueReady = false;
  WaveformDisplay waveformDisplay;

  DJAudioPlayer *player;
  TrackDragComponent trackDragComponent{player};
  double pos = 0.0;
  bool isMouseMoving = false;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
