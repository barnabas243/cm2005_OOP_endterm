/*
  ==============================================================================

    WaveFormDisplay.h
    Created: 30 Jan 2023 5:30:10pm
    Author:  Barnabas

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
 */
class WaveformDisplay : public Component,
                        public ChangeListener
{
public:
  WaveformDisplay(AudioFormatManager &formatManagerToUse,
                  AudioThumbnailCache &cacheToUse);
  ~WaveformDisplay();

  void paint(Graphics &) override;
  void resized() override;

  void changeListenerCallback(ChangeBroadcaster *source) override;

  void loadURL(URL audioURL);

  /** set the relative position of the playhead*/
  void setPositionRelative(double pos);
  void setCuePos(double cuepos); // cuepos from cueButton

  
  void mouseDown(const MouseEvent &event) override;
  void mouseDrag(const MouseEvent &event) override;
  void mouseUp(const MouseEvent &event) override;

  /*setter for displaying time at mouseX playhead*/
  void setCurrentTime(std::string);
  void setRemainingTime(std::string);
  void setTimeFromPlayHead(std::string);

private:
  AudioThumbnail audioThumb;

  bool fileLoaded;
  double position;

  bool isMouseMoving = false;
  double mouseXPos;

  double cuePos;

  std::string currentTime;
  std::string remainingTime;
  std::string timeFromPlayHead;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
