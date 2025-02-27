/*
  ==============================================================================

    FileManager.h
    Created: 27 Feb 2025 10:51:40am
    Author:  Nicolas Arellano

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class FileManager  : public juce::Component, public juce::TableListBoxModel, public juce::FileDragAndDropTarget, public juce::Button::Listener
{
public:
    FileManager(DJAudioPlayer& player1, DJAudioPlayer& player2);
    ~FileManager() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    juce::Component * refreshComponentForCell(int rowNumber, int ColumnId, bool isRowSelected, juce::Component * existingComponentToUpdate) override;
    
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    
    void buttonClicked(juce::Button * button) override;
    
    void loadFileIntoDeck(const juce::File& file, int deckIndex);
    void addFilesToList(const juce::StringArray& files);

    
private:
    juce::TableListBox table;
    std::vector<juce::File> audioFiles;
    DJAudioPlayer& djAudioPlayer1;
    DJAudioPlayer& djAudioPlayer2;
    juce::TextButton addButton;
    juce::TextButton deck1Button;
    juce::TextButton removeButton;
    juce::TextButton deck2Button;
    juce::Image background;
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileManager)
};
