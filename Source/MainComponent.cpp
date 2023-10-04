/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1280, 700);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);  
    addAndMakeVisible(playlistComponent);
    addAndMakeVisible(folderComponent);

    addChildComponent(&msgPopup);

    //run the function after the texteditor is initialized and drawn,
    Timer::callAfterDelay(100, [&] { grabKeyboardFocus(); });
 
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll(Colour::fromRGBA(15, 15, 15, 255));
}

void MainComponent::resized()
{
    double h = getHeight() / 3;
    deckGUI1.setBounds(0, 0, getWidth()/2, h);
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, h);

    //audioPlaylists.setBounds(0, h, getWidth(), getHeight() - h);
    playlistComponent.setBounds(getWidth()/7, h, getWidth() - getWidth() / 7, getHeight() - h);
    folderComponent.setBounds(0, h, getWidth() / 7, getHeight() - h);
    msgPopup.setBounds(0, getHeight() - 150, getWidth() / 5,80);
  }

/* Receive commands from child Components*/
void MainComponent::handleCommandMessage(int command)
{
    // update the playlist to show changes
    if (command == 1)
    {
        playlistComponent.updatePlaylist();
    }

    // display duplicate playlistname error
    if (command == 2)
    {
        msgPopup.setMsg("Playlist name already exists.");

        if (Desktop::getInstance().getAnimator().isAnimating())
        {
            Desktop::getInstance().getAnimator().cancelAnimation(&msgPopup, true);
        }
        Desktop::getInstance().getAnimator().fadeIn(&msgPopup,800);
        Timer::callAfterDelay(5000, [&] { Desktop::getInstance().getAnimator().fadeOut(&msgPopup, 1000); });
    }

    // display empty playlistname error
    if (command == 3)
    {
        msgPopup.setMsg("Playlist name cannot be empty!");

        if (Desktop::getInstance().getAnimator().isAnimating())
        {
            Desktop::getInstance().getAnimator().cancelAnimation(&msgPopup, true);
        }
        Desktop::getInstance().getAnimator().fadeIn(&msgPopup, 800);
        Timer::callAfterDelay(5000, [&] { Desktop::getInstance().getAnimator().fadeOut(&msgPopup, 1000); });
    }

    // run 'create new playlist' alert Window 
    if (command == 4)
    {
        //create new playlist
        folderComponent.newPlaylistAlertW();
    }

    // filter playlistCOmponent table with searchbar text
    if (command == 5)
    {
        playlistComponent.setSearchBarText(folderComponent.getSearchBarText());
    }
}