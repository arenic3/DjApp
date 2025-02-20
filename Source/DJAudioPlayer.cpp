/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 12 Feb 2025 12:25:28pm
    Author:  Nicolas Arellano

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(){
    formatManager.registerBasicFormats();
}

DJAudioPlayer::~DJAudioPlayer(){
}

bool DJAudioPlayer::loadURL(const juce::URL& url){
    auto * reader = formatManager.createReaderFor(url.getLocalFile());
    if(reader){
        auto newSource = std::make_unique<AudioFormatReaderSource>(reader, true);
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource = std::move(newSource);
        return true;
    }
    return false;
}

void DJAudioPlayer::play(){
    transportSource.start();
}

void DJAudioPlayer::stop(){
    transportSource.stop();
}

void DJAudioPlayer::setPosition(double posInSecs){
    if(posInSecs < 0. || posInSecs > transportSource.getLengthInSeconds()){
        DBG("DJAudioPlayer::setPosition: warning: set position " << posInSecs << " greater than length " << transportSource.getLengthInSeconds());
        return;
    }
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setGain(double gain){
    transportSource.setGain(gain);
}

void DJAudioPlayer::setSpeed(double ratio){
    if(ratio <= 0.){
        DBG("DJAudioPlayer::setSpeed: ratio must be greater than 0");
    } else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate){
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill){
    if(!readerSource.get()){
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources(){
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::setPositionRelative(double pos){
    auto posInSecs = pos * transportSource.getLengthInSeconds();
    setPosition(posInSecs);
}
