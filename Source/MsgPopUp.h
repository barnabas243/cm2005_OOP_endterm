/*
  ==============================================================================

    msgPopUp.h
    Created: 20 Feb 2023 9:14:34pm
    Author:  Barnabas

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MsgPopUp  : public juce::Component
{
public:
    MsgPopUp();
    ~MsgPopUp() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /* set error message */
    void setMsg(String msg);

private:
    String msg;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MsgPopUp)

      
};
