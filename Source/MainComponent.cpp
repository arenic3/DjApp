#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (1200, 700);
    setBufferedToImage(true);
    setOpaque(true);
    
    logo = juce::ImageFileFormat::loadFrom(BinaryData::logo2_png, BinaryData::logo2_pngSize);

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
    deck1.setLookAndFeel(&ccomp);
    addAndMakeVisible(deck1);
    
    //Deck2
    deck2.setLookAndFeel(&ccomp2);
    addAndMakeVisible(deck2);
    
    //Scrolling waveform
    addAndMakeVisible(wave1);
    addAndMakeVisible(wave2);
    
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
    
    xfadeSlider.setLookAndFeel(&customXFadeSlider);
    xfadeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
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
    ccomp.paint(g, deck1);
    ccomp2.paint(g, deck2);
    
    g.setColour(juce::Colour(100, 100, 100).withAlpha(0.8f));
    g.fillRect(getWidth()/3, getHeight()/3.16, getWidth()/3, getHeight()/3);
    
    g.setColour(juce::Colour(55, 55, 55));
    g.fillRect(getWidth()/3, getHeight()/1.731, getWidth()/3, getHeight()/12);
    
    g.setColour(juce::Colour(120, 120, 120));
    g.drawRect(getWidth()/3, getHeight()/1.731, getWidth()/3, getHeight()/12+1);
    
    g.drawImage(logo, getWidth()/2.75, getHeight()/3.15, getWidth()/3.75, getHeight()/4.5, 0, 0, logo.getWidth(), logo.getHeight());
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    deck1.setBounds(0, getHeight()/3.16, getWidth()/3, getHeight()/2.9+1);  //Deck1
    deck2.setBounds(getWidth()/1.5, getHeight()/3.16, getWidth()/3, getHeight()/2.9+1); //Deck2
    wave1.setBounds(0, 0, getWidth(), getHeight()/6);
    wave2.setBounds(0, (getHeight()/7)+5, getWidth(), getHeight()/6);
    xfadeSlider.setBounds(getWidth()/3+20, getHeight()/1.7, getWidth()/3-40, getHeight()/12);
    fileManager.setBounds(0, getHeight()/1.509, getWidth(), getHeight()/1.48);
}
