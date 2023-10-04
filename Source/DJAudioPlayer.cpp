/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 28 Jan 2023 2:40:36pm
    Author:  Barnabas

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager &_formatManager)
    : formatManager(_formatManager)
{
}
DJAudioPlayer::~DJAudioPlayer()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string DJAudioPlayer::remove_extension(std::string filename)
{
    // get pos of .
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos)
    {
        // . is not in the filename
        return filename;
    }
    return filename.substr(0, lastdot);
}
void DJAudioPlayer::loadURL(URL audioURL)
{
    auto *reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // supported file format
    {
        // remove the file extension from filename to be displayed in DeckGUI
        audioName = remove_extension(audioURL.getFileName().toStdString());

        // set source
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader,
                                                                                       true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/*set audio gain from slider value*/
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    }
    else
    {
        transportSource.setGain(gain);
    }
}
/*set audio speed/ratio from slider value*/
void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 5.0)
    {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 5");
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}
/*set the audio playback position */
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}


void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DJAudioPlayer::hasAudioLoaded()
{
    return transportSource.getLengthInSeconds() != 0;
}

void DJAudioPlayer::start()
{
    transportSource.start();
}
void DJAudioPlayer::stop()
{
    transportSource.stop();
}

bool DJAudioPlayer::isPlayingAudio()
{
    return transportSource.isPlaying();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

bool DJAudioPlayer::hasStreamFinished()
{
    return transportSource.hasStreamFinished();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Functions for displaying audio information in DeckGUI
std::string DJAudioPlayer::getAudioName()
{
    return audioName;
}

// format the time using strftime
std::string DJAudioPlayer::formatTime(int sec)
{
    std::tm tm{};
    tm.tm_min = int(sec / 60);
    tm.tm_sec = int(sec) % 60;
    char time[50];
    strftime(time, sizeof(time), "%M:%S", &tm);
    return time;
}

double DJAudioPlayer::getTotalTime()
{
    return transportSource.getLengthInSeconds();
}
double DJAudioPlayer::getCurrentTime()
{
    return transportSource.getCurrentPosition();
}

double DJAudioPlayer::getRemainingTime()
{
    // floor the currentPosition to make sure it only decrement when the actual time passes
    return transportSource.getLengthInSeconds() - std::floor(transportSource.getCurrentPosition());
}
