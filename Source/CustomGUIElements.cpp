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
    
    dialImg = juce::ImageFileFormat::loadFrom(BinaryData::bleh_png, BinaryData::bleh_pngSize);
}

void CustomGUIDial::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    if(dialImg.isValid()){
        const double rotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum());
        
        const int frames = 150;
        const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
        const float radius = juce::jmin(width / 2.0f, height / 2.0f);
        const float centerX = x + width * 0.5f;
        const float centerY = y + height * 0.5f;
        const float rx = centerX - radius - 1.0f;
        const float ry = centerY - radius;
        
        g.drawImage(dialImg, (int)rx, (int)ry, 2*(int)radius, 2*(int)radius, 0, frameId*dialImg.getWidth(), dialImg.getHeight()/frames, dialImg.getWidth());
    } else {
        static const float textPpercent = 0.35f;
        juce::Rectangle<float> text_bounds(1.0f + width * (1.0f - textPpercent) / 2.0f, 0.5f * height, width * textPpercent, 0.5f * height);
        g.setColour(juce::Colours::white);
        g.drawFittedText(juce::String("No image"), text_bounds.getSmallestIntegerContainer(), juce::Justification::horizontallyCentred | juce::Justification::centred, 1);
    }
}

CustomGUISlider::CustomGUISlider() 
{
    verticalSliderImg = juce::ImageFileFormat::loadFrom(BinaryData::fader_vertical_png, BinaryData::fader_vertical_pngSize);
    horizontalSliderImg = juce::ImageFileFormat::loadFrom(BinaryData::fader_horizontal_png, BinaryData::fader_horizontal_pngSize);
}

void CustomGUISlider::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle, juce::Slider& slider)
{
    if(verticalSliderImg.isValid() && horizontalSliderImg.isValid()){
        const double position = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum());
        
        const int frames = 150;
        const int frameId = (int)ceil(position * ((double)frames - 1.0));
        
        if(slider.isVertical()){
            g.drawImage(verticalSliderImg, 0, 0, (int)width, (int)height, 0, frameId*(verticalSliderImg.getHeight()/frames), verticalSliderImg.getWidth(), verticalSliderImg.getHeight()/frames);
        } else if(slider.isHorizontal()){
            g.drawImage(horizontalSliderImg, -25, 0, (int)width+75, (int)height, 0, frameId*(horizontalSliderImg.getHeight()/frames), horizontalSliderImg.getWidth(), horizontalSliderImg.getHeight()/frames);
        }
    } else {
        static const float textPpercent = 0.35f;
        juce::Rectangle<float> text_bounds(1.0f + width * (1.0f - textPpercent) / 2.0f, 0.5f * height, width * textPpercent, 0.5f * height);
        g.setColour(juce::Colours::white);
        g.drawFittedText(juce::String("No image"), text_bounds.getSmallestIntegerContainer(), juce::Justification::horizontallyCentred | juce::Justification::centred, 1);
    }
}

CustomGUIButton::CustomGUIButton()
{
    
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

CustomPlayButton::CustomPlayButton()
{
    playButtonImg = juce::ImageFileFormat::loadFrom(BinaryData::play_png, BinaryData::play_pngSize);
}

void CustomPlayButton::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& c, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    b.drawButtonBackground(g, button, c, false, false);
    
    if(shouldDrawButtonAsHighlighted){
        b.drawButtonBackground(g, button, c, true, false);
    }
    g.drawImage(playButtonImg, button.getWidth()/4, 0, button.getWidth()-button.getWidth()/2, button.getHeight(), 0, 0, playButtonImg.getWidth(), playButtonImg.getHeight());
}

CustomStopButton::CustomStopButton()
{
    stopButtonImg = juce::ImageFileFormat::loadFrom(BinaryData::stop_png, BinaryData::stop_pngSize);
}

void CustomStopButton::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& c, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    b.drawButtonBackground(g, button, c, false, false);
    
    if(shouldDrawButtonAsHighlighted){
        b.drawButtonBackground(g, button, c, true, false);
    }
    g.drawImage(stopButtonImg, button.getWidth()/5.5, 0, button.getWidth()-button.getWidth()/2.75, button.getHeight(), 0, 0, stopButtonImg.getWidth(), stopButtonImg.getHeight());
}

CustomGUITable::CustomGUITable()
{
    
}

void CustomGUITable::drawTableHeaderBackground(juce::Graphics& g, juce::TableHeaderComponent& header)
{
    auto bounds = header.getLocalBounds().toFloat();
    
    g.setColour(juce::Colour(230, 150, 70));
    g.fillRect(bounds);
    
    g.setColour(juce::Colours::black.withAlpha(0.5f));
}

CustomComponent::CustomComponent()
{
    
}

void CustomComponent::paint(juce::Graphics& g, juce::Component& comp)
{
    auto innerBounds = comp.getBounds().toFloat();
    g.setColour(juce::Colour(60, 60, 60));
    g.fillRect(innerBounds);
    
    g.setColour(juce::Colour(120, 120, 120).withAlpha(0.5f));
    g.drawRect(innerBounds);
};

CustomProgressBar::CustomProgressBar()
{
    
}

void CustomProgressBar::drawLinearSlider(juce::Graphics& g,int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider& slider)
{
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(x, y + height / 2 - 4, width, 8);

    g.setColour(juce::Colours::green);
    g.fillRect(x, y + height / 2 - 4, static_cast<int>(sliderPos - x), 8);

    g.setColour(juce::Colours::white);
    g.fillEllipse(sliderPos - 5, y + height / 2 - 5, 10, 10);
}
