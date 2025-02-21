/*
  ==============================================================================

    CustomGUIElements.h
    Created: 20 Feb 2025 12:50:09pm
    Author:  Nicolas Arellano

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomGUIDial : public juce::LookAndFeel_V4
{
public:
    CustomGUIDial();
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    
private:
    juce::Image dialImg;
};

class CustomGUISlider : public juce::LookAndFeel_V4
{
public:
    CustomGUISlider();
    
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle, juce::Slider& slider) override;
    
private:
    juce::Image speedSliderImg;
};

class CustomGUIButton : public juce::LookAndFeel_V4
{
public:
    CustomGUIButton();
    
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& c, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
private:
    juce::Image playButtonImg;
};




