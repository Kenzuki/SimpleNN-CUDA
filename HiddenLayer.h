#pragma once
#include "Matrix.h"
#include "Math.h"

class HiddenLayer
{
public:
	HiddenLayer(string activation, int inputSize, int neurons, vector<float> valuesRange);
	~HiddenLayer();
	void InitializeWeights(int numberOfSamples);
	void InitializeBiases(int numberOfSamples);
	Matrix GetWeights();
	void UpdateWeights(Matrix matrix);
	Matrix GetBiases();
	void UpdateBiases(Matrix matrix);
	Matrix ApplyActivation(Matrix hiddenLayerInput);
	Matrix ApplyDerivative(Matrix hiddenLayerActivations);
	int GetNeurons();
private:
	vector<float> valuesRange;
	Matrix weights;
	Matrix biases;
	string activation;
	int inputSize;
	int neurons;

	vector <float> GenerateRandomVector(int numberOfSamples);
	vector <float> GenerateRandomVector_Repetitives(int numberOfSamples);
};

