#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (1000, 700);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    //Deck1
    addAndMakeVisible(deck1);
    
    //Deck2
    addAndMakeVisible(deck2);
    
    //FileManager
    addAndMakeVisible(fileManager);
    
    //Format manager
    formatManager1.registerBasicFormats();
    
    //Crossfader
    xfadeSlider.onValueChange = [this](){
        float v = xfadeSlider.getValue();
        float gl = std::min(1.f, 2.f*(1.f-v));
        float gr = std::min(1.f, 2.f*v);
        
        player1.setMix(gl);
        player2.setMix(gr);
    };
    
    xfadeSlider.setRange(0, 1);
    xfadeSlider.setValue(0.5);
    xfadeSlider.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(xfadeSlider);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    //Add individual players to mixer and call prepareToPlay for each
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    //Audio playback for each player
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    //Shutdown mixer & players
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    //Set background
    background = juce::ImageCache::getFromMemory(BinaryData::bg2_png, BinaryData::bg2_pngSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    deck1.setBounds(0, getHeight()/4, getWidth()/2, getHeight()/3.5);  //Deck1
    deck2.setBounds(getWidth()/2, getHeight()/4, (getWidth()/2), getHeight()/3.5); //Deck2
    xfadeSlider.setBounds(0, getHeight()/2, getWidth(), getHeight()/12);
    fileManager.setBounds(0, getHeight()/1.8, getWidth(), getHeight()/1.8);
}
