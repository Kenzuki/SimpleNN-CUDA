#include "InputLayer.h"

InputLayer::InputLayer()
{
}

InputLayer::InputLayer(Matrix X)
{
	this->input = X;
	this->samples = X.GetColumns();
}

InputLayer::~InputLayer()
{
}

int InputLayer::GetNumberOfSamples()
{
	return samples;
}

Matrix InputLayer::GetInputs()
{
	return input;
}