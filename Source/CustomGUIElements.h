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
    mutable juce::Image dialImg;
};

class CustomGUISlider : public juce::LookAndFeel_V4
{
public:
    CustomGUISlider();
    
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle, juce::Slider& slider) override;
    
private:
    mutable juce::Image verticalSliderImg;
    mutable juce::Image horizontalSliderImg;
};

class CustomGUIButton : public juce::LookAndFeel_V4
{
public:
    CustomGUIButton();
    
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& c, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};

class CustomPlayButton : public CustomGUIButton
{
public:
    CustomPlayButton();
    
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& c, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
private:
    CustomGUIButton b;
    mutable juce::Image playButtonImg;
};

class CustomStopButton : public CustomGUIButton
{
public:
    CustomStopButton();
    
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& c, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
private:
    CustomGUIButton b;
    mutable juce::Image stopButtonImg;
};


class CustomGUITable : public juce::LookAndFeel_V4
{
public:
    CustomGUITable();
    
    void drawTableHeaderBackground(juce::Graphics& g, juce::TableHeaderComponent& header) override;
    
private:
};

class CustomComponent : public juce::LookAndFeel_V4
{
public:
    CustomComponent();
    
    void paint(juce::Graphics& g, juce::Component& comp);
    
private:
};

class CustomProgressBar : public juce::LookAndFeel_V4
{
    public:
    CustomProgressBar();
    
    void drawLinearSlider(juce::Graphics& g,int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider& slider) override;
};
