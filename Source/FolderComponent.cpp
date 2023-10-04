/*
  ==============================================================================

    FolderComponent.cpp
    Created: 10 Feb 2023 1:15:50pm
    Author:  Barnabas

  ==============================================================================
*/

#include "FolderComponent.h"

//==============================================================================
FolderComponent::FolderComponent(AudioPlaylists *_playlists)

    : playlists(_playlists)
{
    // table settings
    folderComponent.getHeader().addColumn("", 1, 120);
    folderComponent.setHeaderHeight(0); // hide header
    folderComponent.setModel(this);
    folderComponent.setColour(juce::ListBox::backgroundColourId, Colour::fromRGBA(15, 15, 15, 255));
    folderComponent.setColour(juce::ListBox::outlineColourId, Colour::fromRGB(23, 23, 25));
    folderComponent.setOutlineThickness(1);

    // search bar settings
    searchBar.setTextToShowWhenEmpty("search...", Colours::white);
    searchBar.addListener(this);
    searchBar.setInterceptsMouseClicks(true, false);
    searchBar.setPopupMenuEnabled(false);

    // delay this long enough for the text editor to be initialized and drawn
    Timer::callAfterDelay(100, [&]
                          { grabKeyboardFocus(); });

    // get vector of playlist names
    playListNames = playlists->getPlayListNames();

    addChildComponent(folderComponent);
    addAndMakeVisible(searchBar);

    folderComponent.addMouseListener(this, false);
}

FolderComponent::~FolderComponent()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Component essential functions
void FolderComponent::paint(juce::Graphics &g)
{
    g.fillAll(Colour::fromRGB(15, 15, 15));

    if (getNumRows() == 0)
    {
        folderComponent.setVisible(false);
        g.setColour(juce::Colours::white);
        g.setFont(14.0f);
        g.drawText("Folder Pane", Rectangle(0, 30, getWidth(), 30),
                   juce::Justification::centred, true);
        return;
    }
    folderComponent.setVisible(true);
}

void FolderComponent::resized()
{
    folderComponent.setBounds(0, 25, getWidth(), getHeight());
    searchBar.setBounds(0, 0, getWidth(), 25);
}

int FolderComponent::getNumRows()
{
    return playListNames.size();
}

void FolderComponent::paintCell(Graphics &g,
                                int rowNumber,
                                int columnId,
                                int width,
                                int height,
                                bool rowIsSelected)
{
    if (getNumRows() == 0)
        return;
    if (rowNumber >= getNumRows())
        return;
    g.fillAll(Colour::fromRGBA(10, 10, 10, 255));
    g.setColour(Colour::fromRGBA(194, 179, 165, 255));
    if (playListNames[rowNumber] == playlists->getPlayListName())
    {
        g.fillAll(Colour::fromRGBA(255, 128, 0, 80));
        g.setColour(Colours::white);
    }
    if (columnId == 1)
    {
        g.drawText(playListNames[rowNumber],
                   2, 0,
                   width - 4, height,
                   Justification::centred,
                   true);
    }
}

void FolderComponent::paintRowBackground(Graphics &g,
                                         int rowNumber,
                                         int width,
                                         int height,
                                         bool rowIsSelected)
{
    // not needed as there is only 1 column for folderComponent
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// TextEditor listener
void FolderComponent::textEditorTextChanged(TextEditor &t)
{
    // get playlist with names containing the string
    searchBarText = t.getText().toStdString();
    getParentComponent()->postCommandMessage(5);
}
std::string FolderComponent::getSearchBarText()
{
    return searchBarText;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Alert Windows

/* Alert Window to create new playlist */
void FolderComponent::newPlaylistAlertW()
{
    // create a new AlertWindow everytime user wants to create a playlist
    createPlaylistW = new AlertWindow("Create a playlist", "Enter a name for the new playlist", MessageBoxIconType::NoIcon);
    createPlaylistW->addTextEditor("inputNameEditor", "NewPlaylist", "Enter a name...", false);
    createPlaylistW->addButton("Save", 1, KeyPress(KeyPress::returnKey, 0, 0));
    createPlaylistW->addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));
    createPlaylistW->grabKeyboardFocus();
    createPlaylistW->enterModalState(true, 
            ModalCallbackFunction::create([this](int r)
            {
                if (r)
                {
                    //get text from texteditor
                    std::string newName = createPlaylistW->getTextEditorContents("inputNameEditor").toStdString();
                    if (playlists->playListNameExists(newName))
                    {
                            // postCommandMessage 2 to show error popup that playlist name exists
                            getParentComponent()->postCommandMessage(2);
                            return;
                    }
                    if (newName.empty())
                    {
                        // postCommandMessage 3 to show error popup that newName is empty string
                        getParentComponent()->postCommandMessage(3);
                        return;
                    }
                    playlists->addNewPlayList(newName);
                    playListNames = playlists->getPlayListNames();
                    playlists->setPlayListName(getNumRows() - 1);
                    getParentComponent()->postCommandMessage(1);
                    updatefolderTable();
                }
                else
                {
                    DBG("you have cancelled the creation of playlist");
                }
            // the alert Window is destroyed.
            })
        , true);
}
/* Alert Window to rename an existing playlist */
void FolderComponent::renamePlaylistAlertW()
{
    // create a new AlertWindow
    renamePlaylistW = new AlertWindow("Rename Playlist", "Enter a new name for the playlist", MessageBoxIconType::NoIcon);
    renamePlaylistW->addTextEditor("renameEditor", playListNames[currentRowNumber], "enter a name...", false);
    renamePlaylistW->addButton("Save", 1, KeyPress(KeyPress::returnKey, 0, 0));
    renamePlaylistW->addButton("Cancel", 0, KeyPress(KeyPress::escapeKey, 0, 0));

    renamePlaylistW->enterModalState(true, 
            ModalCallbackFunction::create([this](int r)
            {
                if (r)
                {
                    std::string newName = renamePlaylistW->getTextEditorContents("renameEditor").toStdString();
                    DBG(newName);
                    if (playlists->playListNameExists(newName))
                    {
                        // postCommandMessage 2 to show error popup that playlist name exists
                        getParentComponent()->postCommandMessage(2);
                    }
                    else if (newName.empty())
                    {
                        // postCommandMessage 3 to show that newName is empty string
                        getParentComponent()->postCommandMessage(3);
                    }
                    else
                    {
                        //  playlists->addNewPlayList(newName);
                        playlists->renamePlaylistName(playListNames[currentRowNumber], newName);
                        playListNames = playlists->getPlayListNames();
                        // DBG(playListNames[0]);
                        playlists->setPlayListName(currentRowNumber);
                        getParentComponent()->postCommandMessage(1);
                        updatefolderTable();
                    }
                }
                else
                {
                    DBG("you did not rename the playlist");
                }
                // the alert Window is destroyed.
            })
        ,true);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// general event listener
void FolderComponent::mouseDown(const MouseEvent &event)
{
    // only run this when table is hidden (no playlist is created)
    if (event.mods.isRightButtonDown() && !folderComponent.isVisible())
    {
        // close all active menus before creating new ones
        PopupMenu::dismissAllActiveMenus();
        PopupMenu mouseDownMenu;

        mouseDownMenu.addItem(1, "Create new playlist");

        mouseDownMenu.showMenuAsync(PopupMenu::Options(),
                    [&](int result)
                    {
                        if (result == 1)
                        {
                            // user create new playlist
                            newPlaylistAlertW();
          
                            updatefolderTable();
                        }
                    });
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
// TableListBox listener
void FolderComponent::cellClicked(int rowNumber,
                                  int columnId,
                                  const MouseEvent &event)
{
    currentRowNumber = rowNumber;

    if (event.mods.isLeftButtonDown())
    {
        playlists->setPlayListName(rowNumber);
        getParentComponent()->postCommandMessage(1);
        updatefolderTable();
    }
    /* show popup menu when table cell is right-clicked*/
    else if (event.mods.isRightButtonDown())
    {
        PopupMenu cellClickedmenu;

        cellClickedmenu.addItem(1, "Create new playlist");
        cellClickedmenu.addItem(2, "Rename playlist");
        cellClickedmenu.addItem(3, "Remove playlist");

        cellClickedmenu.showMenuAsync(PopupMenu::Options(),
                    [&](int result)
                    {
                        if (result == 1)
                        {
                            // user create new playlist
                            newPlaylistAlertW();
                        }
                        else if (result == 2)
                        {
                            // user rename playlist
                            renamePlaylistAlertW();
                        }
                        else if (result == 3)
                        {
                            // user remove playlist
                            std::string name = playListNames[currentRowNumber];
                            playlists->removeEntirePlaylist(name);
                            playListNames = playlists->getPlayListNames();

                            //send command to update PlaylistComponent
                            getParentComponent()->postCommandMessage(1);

                            updatefolderTable();
                        }
                    });
    }
    repaint();
}

void FolderComponent::backgroundClicked(const MouseEvent &event)
{
    /* show popup menu when background is right-clicked*/
    if (event.mods.isRightButtonDown() && folderComponent.isVisible())
    {
        PopupMenu backgroundClickedMenu;

        backgroundClickedMenu.addItem(1, "Create new playlist");

        backgroundClickedMenu.showMenuAsync(PopupMenu::Options(),
                    [&](int result)
                    {
                        if (result == 1)
                        {
                            // create new playlist
                            newPlaylistAlertW();
                        }
                    });
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void FolderComponent::updatefolderTable()
{
    if (getNumRows() == 0)
    {
        folderComponent.setVisible(false);
    }
    else
    {
        folderComponent.setVisible(true);
    }
    searchBar.setText("");
    folderComponent.updateContent();
    repaint();
}
