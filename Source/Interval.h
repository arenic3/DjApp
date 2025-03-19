/*
  ==============================================================================

    Interval.h
    Created: 19 Mar 2025 2:02:09pm
    Author:  Nicolas Arellano

  ==============================================================================
*/

#pragma once

template <class T>
class Interval{
public:
    typedef T value_type;
    const T& start() const { return mStart; }
    const T& end() const { return mEnd; }
    
    Interval& start(const T& v){
        mStart = v;
        return validate();
    }
    
    Interval& end(const T& v){
        mEnd = v;
        return validate();
    }
    
    Interval& validate(){
        if(mStart > mEnd){
            double temp = mStart;
            mStart = mEnd;
            mEnd = temp;
        }
        
        return *this;
    }
    
    float  length(){
        float length = mEnd - mStart;
        return length;
    }
    
    bool proper(){
        bool isProper;
        
        if(mStart < mEnd){
            isProper = true;
        } else {
            isProper = false;
        }
        
        return isProper;
    }
    
private:
    T mStart = T();
    T mEnd = T();
};
