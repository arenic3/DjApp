#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

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
    
    //Play Button
    addAndMakeVisible(playButton);
    playButton.setButtonText("Play");
    playButton.addListener(this);
    
    
    //Stop Button
    addAndMakeVisible(stopButton);
    stopButton.setButtonText("Stop");
    stopButton.addListener(this);
    
    //Load Button
    addAndMakeVisible(loadButton);
    loadButton.setButtonText("Load file");
    loadButton.addListener(this);
    
    //Gain Slider
    gainDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    addAndMakeVisible(gainDial);
    gainDial.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);    //Remove gain slider value box
    gainDial.addListener(this);
    gainDial.setRange(0, 1);
    gainDial.setValue(0.5);
    
    //Playback position slider
    addAndMakeVisible(posSlider);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);     //Remove text box from position slider
    posSlider.addListener(this);
    posSlider.setRange(0, 1);
    posSlider.setValue(0);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    //
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    //
    player1.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    //
    player1.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::grey);
    //Drawing code:
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    loadButton.setBounds(0, 0, getWidth()/9, getHeight()/7);    //Load button
    playButton.setBounds(getWidth()/9, 0, getWidth()/9, getHeight()/7); //Play Button
    stopButton.setBounds(getWidth()/4.5, 0, getWidth()/9, getHeight()/7); //Stop Button
    gainDial.setBounds(getWidth()/1.18, 1, getWidth()/9, getHeight()/6);  //Gain Slider
    posSlider.setBounds(getWidth()/2.6, 0, getWidth()/2.4, getHeight()/9);  //Position Slider
}

void MainComponent::buttonClicked(juce::Button * button)
{
    //Called when buttons are clicked
    
    if (&playButton == button) {
        //DBG("Maincomponent::buttonClicked: Play button clicked");
        playing = true;
        player1.play();
    } else if (&stopButton == button) {
        //DBG("Maincomponent::buttonClicked: Stop button clicked");
        playing = false;
        player1.stop();
    }
    
    if(&loadButton == button) {
        FileChooser chooser{"choose a file to play..."};
        if(chooser.browseForFileToOpen()){
            auto url = URL(chooser.getResult());
            player1.loadURL(url);
          
            /* Old URL code
            auto file = chooser.getResult();
            auto * reader = formatManager.createReaderFor(file);
            if(reader){
                auto newSource = std::make_unique<AudioFormatReaderSource>(reader, true);
                transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                readerSource = std::move(newSource);
            }//*/
        }
    }
}

void MainComponent::sliderValueChanged(juce::Slider * slider)
{
    //Called when sliders are changed
    
    if(&gainDial == slider) {
        DBG("Maincomponent::sliderValueChanged: slider changed" << gainDial.getValue());
        gain = gainDial.getValue();
        player1.setGain(gain);
    }
    
    if(&posSlider == slider) {
        DBG("Maincomponent::sliderValueChanged: slider changed" << posSlider.getValue());
        posi = posSlider.getValue();
        player1.setPositionRelative(posi);
    }
}
