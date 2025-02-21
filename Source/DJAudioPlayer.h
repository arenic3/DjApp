/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 12 Feb 2025 12:25:28pm
    Author:  Nicolas Arellano

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

class DJAudioPlayer : public AudioSource {
public:
    DJAudioPlayer();
    ~DJAudioPlayer();
    
    bool loadURL(const juce::URL& url);
    void play();
    void stop();
    void setPosition(double posInSecs);
    void setGain(double gain);
    void setSpeed(double ratio);
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void setPositionRelative(double pos);
    
    bool playing = false;
    
private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
