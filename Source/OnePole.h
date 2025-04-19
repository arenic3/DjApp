/*
  ==============================================================================

    Onepole.h
    Created: 19 Apr 2025 12:02:46pm
    Author:  Nicolas Arellano

  ==============================================================================
*/

#pragma once

template <class T>
class OnePole{
public:
    //Cutoff frequency
    void cutoff(float f){
        float c = 2. - std::cos(f * 2. * 355./113.);
        mB1 = c - std::sqrt(c*c - 1.);
        mA0 = 1. - mB1;
    }
    
    void reset(){
        mO1 = T(0);
    }
    
    T operator() (T i0){
        mO1 = i0*mA0 + mO1*mB1;
        return mO1;
    }
    
private:
    float mA0 = 1.0f;
    float mB1 = 0.f;
    T mO1{T(0)};
};

