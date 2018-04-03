#include "ScalingCurve.h"


ScalingCurve::ScalingCurve()
{
    _isScalingOn = true;
    _minLookup = 0;
    _maxLookup = 0;
    _lookupRange = 0;
    _maxInput = 1;
    _curveLoaded = false;
}

bool ScalingCurve::loadCurve(std::string filename)
{
    std::fstream in;
    std::string line;
    in = std::fstream(filename);
    float valX, valY;
    while (std::getline(in, line)) {
        std::istringstream stream(line);
        stream >> valX >> valY;
        _curve.push_back(pair(valX, valY));
    }
    if (_curve.size() > 0) {
        _minLookup = _curve[0].x;
        _maxLookup = _curve.back().x;
        _lookupRange = _maxLookup - _minLookup;
        _curveLoaded = true;
        return true;
    } else {
        return false;
    }

}


bool ScalingCurve::loadCurve(std::vector<float> x, std::vector<float> y)
{
	if (x.size() != y.size()) {
		return false;
	}
	for (int i = 0; i < x.size(); i++) {
		_curve.push_back(pair(x[i], y[i]));
	}

	if (_curve.size() > 0) {
		_minLookup = _curve[0].x;
		_maxLookup = _curve.back().x;
		_lookupRange = _maxLookup - _minLookup;
		_curveLoaded = true;
		return true;
	}
	else {
		return false;
	}
}



void ScalingCurve::setInputMax(float max)
{
    _maxInput = max;
}

float ScalingCurve::getValue(float lookupVal, float in)
{
    if (_isScalingOn && _curveLoaded) {
        float scaler;
        if (lookupVal <= _minLookup) { 
            scaler =  _curve[0].y;
        } else if (lookupVal >= _maxLookup) { 
            scaler = _curve.back().y;
        } else {
            scaler = _lookup(lookupVal);
        }
        return scaler * (in / _maxInput);
    }
    return in;
}

float ScalingCurve::getInputValue(float lookupVal, float output)
{
	if (_isScalingOn && _curveLoaded) {
		float oldScaler = _lookup(lookupVal);
		return  output * _maxInput / oldScaler;
	}
	return output;
}


void ScalingCurve::enableScaling()
{
    // only allow to be turned on if curve is loaded
    if (_curveLoaded) {
        _isScalingOn = true;
    }
}

void ScalingCurve::disableScaling()
{
    _isScalingOn = false;
}

bool ScalingCurve::isScalingOn()
{
    return _isScalingOn;
}


float ScalingCurve::_lookup(float lookup)
{
	if (_curveLoaded) {
		if (_curve.size() > 128) {
			// for largish table, binary search and don't bother interpolating
			// binary search for the lookup val
			int min = 0, max = _curve.size();
			int cursor = min + ((max - min) / 2);
			while (min < max - 1 && _curve[cursor].x != lookup) {
				if (_curve[cursor].x < lookup) {
					min = cursor;
				}
				else {
					max = cursor;
				}
				if (_curve[cursor].x != lookup) {
					cursor = min + ((max - min) / 2);
				}
			}
			return _curve[cursor].y;
		}

		// 
		int cursor = 0;
		float y = 0;
		while (cursor < _curve.size() && _curve[cursor].x <= lookup) {
			cursor++;
		}
		if (cursor >= _curve.size()) { return _curve.back().y; }
		if (cursor == 0) { return _curve[0].y; }

		if (_curve[cursor].x == lookup) {
			return _curve[cursor].y;
		}
		// interpolate
		float error = lookup - _curve[cursor - 1].x;
		float errorScaled = error / (_curve[cursor].x - _curve[cursor - 1].x);
		float delta = _curve[cursor].y - _curve[cursor - 1].y;

		return _curve[cursor - 1].y + (errorScaled*delta);
	}
}


float ScalingCurve::_scaleValue(float in, float range)
{
    return range * (in / _maxInput);
}
