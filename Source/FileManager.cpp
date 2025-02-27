/*
  ==============================================================================

    FileManager.cpp
    Created: 27 Feb 2025 10:51:40am
    Author:  Nicolas Arellano

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FileManager.h"

//==============================================================================
FileManager::FileManager(DJAudioPlayer& player1, DJAudioPlayer& player2) : djAudioPlayer1(player1), djAudioPlayer2(player2)
{
    addAndMakeVisible(addButton);
    addAndMakeVisible(removeButton);
    addAndMakeVisible(deck1Button);
    addAndMakeVisible(deck2Button);
    
    addButton.addListener(this);
    removeButton.addListener(this);
    deck1Button.addListener(this);
    deck2Button.addListener(this);
    
    addAndMakeVisible(table);
    table.setModel(this);
    table.setOpaque(false);
    table.getViewport()->setOpaque(false);
    table.getHeader().setOpaque(true);
    
    table.getHeader().addColumn("File Name", 1, 200);
    table.getHeader().addColumn("Duration", 2, 200);
    table.getHeader().addColumn("Load to Deck", 3, 200);
    table.getHeader().addColumn("Remove from Deck", 4, 200);
}

FileManager::~FileManager()
{
}

void FileManager::paint (juce::Graphics& g)
{
    table.setColour(table.backgroundColourId, juce::Colours::transparentWhite);
    //background = juce::ImageCache::getFromMemory(BinaryData::Bg2_jpeg, BinaryData::Bg2_jpegSize);
    //g.drawImageWithin(background, 0, getHeight()/2, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
}

void FileManager::resized()
{
    float height = getHeight()/8;
    
    addButton.setBounds(0, 3.5*height, getWidth()/4, height/2);
    removeButton.setBounds(getWidth()/4, 3.5*height, getWidth()/4, height/2);
    deck1Button.setBounds(getWidth()/2, 3.5*height, getWidth()/4, height/2);
    deck2Button.setBounds(getWidth()/1.33, 3.5*height, getWidth()/4, height/2);
    
    table.setBounds(0, 20, getWidth(), 3.2*height);
}

int FileManager::getNumRows()
{
    return (int)audioFiles.size();
}

void FileManager::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if(rowIsSelected)
    {
        g.fillAll(juce::Colours::orange.withAlpha(0.3f));
    } else if(rowNumber % 2)
    {
        g.fillAll(juce::Colour(0xff222222).withAlpha(0.3f));
    } else
    {
        g.fillAll(juce::Colour(0xff333333).withAlpha(0.3f));
    }
}

void FileManager::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    g.setColour(rowIsSelected ? juce::Colours::orangered : juce::Colours::white);
    
    if(rowNumber < audioFiles.size()){
        if(columnId == 1){
            g.drawText(audioFiles[rowNumber].getFileName(), 2, 0, width -4, height, juce::Justification::centredLeft);
        }
    }
}

juce::Component * FileManager::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component * existingComponentToUpdate)
{
    if(columnId == 3 && rowNumber < audioFiles.size())
    {
        if(existingComponentToUpdate == nullptr)
        {
            //return new DeckButtonComponent(*this, audioFiles[rowNumber], rowNumber);
        }
        return existingComponentToUpdate;
    }
    else if (columnId == 4 && rowNumber < audioFiles.size())
    {
        if(existingComponentToUpdate == nullptr)
        {
            //return new DeckButtonComponent(*this, audioFiles[rowNumber], rowNumber);
        }
        return existingComponentToUpdate;
    }
    return nullptr;
    
}

bool FileManager::isInterestedInFileDrag(const juce::StringArray& files)
{
    for(auto file : files)
    {
        if(juce::File(file).hasFileExtension("wav;mp3;aif;aiff"))
            return true;
    }
    return false;
}

void FileManager::filesDropped(const juce::StringArray& files, int x, int y)
{
    addFilesToList(files);
}

void FileManager::buttonClicked(juce::Button * button)
{
    if(button == &addButton)
    {
        juce::FileChooser chooser("select audio files...", {}, "*.wav;*.mp3;*.aif;*.aiff");
        
        if(chooser.browseForMultipleFilesToOpen())
        {
            auto files = chooser.getResults();
            juce::StringArray fileStrings;
            
            for(auto file : files)
            {
                fileStrings.add(file.getFullPathName());
            }
            addFilesToList(fileStrings);
        }
    }
    else if(button == &removeButton)
    {
        auto selectedRows = table.getSelectedRows();
        for(int i= selectedRows.size(); --i >= 0;)
        {
            audioFiles.erase(audioFiles.begin() + selectedRows[i]);
        }
        table.updateContent();
    }
    else if(button == &deck1Button){
        auto selectedRow = table.getSelectedRow();
        loadFileIntoDeck(audioFiles[selectedRow], 1);
    }
    else if(button == &deck2Button){
        auto selectedRow = table.getSelectedRow();
        loadFileIntoDeck(audioFiles[selectedRow], 2);
    }
}

void FileManager::loadFileIntoDeck(const juce::File& file, int deckIndex)
{
    if(deckIndex == 1)
    {
        djAudioPlayer1.loadURL(juce::URL(file));
    } else if(deckIndex == 2)
    {
        djAudioPlayer2.loadURL(juce::URL(file));
    }
}

void FileManager::addFilesToList(const juce::StringArray& files)
{
    for(auto file : files)
    {
        juce::File audioFile(file);
        if(audioFile.hasFileExtension("wav;mp3;aif;aiff"))
        {
            audioFiles.push_back(audioFile);
        }
    }
    table.updateContent();
 }
