/*
  ==============================================================================

    PlaylistComponent.h
    Created: 3 Feb 2023 1:37:18pm
    Author:  Barnabas

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioPlaylists.h"
#include "DeckGUI.h"
#include <vector>
#include <string>
//==============================================================================
/*
 */
class PlaylistComponent : public Component,
                          public TableListBoxModel,
                          public Button::Listener,
                          public FileDragAndDropTarget,
                          public DragAndDropContainer,
                          public DragAndDropTarget,
                          public TextEditor::Listener

{
public:
    //  PlaylistComponent(DeckGUI* deckGUI1, DeckGUI* deckGUI2, std::vector<AudioFilesData>* playlists, std::string* currentPlaylistName, AudioFormatManager& formatManager);
    PlaylistComponent(DeckGUI *deckGUI1, DeckGUI *deckGUI2, AudioPlaylists *playlists, AudioFormatManager &formatManager);
    ~PlaylistComponent();

    void paint(Graphics &) override;
    void resized() override;
    int getNumRows();

    void paintRowBackground(Graphics &,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected);

    void paintCell(Graphics &,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected);

    Component *refreshComponentForCell(int rowNumber,
                                       int columnId,
                                       bool isRowSelected,
                                       Component *existingComponentToUpdate);
    /*FileDragAndDropTarget functions*/
    bool isInterestedInFileDrag(const StringArray &files);
    void filesDropped(const StringArray &files, int x, int y);

    /*DragAndDropTarget functions*/
    bool isInterestedInDragSource(const SourceDetails &dragSourceDetails);
    void itemDropped(const SourceDetails &dragSourceDetails);

    /* DragAndDropContainer functions */
    var getDragSourceDescription(const SparseSet<int> &currentlySelectedRows) override;
    void mouseDown(const MouseEvent &) override;
    void mouseDrag(const MouseEvent &) override;

    /* sort rows by column*/
    void sortOrderChanged(int newSortColumnId,
                          bool isForwards) override;

    void insertNewTracks(File);

    void updatePlaylist();

    URL getFileLocationURL(int rowNumber);
    void setSearchBarText(std::string);

    /* Create playlist hyperlink listener */
    void buttonClicked(Button *button);

    /*TableComponent listeners*/
    void cellDoubleClicked(int rowNumber,
                           int columnId,
                           const MouseEvent &event);
    void cellClicked(int rowNumber,
                     int columnId,
                     const MouseEvent &event);
    void backgroundClicked(const MouseEvent &) override;

private:
    AudioFormatManager &formatManager;

    AudioPlaylists *playlists;
    DeckGUI *deckGUI1;
    DeckGUI *deckGUI2;

    TableListBox tableComponent;

    FileChooser fChooser{"Select a File"};

    std::vector<TrackData> currentPlaylist;

    int currentRowNumber = 0;
    std::vector<int> indxV; // store index of tracks to sort without modifying the original

    HyperlinkButton createPlaylistBtn;
    ArrowButton arrowBtn{"", 0.5, Colour::fromRGBA(194, 179, 165, 200)};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};