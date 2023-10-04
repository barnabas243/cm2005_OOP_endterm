/*
  ==============================================================================

    WaveFormDisplay.cpp
    Created: 30 Jan 2023 5:30:10pm
    Author:  Barnabas

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager &formatManagerToUse,
                                 AudioThumbnailCache &cacheToUse) 
      : audioThumb(1000, formatManagerToUse, cacheToUse),
        fileLoaded(false),
        position(0)

{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
  cuePos = 0.0;
  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(Graphics &g)
{
  
  g.fillAll(Colour::fromRGB(25, 25, 26)); 

  g.setColour(Colour::fromRGB(0, 0, 0));
  g.drawRect(getLocalBounds(), 1); 

  g.setColour(Colour::fromRGBA(5, 177, 255, 150));
  if (fileLoaded)
  {
    audioThumb.drawChannel(g,
                           getLocalBounds(),
                           0,
                           audioThumb.getTotalLength(),
                           0,
                           1.0f);
    // 'play' playhead Pos
    g.setColour(Colour::fromRGB(255, 128, 0));
    g.drawRect(position * getWidth(), 0, 1, getHeight());

    // 'cue' playhead Pos
    g.setColour(Colour::fromRGB(169, 156, 145));
    g.drawRect(cuePos * getWidth(), 0, 1, getHeight());
    g.setFont(15.0f);
    g.setColour(Colour::fromRGB(243, 243, 243));
    g.drawText("C", Rectangle(int(cuePos * getWidth()) + 5, 5, 10, 10), Justification::topRight, true);

    // fill passed time with grey colour shade
    if (position > 0)
    {
      g.setColour(Colour::fromRGB(21, 21, 21));
      g.setOpacity(0.6);
      g.fillRect(0, 0, position * getWidth(), getHeight());
    }

    // show a playhead when user is using mouse to set pos of audio
    if (isMouseMoving)
    {   
      // draw the mouseX playhead
      g.setColour(Colour::fromRGBA(255, 128, 0,200));
      g.drawRect(mouseXPos, 0, 1, getHeight());

      g.setColour(Colour::fromRGB(243, 243, 243));
      g.setFont(15.0f);
      // display time on current MouseX position
      // shift text to the left side when we are reaching around 80% of wfd width
      if (int(mouseXPos > getWidth() - getWidth()/5))
      {
          g.drawText(currentTime, Rectangle(int(mouseXPos) - 100, getHeight() / 2 - 25, 40, 10), Justification::topLeft, true);
          g.drawText(remainingTime, Rectangle(int(mouseXPos) - 100, getHeight() / 2 - 25, 90, 10), Justification::topRight, true);
          g.drawText(timeFromPlayHead, Rectangle(int(mouseXPos) - 100, getHeight() / 2, 90, 10), Justification::topRight, true);
      }
      else {
          g.drawText(currentTime, Rectangle(int(mouseXPos) + 10, getHeight() / 2 - 25, 40, 10), Justification::topLeft, true);
          g.drawText(remainingTime, Rectangle(int(mouseXPos) + 10, getHeight() / 2 - 25, 90, 10), Justification::topRight, true);
          g.drawText(timeFromPlayHead, Rectangle(int(mouseXPos) + 10, getHeight() / 2, 60, 10), Justification::topLeft, true);
      }
   
    }
  }
  else
  {
    g.setFont(20.0f);
    g.drawText("File not loaded...", getLocalBounds(),
               Justification::centred, true); // draw some placeholder text
  }
}

void WaveformDisplay::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
} 

////////////////////////////////////////////////////////////////////////////////////////////////////////
void WaveformDisplay::loadURL(URL audioURL)
{
  audioThumb.clear();
  fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
  if (fileLoaded)
  {
    DBG("wfd: loaded! ");
    cuePos = 0.0;
  
  } 
  else
  {
    DBG("wfd: not loaded! ");
  }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster *source)
{
  repaint();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void WaveformDisplay::setPositionRelative(double pos)
{
  if (pos != position)
  {
    position = pos;
    repaint();
  }
}

void WaveformDisplay::setCuePos(double cuepos)
{
  cuePos = cuepos;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
/* set the time to display in the mouseX playhead*/
void WaveformDisplay::setCurrentTime(std::string cT)
{
    currentTime = cT;
    repaint();
}
void WaveformDisplay::setRemainingTime(std::string rT)
{
    remainingTime = rT;
    repaint();
}
void WaveformDisplay::setTimeFromPlayHead(std::string pT)
{
    timeFromPlayHead = pT;
    repaint();
}

/* Mouse events to set Position*/
void WaveformDisplay::mouseDown(const MouseEvent &e)
{
  mouseXPos = e.getPosition().getX();
  isMouseMoving = true;
  repaint();
}

void WaveformDisplay::mouseDrag(const MouseEvent &e)
{
  // mouseDown position is beyond waveFormDisplay width
  if (e.getPosition().getX() < 0 || e.getPosition().getX() > getWidth())
  {
    return;
  }
 
  mouseXPos = e.getPosition().getX();

  isMouseMoving = true;
  repaint();
}

void WaveformDisplay::mouseUp(const MouseEvent &event)
{
  isMouseMoving = false;
  repaint();
}

