/*
  ==============================================================================

    msgPopUp.cpp
    Created: 20 Feb 2023 9:14:34pm
    Author:  Barnabas

  ==============================================================================
*/

#include <JuceHeader.h>
#include "msgPopUp.h"

//==============================================================================
MsgPopUp::MsgPopUp()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    msg = "Default error message here. Something must be wrong if you see this";
}

MsgPopUp::~MsgPopUp()
{
}

void MsgPopUp::paint (juce::Graphics& g)
{
    g.fillAll (Colour::fromRGBA(255, 227, 224,255));   // clear the background
    Image errImg = ImageFileFormat::loadFrom(BinaryData::error_png, BinaryData::error_pngSize);
    g.drawImageAt(errImg, 5, getHeight()/3.5,0);
 
    g.drawRect (getLocalBounds(),1);   // draw an outline around the component

    //error title
    g.setColour(Colour::fromRGB(51, 46, 39));
    g.setFont(18.0f);
    g.drawText("Error", Rectangle(getWidth()/5 ,getHeight()/2 - 20,50,10),Justification::topLeft, true);
    
    //error message
    g.setFont(13.0f);
    g.drawFittedText(msg, Rectangle(getWidth() / 5, getHeight() / 2 + 10, getWidth() /2 + 60 , 20), Justification::topLeft, 2, 0.0);
}

void MsgPopUp::resized()
{
}
void MsgPopUp::setMsg(String m)
{
    msg = m;
    repaint();
}

