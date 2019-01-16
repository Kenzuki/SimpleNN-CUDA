#include <iostream>
#include "Matrix.h"
using namespace std;

#pragma once
class Layer
{
public:
	Layer();
	Layer(string type, int inputSize, int neurons = 0);
	~Layer();
private:
	string type;
	int neurons;
};

