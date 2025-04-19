/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 12 Feb 2025 12:25:28pm
    Author:  Nicolas Arellano

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Interval.h"
#include <atomic>
#include "OnePole.h"

using namespace juce;

class DJAudioPlayer : public AudioSource {
public:
    DJAudioPlayer(juce::AudioFormatManager& afm);
    ~DJAudioPlayer();
    
    bool loadURL(const juce::URL& url);
    void play();
    void stop();
    void setPosition(double posInSecs);
    void setGain(double gain);
    void setSpeed(double ratio);
    int getLength(juce::File& file);
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void setPositionRelative(double pos);
    double getPosition() const;
    double getPositionRelative() const;
    
    std::atomic<Interval<float>> loopRegion;
    
    void setCutoff(float f);
    void setMix(float v);
    void setGain(float v);
    
    
    float gain = 1.;
    float mix = 1.;
    
    bool isLoaded = false;
    juce::URL lurl;
    bool playing = false;
private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
    OnePole<float> filter1, filter2;
};
