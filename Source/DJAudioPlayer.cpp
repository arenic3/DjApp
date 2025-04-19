/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 12 Feb 2025 12:25:28pm
    Author:  Nicolas Arellano

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& afm) : formatManager(afm) { }

DJAudioPlayer::~DJAudioPlayer() { }
bool DJAudioPlayer::loadURL(const juce::URL& url){
    auto * reader = formatManager.createReaderFor(url.getLocalFile());
    if(reader){
        auto newSource = std::make_unique<AudioFormatReaderSource>(reader, true);
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource = std::move(newSource);
        lurl = url;
        isLoaded = true;
        filter1.reset();
        filter2.reset();
        return true;
    }
    isLoaded = false;
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
        return;
    }
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setGain(double gain){
    transportSource.setGain(gain);
}

void DJAudioPlayer::setSpeed(double ratio){
    if(ratio <= 0.){
    } else {
        resampleSource.setResamplingRatio(ratio);
    }
}

int DJAudioPlayer::getLength(juce::File& file)
{
    int len = 0;
    
    if(auto * reader = formatManager.createReaderFor(file)){
        len = reader->lengthInSamples / reader->sampleRate;
        std::make_unique<AudioFormatReaderSource>(reader, true);
    }
    return len;
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate){
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill){
    
    auto l = loopRegion.load();
    if(l.proper()){
        auto p = getPositionRelative();
        if(p < l.start()){
            setPositionRelative(l.start());
        } else if(p >= l.end()){
            setPositionRelative(l.start());
        }
    }
    
    if(!readerSource.get()){
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    resampleSource.getNextAudioBlock(bufferToFill);
    
    int numChannels = bufferToFill.buffer->getNumChannels();
    auto * chan0 = numChannels>0 ? bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample) : nullptr;
    auto * chan1 = numChannels>1 ? bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample) : nullptr;
    
    for(int i=0; i<bufferToFill.numSamples; ++i){
        if(chan0) chan0[i] = filter1(chan0[i]);
        if(chan1) chan1[i] = filter2(chan1[i]);
    }
}

void DJAudioPlayer::releaseResources(){
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::setPositionRelative(double pos){
    auto posInSecs = pos * transportSource.getLengthInSeconds();
    setPosition(posInSecs);
}

double DJAudioPlayer::getPosition() const {
    return transportSource.getCurrentPosition();
}

double DJAudioPlayer::getPositionRelative() const {
    auto len = transportSource.getLengthInSeconds();
    if(len > 0.) return getPosition() / len;
    return 0.;
}

void DJAudioPlayer::setCutoff(float f){
    filter1.cutoff(f);
    filter2.cutoff(f);
}

void DJAudioPlayer::setMix(float v){
    mix = v;
    transportSource.setGain(gain * mix);
}

void DJAudioPlayer::setGain(float v){
    gain = v;
    transportSource.setGain(mix * gain);
}
