#pragma once


/*! \class ScalingCurve
*  \brief Engine: Used to scale connections weights based on ratio of frequencies
*/


#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

class ScalingCurve
{
public:
    ScalingCurve();

    bool loadCurve(std::string filename);
	bool loadCurve(std::vector<float> x, std::vector<float> y);
    void setInputMax(float max);

    float getValue(float lookupVal, float in);
    float getInputValue(float lookupVal, float output);


    void enableScaling();
    void disableScaling();
    bool isScalingOn();

private:
    struct pair
    {
        float y;
        float x;
        pair(float xval, float yval)
        {
            x = xval;
            y = yval;
        }
        pair()
        {
            pair(0, 0);
        }
    }            ;

    std::vector<pair> _curve;
    float _minLookup;
    float _maxLookup;
    float _lookupRange;
    float _maxInput;
    bool _isScalingOn;
    bool _curveLoaded;



    float _lookup(float lookup);
    float _scaleValue(float in, float range);

};