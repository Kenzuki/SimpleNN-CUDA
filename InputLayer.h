#pragma once
#include "HiddenLayer.h"
class InputLayer
{
public:
	InputLayer();
	InputLayer(Matrix X);
	~InputLayer();
	int GetNumberOfSamples();
	Matrix GetInputs();
private:
	Matrix input;
	int samples;
};

