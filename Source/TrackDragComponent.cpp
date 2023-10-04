/*
  ==============================================================================

    TrackDragComponent.cpp
    Created: 26 Feb 2023 9:04:34pm
    Author:  Barnabas

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TrackDragComponent.h"

//==============================================================================
TrackDragComponent::TrackDragComponent(DJAudioPlayer* _player)
    : player(_player)
{
}

TrackDragComponent::~TrackDragComponent()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void TrackDragComponent::paint (juce::Graphics& g)
{
    g.fillAll(Colour::fromRGBA(50, 50, 50, 255));

    if (player->hasAudioLoaded())
    {
        g.setColour(Colour::fromRGBA(255, 128, 0, 200));
        g.setFont(25.0f);

        //draw track Name
        g.drawText(player->getAudioName(), Rectangle(0, 0 , getWidth(), getHeight()), Justification::centredLeft, true);

        //draw track current Time
        g.drawText(DJAudioPlayer::formatTime( player->getCurrentTime()), Rectangle(getWidth() / 2 + 30, getHeight() / 3, getWidth() - getWidth() / 2, 20), Justification::centred, true);

        //draw track remaining Time
        g.drawText("-" + DJAudioPlayer::formatTime(player->getRemainingTime()), Rectangle(getWidth() / 2 + 80, getHeight() / 3, getWidth() - (getWidth() / 2 + 80), 20), Justification::centredRight, true);

        g.setFont(15.0f);
        //draw total time
        g.drawText(DJAudioPlayer::formatTime(player->getTotalTime()), Rectangle(getWidth() / 2 + 80, getHeight() / 3 + 15, getWidth() - (getWidth() / 2 + 80), 30), Justification::bottomRight, true);


    }
}

void TrackDragComponent::resized()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void TrackDragComponent::setCurrentAudioFileString(String fileStr)
{
    currentAudioFileString = fileStr;
}

void TrackDragComponent::mouseDown(const MouseEvent& e)
{
  //  DBG("TrackDragComponent::mouseDown():");
}
void TrackDragComponent::mouseDrag(const MouseEvent& e)
{
    if (e.mods.isRightButtonDown()) return;

    juce::DragAndDropContainer* dragC =
        juce::DragAndDropContainer::findParentDragContainerFor(this);
    
    //parentComponent(will be MainComponent) exists
    if (dragC) {
        if (!dragC->isDragAndDropActive() && currentAudioFileString.isNotEmpty())
        {
            dragC->startDragging(currentAudioFileString, this, ImageCache::getFromMemory(BinaryData::audioLogo_png, BinaryData::audioLogo_pngSize));
        }
    }
    else {
        DBG("TrackDragComponent::mouseDrag() : can't find parent drag container");
    }
}