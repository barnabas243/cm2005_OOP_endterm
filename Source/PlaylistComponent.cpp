/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 3 Feb 2023 1:37:18pm
    Author:  Barnabas

  ==============================================================================
*/

#include "PlaylistComponent.h"
#include "FolderComponent.h"
#include <map>
#include <string>
#include <ctime>
#include <numeric>
//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI *_deckGUI1, DeckGUI *_deckGUI2, AudioPlaylists *_playlists, AudioFormatManager &_formatManager)
    : deckGUI1(_deckGUI1), deckGUI2(_deckGUI2), playlists(_playlists), formatManager(_formatManager)
{

    // tableComponent to show audio tracks in the playlist
    tableComponent.getHeader().addColumn("#", 1, 30);
    tableComponent.getHeader().addColumn("Title", 2, 150);
    tableComponent.getHeader().addColumn("Duration", 3, 100);
    tableComponent.getHeader().addColumn("Type", 4, 200);
    tableComponent.getHeader().addColumn("Location", 5, 200);
    tableComponent.setModel(this);

    tableComponent.setColour(juce::ListBox::backgroundColourId, Colour::fromRGBA(15, 15, 15, 255));
    tableComponent.setMultipleSelectionEnabled(true);
    tableComponent.setHeaderHeight(25);

    addChildComponent(tableComponent);
    tableComponent.addMouseListener(this, true);

    // get current playlist
    currentPlaylist = playlists->getPlaylist();

    // fill the indx vector
    indxV.resize(currentPlaylist.size());
    std::iota(indxV.begin(), indxV.end(), 0);

    // createPlaylistBtn settings (imitate a hyperlink)
    createPlaylistBtn.setButtonText("Create a playlist");
    createPlaylistBtn.setFont(Font(14.0f, 4), false, Justification::centredLeft);
    addChildComponent(createPlaylistBtn);
    createPlaylistBtn.addListener(this);

    addChildComponent(arrowBtn);
}

PlaylistComponent::~PlaylistComponent()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlaylistComponent::paint(juce::Graphics &g)
{

    g.fillAll(Colour::fromRGB(15, 15, 15));

    g.setColour(Colour::fromRGBA(194, 179, 165, 255));
    //no playlist is created
    if (getNumRows() == 0 && playlists->getPlayListName().empty())
    {
        tableComponent.setVisible(false);
        g.setFont(25.0f);
        g.drawText("Create a new Playlist", Rectangle(20, 10, getWidth() / 2, 50),
                   juce::Justification::topLeft, true);

        g.setFont(14.0f);
        g.drawSingleLineText("You have not created any playlists.", 20, 80);
        g.drawSingleLineText("To get started, you can 'right-click' at the Folder Pane or click the button below.", 20, 120);

        createPlaylistBtn.setVisible(true);
        arrowBtn.setVisible(true);
    }
    //playlist created but no tracks added
    else if (currentPlaylist.size() != 0 && currentPlaylist[0].trackTitle.empty())
    {
        //hide buttons
        createPlaylistBtn.setVisible(false);
        arrowBtn.setVisible(false);

        // playlistname header
        g.setFont(20.0f);
        g.drawText(playlists->getPlayListName(), Rectangle(20, 10, getWidth() / 2, 50),
                   juce::Justification::topLeft, true);

        // text about what to do to insert track
        g.setFont(14.0f);
        g.drawSingleLineText("The playlist is empty. You can 'right-click' or drag-and-drop here to load tracks into this playlist.", 20, 60);
    }
    else
    {
        tableComponent.setVisible(true);
    }
}

void PlaylistComponent::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
    createPlaylistBtn.setBounds(20, getHeight() / 3, 200, 30);
    arrowBtn.setBounds(5, 110, 10, 15);
}

int PlaylistComponent::getNumRows()
{
    return indxV.size();
}

void PlaylistComponent::paintRowBackground(Graphics &g,
                                           int rowNumber,
                                           int width,
                                           int height,
                                           bool rowIsSelected)
{
    if (getNumRows() == 0)
        return;

    if (rowNumber != 0 && rowNumber >= getNumRows())
        return;
    if (currentPlaylist[indxV[rowNumber]].playListName.empty() || (currentPlaylist[indxV[rowNumber]].trackTitle.empty() && getNumRows() == 1))
    {
        tableComponent.setVisible(false);
        return;
    }

    if (rowIsSelected)
    {
        g.fillAll(Colour::fromRGBA(255, 128, 0, 80));

        currentRowNumber = indxV[rowNumber];
    }
    else
    {
        if (rowNumber % 2 == 0)
        {
            g.fillAll(Colour::fromRGBA(10, 10, 10, 255));
        }
    }
    tableComponent.updateContent();
}

void PlaylistComponent::paintCell(Graphics &g,
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
    if (currentPlaylist[indxV[rowNumber]].playListName.empty())
        return;
    if (currentPlaylist[indxV[rowNumber]].trackTitle.empty() && getNumRows() == 1)
        return;
    if (currentPlaylist[indxV[rowNumber]].trackTitle.empty() && getNumRows() > 1)
    {
        playlists->removeAudioFromPlaylist(currentPlaylist[indxV[rowNumber]], indxV[rowNumber]);

        updatePlaylist();

        return;
    }
    //increment rowNumber by 1 and convert to string
    std::string rowNum = std::to_string(indxV[rowNumber] + 1);
    g.setColour(Colour::fromRGBA(194, 179, 165, 255));
    if (rowIsSelected)
    {
        g.setColour(Colours::white);
    }
    if (columnId == 1)
    {
        // row number
        g.drawText(rowNum,
                   2, 0,
                   width - 4, height,
                   Justification::centredLeft,
                   true);
    }
    else if (columnId == 2)
    {
        // trackTitle
        g.drawText(currentPlaylist[indxV[rowNumber]].trackTitle,
                   2, 0,
                   width - 4, height,
                   Justification::centredLeft,
                   true);
    }
    else if (columnId == 3)
    {
        // duration
        g.drawText(currentPlaylist[indxV[rowNumber]].duration,
                   2, 0,
                   width - 4, height,
                   Justification::centredLeft,
                   true);
    }
    else if (columnId == 4)
    {
        // type
        g.drawText(currentPlaylist[indxV[rowNumber]].type,
                   2, 0,
                   width - 4, height,
                   Justification::centredLeft,
                   true);
    }
    else if (columnId == 5)
    {
        //fileLocation
        g.drawText(currentPlaylist[indxV[rowNumber]].fileLocation,
                   2, 0,
                   width - 4, height,
                   Justification::centredLeft,
                   true);
    }
}

Component *PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId,
                                                      bool isRowSelected,
                                                      Component *existingComponentToUpdate)
{
    return existingComponentToUpdate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlaylistComponent::buttonClicked(Button *button)
{
    if (button == &createPlaylistBtn)
    {
        //create new playlist
        getParentComponent()->postCommandMessage(4);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/* File Drag And Drop methods */
bool PlaylistComponent::isInterestedInFileDrag(const StringArray &files)
{
    DBG("PlaylistComponent::isInterestedInFileDrag");
    return true;
}

void PlaylistComponent::filesDropped(const StringArray &files, int x, int y)
{
    DBG("PlaylistComponent::filesDropped");
    for (String f : files)
    {
        insertNewTracks(File{f});
    }
    currentPlaylist = playlists->getPlaylist();
    indxV.resize(currentPlaylist.size());
    std::iota(indxV.begin(), indxV.end(), 0);

    tableComponent.setVisible(true);
    tableComponent.updateContent();
    repaint();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Drag And Drop Target methods*/
bool PlaylistComponent::isInterestedInDragSource(const SourceDetails &dragSourceDetails)
{
    //source parent component must be from DeckGUI
    if (dragSourceDetails.sourceComponent->getParentComponent() != deckGUI1 && dragSourceDetails.sourceComponent->getParentComponent() != deckGUI2)
        return false;

    //description cannot be empty
    if (dragSourceDetails.description.equals(""))
        return false;

    return true;
}
void PlaylistComponent::itemDropped(const SourceDetails &dragSourceDetails)
{
    DBG("PlaylistComponent::itemDropped");
    DBG(dragSourceDetails.description.toString());

    insertNewTracks(File{dragSourceDetails.description.toString()});
    updatePlaylist();

    DBG("PlaylistComponent::itemDropped passed");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
var PlaylistComponent::getDragSourceDescription(const SparseSet<int> &currentlySelectedRows)
{
    //currentlySelectedRows will only have 1 row
    return currentPlaylist[currentlySelectedRows[0]].fileLocation;
}

void PlaylistComponent::mouseDrag(const MouseEvent &e)
{
    if (getNumRows() == 0)
        return;
    if (e.mods.isRightButtonDown())
        return;

    DragAndDropContainer *dragR =
        DragAndDropContainer::findParentDragContainerFor(this);

    /*Parent DragContainer is MainComponent*/
    if (dragR)
    {
        if (!dragR->isDragAndDropActive())
        {
            SparseSet<int> selectedRow;
            // only gets the first selected row number
            int row = tableComponent.getSelectedRow();
            //no row selected
            if (row == -1) return;
            
            tableComponent.selectRow(row);
            currentRowNumber = indxV[row];

            //only add the currentRowNumber into selectedRows
            selectedRow.addRange(Range<int>(currentRowNumber, currentRowNumber + 1));

            dragR->startDragging(getDragSourceDescription(selectedRow),this,ImageCache::getFromMemory(BinaryData::audioLogo_png, BinaryData::audioLogo_pngSize));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlaylistComponent::insertNewTracks(File f)
{
    if (playlists->getPlayListName().empty())
        return;

    URL audioURL = URL{f};

    auto *reader = formatManager.createReaderFor(URL{f}.createInputStream(false));
    if (reader != nullptr) // supported format
    {
        // init track obj
        TrackData newTrack{playlists->getPlayListName(), "", "", "", ""};

        // get file name and extension
        newTrack.trackTitle = f.getFileNameWithoutExtension().toStdString();

        std::string fileExt = f.getFileExtension().toStdString();
        newTrack.type = fileExt.substr(1, fileExt.size()); // skip the dot in the file extension

        // get file path
        newTrack.fileLocation = f.getFullPathName().toStdString();

        // use the formatTime function from DJAudioplayer to format the audio length
        double seconds = reader->lengthInSamples / reader->sampleRate;

        newTrack.duration = DJAudioPlayer::formatTime(seconds);

        playlists->insertNewAudio(newTrack);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlaylistComponent::cellDoubleClicked(int rowNumber,
                                          int columnId,
                                          const MouseEvent &event)
{
    currentRowNumber = indxV[rowNumber];
    if (event.mods.isLeftButtonDown())
    {
        URL fileURL = getFileLocationURL(currentRowNumber);
        //by default send to deck1 if it is not playing any audio
        if (!deckGUI1->isPlayingAudio())
        {
            deckGUI1->sendURL(fileURL);
        }
        //else, send to deck2 if it is not playing any audio
        else if (!deckGUI2->isPlayingAudio())
        {
            deckGUI2->sendURL(fileURL);
        }
    }
    repaint();
}


URL PlaylistComponent::getFileLocationURL(int rowNumber)
{
    return URL{ File{currentPlaylist[rowNumber].fileLocation } };
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlaylistComponent::cellClicked(int rowNumber,
                                    int columnId,
                                    const MouseEvent &event)
{
    // assign rowNumber to a variable as showMenuAsync will somehow change it
    currentRowNumber = indxV[rowNumber];

    if (event.mods.isRightButtonDown())
    {
        PopupMenu subMenu;
        subMenu.addItem(1, "Deck 1");
        subMenu.addItem(2, "Deck 2");

        PopupMenu cellClickedMenu;
        cellClickedMenu.addSubMenu("Load to Deck", subMenu);
        cellClickedMenu.addItem(3, "Remove from playlist");
        cellClickedMenu.addItem(4, "Load File from directory");

        cellClickedMenu.showMenuAsync(PopupMenu::Options(),
                    [&](int result)
                    {
                        if (result == 1)
                        {
                            // send to deck 1
                            deckGUI1->sendURL(getFileLocationURL(currentRowNumber));
                        }
                        else if (result == 2)
                        {
                            // send to deck 2
                            deckGUI2->sendURL(getFileLocationURL(currentRowNumber));
                        }
                        else if (result == 3)
                        {
                            // remove from playlist
                            // get the rows selected and get the actual id from indxV
                            auto rows = tableComponent.getSelectedRows();
                            std::vector<int> actualRows;
                            for (auto i = 0; i < rows.size(); ++i)
                            {
                                actualRows.push_back(indxV[rows[i]]);
                            }
                            // sort by descending so that you remove from the back
                            std::sort(actualRows.begin(), actualRows.end(), std::greater<int>());
                            for (auto i = 0; i < rows.size(); ++i)
                            {
                                playlists->removeAudioFromPlaylist(currentPlaylist[actualRows[i]], actualRows[i]);
                                currentPlaylist = playlists->getPlaylist();
                            }

                            tableComponent.deselectAllRows();
                            indxV.resize(currentPlaylist.size());
                            std::iota(indxV.begin(), indxV.end(), 0);
                            tableComponent.getHeader().setSortColumnId(1, true);
                            tableComponent.updateContent();
                        }
                        else if (result == 4)
                        {
                            //load tracks from directory
                            if (fChooser.browseForMultipleFilesToOpen(nullptr))
                            {
                                for (auto i = 0; i < fChooser.getResults().size(); ++i)
                                {
                                    insertNewTracks(fChooser.getResults()[i]);
                                }
                                currentPlaylist = playlists->getPlaylist();

                                indxV.resize(currentPlaylist.size());
                                std::iota(indxV.begin(), indxV.end(), 0);

                                tableComponent.getHeader().setSortColumnId(1, true);
                                tableComponent.updateContent();
                            }
                        }
                    });
    }
}

void PlaylistComponent::backgroundClicked(const MouseEvent &e)
{
    // only works when there is playlist
    if (playlists->getPlayListName().empty())
        return;

    if (e.mods.isRightButtonDown())
    {
        PopupMenu backgroundClickedMenu;
        backgroundClickedMenu.addItem(1, "Load File from directory");

        backgroundClickedMenu.showMenuAsync(PopupMenu::Options(),
                    [&](int result)
                    {
                        if (result == 1)
                        {
                            //open file dialog
                            if (fChooser.browseForMultipleFilesToOpen(nullptr))
                            {
                                for (auto i = 0; i < fChooser.getResults().size(); ++i)
                                {
                                    insertNewTracks(fChooser.getResults()[i]);
                                }
                                currentPlaylist = playlists->getPlaylist();

                                indxV.resize(currentPlaylist.size());
                                std::iota(indxV.begin(), indxV.end(), 0);
                                tableComponent.getHeader().setSortColumnId(1, true);

                                tableComponent.updateContent();
                                tableComponent.setVisible(true);
                                repaint();
                            }
                        }
                    });
    }
}

void PlaylistComponent::mouseDown(const MouseEvent &event)
{
    // shift + left click to select multitple rows
    // left click to select only that row
    if (event.mods.isRightButtonDown() && !tableComponent.isVisible() && getNumRows() != 0)
    {
        PopupMenu mainMenu;
        mainMenu.addItem(1, "Load File from directory");

        mainMenu.showMenuAsync(
            PopupMenu::Options(),
            [&](int result)
            {
                if (result == 1)
                {
                    DBG("rowNumber " << currentRowNumber);
                    DBG("load track to playlist");
                    if (fChooser.browseForMultipleFilesToOpen(nullptr))
                    {
                        for (auto i = 0; i < fChooser.getResults().size(); ++i)
                        {
                            insertNewTracks(fChooser.getResults()[i]);
                        }
                        currentPlaylist = playlists->getPlaylist();

                        indxV.resize(currentPlaylist.size());
                        std::iota(indxV.begin(), indxV.end(), 0);
                        tableComponent.getHeader().setSortColumnId(1, true);

                        tableComponent.updateContent();
                        tableComponent.setVisible(true);
                        repaint();
                    }
                }
            });
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlaylistComponent::updatePlaylist()
{
    currentPlaylist = playlists->getPlaylist();
    indxV.resize(currentPlaylist.size());
    std::iota(indxV.begin(), indxV.end(), 0);
    tableComponent.updateContent();
    tableComponent.setVisible(true);

    if (getNumRows() == 0 && playlists->getPlayListName().empty())
    {
        tableComponent.setVisible(false);
        createPlaylistBtn.setVisible(true);
    }
    if (getNumRows() != 0 && (currentPlaylist[0].playListName.empty() || currentPlaylist[0].trackTitle.empty()))
        tableComponent.setVisible(false);

    repaint();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlaylistComponent::sortOrderChanged(int newSortColumnId,
                                         bool isForwards)
{
    // sort by element index
    if (newSortColumnId == 1 && !isForwards)
    {
        //descending order
        std::sort(indxV.begin(), indxV.end(), std::greater<int>());
    }
    else if (newSortColumnId == 1 && isForwards)
    {
        //ascending order
        std::sort(indxV.begin(), indxV.end(), std::less<int>());
    }

    // sort by song title
    else if (newSortColumnId == 2 && !isForwards)
    {
        // use lambda to sort the indxV according to values from currentplaylist. same formula applies to the others
        std::sort(indxV.begin(), indxV.end(), [&](int i, int j)
                  { return currentPlaylist[i].trackTitle > currentPlaylist[j].trackTitle; });
    }
    else if (newSortColumnId == 2 && isForwards)
    {
        std::sort(indxV.begin(), indxV.end(), [&](int i, int j)
                  { return currentPlaylist[i].trackTitle < currentPlaylist[j].trackTitle; });
    }

    // sort by duration
    else if (newSortColumnId == 3 && !isForwards)
    {
        std::sort(indxV.begin(), indxV.end(), [&](int i, int j)
                  { return currentPlaylist[i].duration > currentPlaylist[j].duration; });
    }
    else if (newSortColumnId == 3 && isForwards)
    {
        std::sort(indxV.begin(), indxV.end(), [&](int i, int j)
                  { return currentPlaylist[i].duration < currentPlaylist[j].duration; });
    }

    // sort by file type
    else if (newSortColumnId == 4 && !isForwards)
    {
        std::sort(indxV.begin(), indxV.end(), [&](int i, int j)
                  { return currentPlaylist[i].type > currentPlaylist[j].type; });
    }
    else if (newSortColumnId == 4 && isForwards)
    {
        std::sort(indxV.begin(), indxV.end(), [&](int i, int j)
                  { return currentPlaylist[i].type < currentPlaylist[j].type; });
    }

    // sort by file location
    else if (newSortColumnId == 5 && !isForwards)
    {
        std::sort(indxV.begin(), indxV.end(), [&](int i, int j)
                  { return currentPlaylist[i].fileLocation > currentPlaylist[j].fileLocation; });
    }
    else if (newSortColumnId == 5 && isForwards)
    {
        std::sort(indxV.begin(), indxV.end(), [&](int i, int j)
                  { return currentPlaylist[i].fileLocation < currentPlaylist[j].fileLocation; });
    }

    tableComponent.updateContent();
    repaint();
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void PlaylistComponent::setSearchBarText(std::string text)
{
    if (text.empty())
    {
        //reset the index positions
        indxV.resize(currentPlaylist.size());
        std::iota(indxV.begin(), indxV.end(), 0);
        tableComponent.updateContent();
        return;
    }
   
    indxV.clear();
   
    for (auto i = 0; i < currentPlaylist.size(); ++i)
    {
        // push the index of TrackData that contains the substr of the text in the trackTitle
        if (currentPlaylist[i].trackTitle.find(text) != std::string::npos)
        {
            indxV.push_back(i);
        }
    }

    tableComponent.updateContent();
    repaint();
}
