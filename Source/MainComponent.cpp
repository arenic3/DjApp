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
    gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    addAndMakeVisible(gainSlider);
    gainSlider.addListener(this);
    gainSlider.setRange(0, 1);
    gainSlider.setValue(0.5);
    
    //Format manager
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    if(!playing){
        //Right now we are not producing any data, in which case we need to clear the buffer
        //(to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    // Your audio-processing code goes here!
    transportSource.getNextAudioBlock(bufferToFill);
    
    // For more details, see the help for AudioProcessor::getNextAudioBlock()
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    
    transportSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    
    
    playButton.setBounds(getWidth()/6, 0, getWidth()/3, getHeight()/7); //Play Button
    stopButton.setBounds(getWidth()/2, 0, getWidth()/3, getHeight()/7); //Stop Button
    loadButton.setBounds(0, 0, getWidth()/6, getHeight()/7);    //Load button
    gainSlider.setBounds(getWidth()/2.4, 1, getWidth(), getHeight()/6);  //Gain Slider
    gainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);    //Remove gain slider value box
}

void MainComponent::buttonClicked(juce::Button * button)
{
    //Called when buttons are clicked
    
    if (&playButton == button) {
        DBG("Maincomponent::buttonClicked: Play button clicked");
        playing = true;
        transportSource.setPosition(0);
        transportSource.start();
    } else if (&stopButton == button) {
        DBG("Maincomponent::buttonClicked: Stop button clicked");
        playing = false;
        transportSource.stop();
    }
    
    if(&loadButton == button) {
        FileChooser chooser{"choose a file to play..."};
        if(chooser.browseForFileToOpen()){
            auto file = chooser.getResult();
            auto * reader = formatManager.createReaderFor(file);
            if(reader){
                auto newSource = std::make_unique<AudioFormatReaderSource>(reader, true);
                transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                readerSource = std::move(newSource);
            }
        }
    }
}

void MainComponent::sliderValueChanged(juce::Slider * slider)
{
    //Called when sliders are changed
    
    if(&gainSlider == slider) {
        DBG("Maincomponent::sliderValueChanged: slider changed" << gainSlider.getValue());
        gain = gainSlider.getValue();
        transportSource.setGain(gain);
    }
}
