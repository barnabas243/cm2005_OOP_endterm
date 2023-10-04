/*
  ==============================================================================

    FolderComponent.h
    Created: 10 Feb 2023 1:15:50pm
    Author:  Barnabas

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioPlaylists.h"
#include <map>
#include <vector>
//==============================================================================
/*
 */
class FolderComponent : public Component,
                        public TableListBoxModel,
                        public TextEditor::Listener

{
public:
  FolderComponent(AudioPlaylists *playlists);
  ~FolderComponent();

  void paint(Graphics &) override;
  void resized() override;
  int getNumRows() override;
  void paintRowBackground(Graphics &,
                          int rowNumber,
                          int width,
                          int height,
                          bool rowIsSelected) override;
  void paintCell(Graphics &,
                 int rowNumber,
                 int columnId,
                 int width,
                 int height,
                 bool rowIsSelected) override;

  void textEditorTextChanged(TextEditor &) override;
  std::string getSearchBarText();

  void mouseDown(const MouseEvent &) override;

  void newPlaylistAlertW();
  void renamePlaylistAlertW();

  void backgroundClicked(const MouseEvent &) override;
  void cellClicked(int rowNumber,
                   int columnId,
                   const MouseEvent &) override;

  void updatefolderTable();

private:
  TableListBox folderComponent;

  AlertWindow *createPlaylistW;
  AlertWindow *renamePlaylistW;

  TextEditor searchBar;
  std::string searchBarText;

  AudioPlaylists *playlists;
  std::vector<std::string> playListNames;

  int currentRowNumber;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FolderComponent)
};
