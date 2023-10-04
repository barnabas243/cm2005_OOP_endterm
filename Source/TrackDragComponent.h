/*
  ==============================================================================

    TrackDragComponent.h
    Created: 22 Feb 2023 9:04:34pm
    Author:  Barnabas

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
//==============================================================================
/*
*/
class TrackDragComponent  : public Component,
                            public DragAndDropContainer
{
public:
    TrackDragComponent(DJAudioPlayer* player);
    ~TrackDragComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /*set the file path of the audio being loaded*/
    void setCurrentAudioFileString(String fileStr);

    void mouseDown(const MouseEvent&) override;
    void mouseDrag(const MouseEvent&) override;

private:
    DJAudioPlayer* player;
    String currentAudioFileString;
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackDragComponent)
};
