/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 28 Jan 2023 2:40:36pm
    Author:  Barnabas

  ==============================================================================
*/


#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource 
{
  public:

    DJAudioPlayer(AudioFormatManager& _formatManager);  
    ~DJAudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    static std::string remove_extension(std::string filename);
    void loadURL(URL audioURL);

    /* setters */
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    
    bool hasAudioLoaded();
    void start();
    void stop();
    bool isPlayingAudio();
    double getPositionRelative();
    bool hasStreamFinished();
  
    /* used to display time */
    static std::string formatTime(int sec);
    std::string getAudioName();
    double getTotalTime();
    double getCurrentTime();
    double getRemainingTime();

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource; 
    ResamplingAudioSource resampleSource{&transportSource, false, 2};

    std::string audioName;
};




