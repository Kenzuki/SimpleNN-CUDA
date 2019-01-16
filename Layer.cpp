#include "Layer.h"

Layer::Layer()
{
}

Layer::Layer(string type, int inputSize, int neurons)
{
	this->type = type;
	this->neurons = neurons;
}

Layer::~Layer()
{
}

