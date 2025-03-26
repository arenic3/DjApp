/*
  ==============================================================================
r
    FileManager.cpp
    Created: 27 Feb 2025 10:51:40am
    Author:  Nicolas Arellano

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FileManager.h"

//==============================================================================
FileManager::FileManager(DJAudioPlayer& player1, DJAudioPlayer& player2, DeckGUI& deck, DeckGUI& secondDeck) : djAudioPlayer1(player1), djAudioPlayer2(player2), deck1(deck), deck2(secondDeck)
{
    //add all buttons to component
    addButton.setLookAndFeel(&customButtons);
    addAndMakeVisible(addButton);
    addButton.setButtonText("Browse Files");
    removeButton.setLookAndFeel(&customButtons);
    addAndMakeVisible(removeButton);
    removeButton.setButtonText("Remove File");
    deck1Button.setLookAndFeel(&customButtons);
    addAndMakeVisible(deck1Button);
    deck1Button.setButtonText("Load to Deck 1");
    deck2Button.setLookAndFeel(&customButtons);
    addAndMakeVisible(deck2Button);
    deck2Button.setButtonText("Load to Deck 2");
    
    //Add button listeners to component
    addButton.addListener(this);
    removeButton.addListener(this);
    deck1Button.addListener(this);
    deck2Button.addListener(this);
    
    //Add and set up table
    table.getHeader().setLookAndFeel(&customTable);
    addAndMakeVisible(table);
    table.setModel(this);
    table.setOpaque(false);
    table.getViewport()->setOpaque(false);
    
    //Set table columns
    table.getHeader().addColumn("File Name", 1, 270);
    table.getHeader().addColumn("Song Name", 2, 280);
    table.getHeader().addColumn("Artist", 3, 270);
    table.getHeader().addColumn("Duration", 4, 140);
}

FileManager::~FileManager()
{
}

void FileManager::paint (juce::Graphics& g)
{
    //Add component border & shadow
    //float cornerSize = 10.0f;
    auto innerBounds = getLocalBounds().toFloat().reduced(10.0f);
    auto outerBounds = getLocalBounds().toFloat().reduced(8.0f);
    
    
    path.addRectangle(outerBounds);
    
    shadow = juce::DropShadow(juce::Colours::black.withAlpha(0.5f), 10, juce::Point<int> (0, 0));
    shadow.drawForPath (g, path);
    
    g.setColour(juce::Colours::lightgrey.withAlpha(0.5f));
    g.fillRect(innerBounds);
    
    g.setColour(juce::Colours::orange.withAlpha(0.5f));
    g.drawRect(innerBounds);
    
    table.setColour(table.backgroundColourId, juce::Colours::orange.withAlpha(0.2f));
}

void FileManager::resized()
{
    float height = getHeight()/8;
    
    table.setBounds(10, 10, getWidth()-20, getHeight()/1.4);
    
    addButton.setBounds(0, getHeight()/1.35, getWidth()/4, height/1.9);
    removeButton.setBounds(getWidth()/4, getHeight()/1.35, getWidth()/4, height/1.9);
    deck1Button.setBounds(getWidth()/2, getHeight()/1.35, getWidth()/4, height/1.9);
    deck2Button.setBounds(getWidth()/1.333, getHeight()/1.35, getWidth()/4, height/1.9);
}

int FileManager::getNumRows()
{
    //size of audioFiles vector
    return (int)audioFiles.size();
}

void FileManager::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    //Paint alternating rows in diff colors -> orange & grey
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
    //Populate rows with file data
    g.setColour(rowIsSelected ? juce::Colours::orangered : juce::Colours::white);
    
    if(rowNumber < audioFiles.size()){
        if(columnId == 1){
            g.drawText(audioFiles[rowNumber].getFileName(), 2, 0, width -4, height, juce::Justification::centredLeft);
        }
    }
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
        deck1.loadWaveform();
    }
    else if(button == &deck2Button){
        auto selectedRow = table.getSelectedRow();
        loadFileIntoDeck(audioFiles[selectedRow], 2);
        deck2.loadWaveform();
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
