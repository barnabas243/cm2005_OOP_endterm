/*
  ==============================================================================

    DeckGUI.cpp
    Created: 28 Jan 2023 3:14:42pm
    Author:  Barnabas

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer *_player,
                 AudioFormatManager &formatManagerToUse,
                 AudioThumbnailCache &cacheToUse)
    : player(_player), waveformDisplay(formatManagerToUse, cacheToUse)
{
    // playButton settings
    playButton.setColour(0x1000100, Colour::fromRGB(18, 18, 18));    // The colour used to fill the button shape (toggled 'off' / false).
    playButton.setColour(0x1000102, Colour::fromRGB(127, 117, 113)); // The colour to use for the button's text (toggled 'off' / false)..
    playButton.setColour(0x1000101, Colour::fromRGB(178, 76, 18));   // The colour used to fill the button shape (when the button is toggled 'on' / true).
    playButton.setColour(0x1000103, Colour::fromRGB(0, 0, 0));       // The colour to use for the button's text (when the button is toggled 'on' / true).
    playButton.setToggleable(true);

    //cueButton settings
    cueButton.setColour(0x1000100, Colour::fromRGB(18, 18, 18));
    cueButton.setColour(0x1000102, Colour::fromRGB(127, 117, 113));

    // VolSlider settings
    volSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(1.0);

    volSlider.setColour(Slider::thumbColourId, Colour::fromRGB(237, 134, 18));
    volSlider.setColour(Slider::rotarySliderFillColourId, Colour::fromRGBA(178, 76, 18, 150));
    volSlider.setColour(Slider::rotarySliderOutlineColourId, Colour::fromRGBA(127, 117, 113, 50));

    volSlider.setDoubleClickReturnValue(true, 1.0, ModifierKeys::rightButtonModifier); // allow double click to set to default
    volSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);

    // speedSlider settings
    speedSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    speedSlider.setRange(0, 5);
    speedSlider.setValue(1);

    speedSlider.setColour(Slider::thumbColourId, Colour::fromRGB(237, 134, 18));
    speedSlider.setColour(Slider::rotarySliderFillColourId, Colour::fromRGBA(178, 76, 18, 150));
    speedSlider.setColour(Slider::rotarySliderOutlineColourId, Colour::fromRGBA(127, 117, 113, 50));

    speedSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    speedSlider.setDoubleClickReturnValue(true, 1.0, ModifierKeys::rightButtonModifier);

    // set label for volSlider and speedSlider
    volLabel.attachToComponent(&volSlider, false);
    volLabel.setJustificationType(Justification::centredBottom);
    volLabel.setColour(Label::textColourId, Colour::fromRGB(194, 179, 165));

    speedLabel.attachToComponent(&speedSlider, false);
    speedLabel.setJustificationType(Justification::centredBottom);
    speedLabel.setColour(Label::textColourId, Colour::fromRGB(194, 179, 165));

    // set loopBtn image and colours during mouseEvents
    juce::Image loopImg = ImageCache::getFromMemory(BinaryData::loop_png, BinaryData::loop_pngSize);
    loopBtn.setImages(false, true, true,
        loopImg,
        0.1f, Colour::fromRGB(145, 130, 115),
        loopImg,
        0.1f, Colour::fromRGB(145, 130, 115),
        loopImg,
        0.1f, Colour::fromRGB(178, 76, 18));

    loopBtn.setToggleable(true);

    /*add components*/
    addAndMakeVisible(playButton);
    addAndMakeVisible(cueButton);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(loopBtn);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(trackDragComponent);

    playButton.addListener(this);
    cueButton.addListener(this);
    loopBtn.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);

    waveformDisplay.addMouseListener(this, false);
    trackDragComponent.addMouseListener(this, false);
    startTimer(40);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(Graphics &g)
{
    g.fillAll(Colour::fromRGBA(50, 50, 50, 255));

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    g.setColour(Colours::white);
    g.setFont(12.0f);
    g.drawText("DeckGUI", getLocalBounds(),
               Justification::centred, true);
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 8;
    double rowW = getWidth() / 10;

    trackDragComponent.setBounds(30, 0, getWidth() - 60, rowH * 2.5);
    playButton.setBounds(30, rowH * 5.8, 80, 30);
    cueButton.setBounds(30, rowH * 7, 80, 30);
    loopBtn.setBounds(rowW * 2, rowH * 5.8, 50, 50);
    volSlider.setBounds(rowW * 7, rowH * 6, 80, 70);
    speedSlider.setBounds(rowW * 8.5, rowH * 6, 80, 70);
    waveformDisplay.setBounds(30, rowH * 2.5, getWidth() - 60, rowH * 3);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        // allow togglestate to be changed when audio is loaded
        if (player->hasAudioLoaded())
        {
            // reset pos to 0.0 when user click play again after audioStream ended
            if (player->hasStreamFinished())
            {
                player->setPositionRelative(0.0);
                waveformDisplay.setPositionRelative(0.0);
                button->setButtonText("PLAY");
                return;
            }
            // start playing audio
            if (!button->getToggleState())
            {
                button->setToggleState(true, dontSendNotification);
                player->start();
                button->setButtonText("STOP");
            }
            else
            {
                player->stop();
                button->setToggleState(false, dontSendNotification);
                button->setButtonText("PLAY");
            }
        }
    }

    if (button == &cueButton)
    {
        if (cuePos != player->getPositionRelative())
        {
            cueButton.setColour(0x1000100, Colour::fromRGB(18, 18, 18));
            cueButton.setColour(0x1000102, Colour::fromRGB(127, 117, 113));

            // shift current pos to where cue is if audio is playing
            if (player->isPlayingAudio())
            {
                player->stop();
                player->setPositionRelative(cuePos);
                waveformDisplay.setPositionRelative(cuePos);

                playButton.setToggleState(false, dontSendNotification);
                playButton.setButtonText("PLAY");
            }
            // set cue to current pos if audio is paused
            else
            {
                cuePos = player->getPositionRelative();
                waveformDisplay.setCuePos(cuePos);
            }
        }
        // current pos is already at cue
        cueReady = true;
    }

    if (button == &loopBtn)
    {
        if (!button->getToggleState())
        {
            loopBtn.setToggleState(true, dontSendNotification);
        }
        else
        {
            loopBtn.setToggleState(false, dontSendNotification);
        }
    }
}

void DeckGUI::buttonStateChanged(Button* button)
{
    if (button == &cueButton)
    {
        // play audio when cue button is held down and playButton is inactive
        if (button->isMouseButtonDown() && !playButton.getToggleState() && cueReady)
        {

            cueButton.setColour(0x1000100, Colour::fromRGB(178, 76, 18));
            cueButton.setColour(0x1000102, Colour::fromRGB(18, 18, 18));
            player->start();
        }

        cueReady = false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*hide mouse when user starts dragging slider*/
void DeckGUI::sliderDragStarted(Slider* slider)
{
    slider->setMouseCursor(MouseCursor::NoCursor);
    sliderMousePos = Desktop::getInstance().getMousePosition();
}
/*show mouse when user stop dragging slider*/
void DeckGUI::sliderDragEnded(Slider* slider)
{
    slider->setMouseCursor(MouseCursor::NormalCursor);
    Desktop::getInstance().setMousePosition(sliderMousePos);
}
/*Slider value listener*/
void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Allow File Drag from Desktop */
bool DeckGUI::isInterestedInFileDrag(const StringArray &files)
{
    // only allow 1 audio file to be dragged
    if (files.size() != 1) return false;

    return true;
}
/* Receive files from desktop drag-and-drop*/
void DeckGUI::filesDropped(const StringArray &files, int x, int y)
{
    // player and waveformDisplay load url
    player->loadURL(URL{File{files[0]}});
    waveformDisplay.loadURL(URL{File{files[0]}});

    trackDragComponent.setCurrentAudioFileString(File{ files[0] }.getFullPathName());

    // reset playButton state after new audio is loaded
    playButton.setToggleState(false, dontSendNotification);
    playButton.setButtonText("PLAY");

    // reset Cue position to start
    cuePos = 0.0;
    cueReady = true;
    repaint();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Allow drag-and-drop from any Juce component except itself*/
bool DeckGUI::isInterestedInDragSource(const SourceDetails &dragSourceDetails)
{
    //cannot receive url from the same deckGUI
    if (dragSourceDetails.sourceComponent->getParentComponent() == this) return false;

    //description cannot be empty
    if (dragSourceDetails.description.equals(""))
        return false;

    return true;
}
/* drop dragSource*/
void DeckGUI::itemDropped(const SourceDetails &dragSourceDetails)
{
    //set audio path in trackDragComponent for display
    File path = File{dragSourceDetails.description.toString()};
    trackDragComponent.setCurrentAudioFileString(path.getFullPathName());

    // Load url
    player->loadURL(URL{File{dragSourceDetails.description.toString()}});
    waveformDisplay.loadURL(URL{File{dragSourceDetails.description.toString()}});

    // reset playButton state after new audio is loaded
    playButton.setToggleState(false, dontSendNotification);
    playButton.setButtonText("PLAY");

    // reset Cue position to start
    cuePos = 0.0;
    cueReady = true;
    repaint();
    DBG("DeckGUI::itemDropped");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void DeckGUI::timerCallback()
{
    if (player->hasAudioLoaded() && player->hasStreamFinished())
    {
        // allow loop when playbutton is active
        if (loopBtn.getToggleState() && player->getPositionRelative() != 0.0 && playButton.getToggleState())
        {
            player->setPositionRelative(0.0);
            player->start();
        }
        else
        {
            playButton.setToggleState(false, dontSendNotification);
            playButton.setButtonText("RESTART");
        }
    }
    waveformDisplay.setPositionRelative(
        player->getPositionRelative());

    repaint();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Receive url from playlistComponent */
void DeckGUI::sendURL(URL url)
{
    //get full path from url
    String path = URL::removeEscapeChars(url.getDomain() + "/" + url.getSubPath());

    trackDragComponent.setCurrentAudioFileString(path);
    player->loadURL(url);
    waveformDisplay.loadURL(url);

    playButton.setToggleState(false, dontSendNotification);
    playButton.setButtonText("PLAY");

    cuePos = 0.0;
    repaint();
}
bool DeckGUI::isPlayingAudio()
{
    return player->isPlayingAudio();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Load file in deckGUI with popup menu*/
void DeckGUI::mouseDown(const MouseEvent &e)
{
    /* user right-click at deckGUI and not within waveformDisplay */
    if (e.mods.isRightButtonDown() && Desktop::getInstance().findComponentAt(e.getMouseDownScreenPosition()) != &waveformDisplay)
    {
        PopupMenu menu;
        menu.addItem(1, "Load File from Directory");
        menu.showMenuAsync(PopupMenu::Options(),
            [&](int result)
            {
                if (result == 1)
                {
                    // send to deck 1
                    if (fChooser.browseForFileToOpen(nullptr))
                    {
                        if (fChooser.getResults().size() == 1)
                        {
                            player->loadURL(URL{File{fChooser.getResult()}});
                            waveformDisplay.loadURL(URL{File{fChooser.getResult()}});

                            trackDragComponent.setCurrentAudioFileString(File{ fChooser.getResult() }.getFullPathName());
                            repaint();
                        }
                    }
                }
            }
        );
    }
}
/* Display the time according to mouseX pos*/
void DeckGUI::mouseDrag(const MouseEvent& e)
{
    double mouseX = e.getPosition().getX();
    double w = waveformDisplay.getWidth();

    double pos = mouseX / w;
    //pos must be between 0 and 1
    if (pos < 0)
    {
        pos = 0;
    }
    else if (pos > 1)
    {
        pos = 1;
    }
    DBG("mousePos: " << pos);
    double totalLength = player->getTotalTime();

    double currentTime = pos * totalLength;
    double remainingTime = totalLength - std::floor(currentTime);

    double timeFromPlayHead = currentTime - std::floor(player->getCurrentTime());
    waveformDisplay.setCurrentTime(DJAudioPlayer::formatTime(currentTime));
    waveformDisplay.setRemainingTime("-" + DJAudioPlayer::formatTime(remainingTime));

    //playhead is in front of mouseX position
    if (timeFromPlayHead < 0)
    {
        //ceil currentTime when it is not 0
        if (currentTime > 0)
        {
            timeFromPlayHead = currentTime - std::ceil(player->getCurrentTime());
        }
  
        waveformDisplay.setTimeFromPlayHead("-" + DJAudioPlayer::formatTime(std::abs(timeFromPlayHead)));
    }
    else
    {
        waveformDisplay.setTimeFromPlayHead(DJAudioPlayer::formatTime(timeFromPlayHead));
    }
}

/* set audio Position of player and waveformdisplay */
void DeckGUI::mouseUp(const MouseEvent& e)
{
    // clicked within waveformDisplay
    if (Desktop::getInstance().findComponentAt(e.getMouseDownScreenPosition()) == &waveformDisplay && e.mods.isLeftButtonDown())
    {
        if (!player->isPlayingAudio())
        {
            playButton.setButtonText("PLAY");
        }
        // get mouseX value within waveFormDisplay
        double mouseX = e.getPosition().getX();
        double w = waveformDisplay.getWidth();

        double pos = mouseX / w;

        player->setPositionRelative(pos);
        waveformDisplay.setPositionRelative(
            player->getPositionRelative());
    }
}