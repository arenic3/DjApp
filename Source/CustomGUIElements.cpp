/*
  ==============================================================================

    CustomGUIElements.cpp
    Created: 20 Feb 2025 12:50:09pm
    Author:  Nicolas Arellano

  ==============================================================================
*/

#include "CustomGUIElements.h"

CustomGUIDial::CustomGUIDial()
{
    dialImg = juce::ImageFileFormat::loadFrom(BinaryData::knob2_png, BinaryData::knob2_pngSize);
}

void CustomGUIDial::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    if(dialImg.isValid()){
        const double rotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum());
        
        const int frames = dialImg.getHeight() / dialImg.getWidth();
        const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
        const float radius = juce::jmin(width / 2.0f, height / 2.0f);
        const float centerX = x + width * 0.5f;
        const float centerY = y + height * 0.5f;
        const float rx = centerX - radius - 1.0f;
        const float ry = centerY - radius;
        
        g.drawImage(dialImg, (int)rx, (int)ry, 2*(int)radius, 2*(int)radius, 0, frameId*dialImg.getWidth(), dialImg.getWidth(), dialImg.getWidth());
    } else {
        static const float textPpercent = 0.35f;
        juce::Rectangle<float> text_bounds(1.0f + width * (1.0f - textPpercent) / 2.0f, 0.5f * height, width * textPpercent, 0.5f * height);
        g.setColour(juce::Colours::white);
        g.drawFittedText(juce::String("No image"), text_bounds.getSmallestIntegerContainer(), juce::Justification::horizontallyCentred | juce::Justification::centred, 1);
    }
}

CustomGUISlider::CustomGUISlider() 
{
    speedSliderImg = juce::ImageFileFormat::loadFrom(BinaryData::slider3_png, BinaryData::slider3_pngSize);
}

void CustomGUISlider::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle, juce::Slider& slider)
{
    if(speedSliderImg.isValid()){
        const double position = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum());
        
        const int frames = 100;
        const int frameId = (int)ceil(position * ((double)frames - 1.0));

        g.drawImage(speedSliderImg, 0, 0, (int)width, (int)height, 0, frameId*(speedSliderImg.getHeight()/frames), speedSliderImg.getWidth(), speedSliderImg.getHeight()/frames);
    } else {
        static const float textPpercent = 0.35f;
        juce::Rectangle<float> text_bounds(1.0f + width * (1.0f - textPpercent) / 2.0f, 0.5f * height, width * textPpercent, 0.5f * height);
        g.setColour(juce::Colours::white);
        g.drawFittedText(juce::String("No image"), text_bounds.getSmallestIntegerContainer(), juce::Justification::horizontallyCentred | juce::Justification::centred, 1);
    }
}

CustomGUIButton::CustomGUIButton()
{
    //playButtonImg = juce::ImageFileFormat::loadFrom(BinaryData::play_png, BinaryData::play_pngSize);
}

void CustomGUIButton::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& c, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto cornerSize = 0.0f;
    auto bounds = button.getLocalBounds().toFloat();
    
    juce::Colour color = juce::Colour(220, 150, 50);
    
    if(shouldDrawButtonAsHighlighted)
    {
        color = color.contrasting(juce::Colours::orange, 0.01f);
    } else if (shouldDrawButtonAsDown)
    {
        color = color.contrasting(juce::Colours::orangered, 0.1f);
    }
    
    g.setColour(color);
    g.fillRoundedRectangle(bounds, cornerSize);
    
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
}

CustomGUITable::CustomGUITable()
{
    
}

void CustomGUITable::drawTableHeaderBackground(juce::Graphics& g, juce::TableHeaderComponent& header)
{
    auto bounds = header.getLocalBounds().toFloat();
    auto cornerSize = 10.0f;
    
    g.setColour(juce::Colour(230, 150, 70));
    g.fillRoundedRectangle(bounds, cornerSize);
    
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    //g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
}
