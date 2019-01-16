#include "HiddenLayer.h"

#define A 0.25
#define L 0.1

HiddenLayer::HiddenLayer(string activation, int inputSize, int neurons, vector<float> valuesRange)
{
	this->activation = activation;
	this->weights = Matrix();
	this->biases = Matrix();
	this->inputSize = inputSize;
	this->neurons = neurons;
	this->valuesRange = valuesRange;
}

HiddenLayer::~HiddenLayer()
{
}

void HiddenLayer::InitializeWeights(int numberOfSamples) {

	vector<float> temp = GenerateRandomVector(numberOfSamples);
	host_vector<float> weightsValues = temp;
	weights = Matrix(numberOfSamples, neurons, weightsValues);
}

void HiddenLayer::InitializeBiases(int numberOfSamples) {

	vector<float> temp = GenerateRandomVector_Repetitives(numberOfSamples);
	host_vector<float> biasesValues = temp;
	biases = Matrix(numberOfSamples, neurons, biasesValues);
}

vector<float> HiddenLayer::GenerateRandomVector(int numberOfSamples)
{
	float min = valuesRange[0];
	float max = valuesRange[1];

	int numberOfElements = numberOfSamples * neurons;
	vector<float> values;

	for (int i = 0; i < numberOfElements; i++)
	{
		values.push_back(min + static_cast<float> (rand()) / (static_cast<float> (RAND_MAX/(max-min))));
	}

	return values;
}
vector<float> HiddenLayer::GenerateRandomVector_Repetitives(int numberOfSamples)
{
	float min = valuesRange[0];
	float max = valuesRange[1];

	int numberOfElements = numberOfSamples * neurons;
	vector<float> values;

	for (int i = 0; i < neurons; i++)
	{
		values.push_back(min + static_cast<float> (rand()) / (static_cast<float> (RAND_MAX / (max - min))));
	}
	int counter = 0;
	for (int i = neurons; i < numberOfElements ; i++)
	{
		counter++;

		if (counter % neurons == 0 || i==neurons) 
			counter = 0;
		

		values.push_back(values[counter]);
		
		
	}

	return values;
}


Matrix HiddenLayer::GetWeights()
{
	return weights;
}

void HiddenLayer::UpdateWeights(Matrix matrix)
{
	weights = matrix;
}

Matrix HiddenLayer::GetBiases()
{
	return biases;
}

void HiddenLayer::UpdateBiases(Matrix matrix)
{
	biases = matrix;
}

Matrix HiddenLayer::ApplyActivation(Matrix hiddenLayerInput)
{
	Matrix hiddenLayerActivations = hiddenLayerInput;
	host_vector<float> temp = hiddenLayerInput.GetAllMatrixValues();

	if (activation == "sigmoid")
	{
		for (int i = 0; i < temp.size(); i++)
		{
			//cout << temp[i]<<"--%%--\n";
			temp[i] =float( 1.0 / float(1.0 + exp(-temp[i])));
			//cout << " " << temp[i];
		}

		hiddenLayerActivations.ChangeAllValuesInMatrix(temp);
	}
	else if(activation == "binstep")
	{
		for (int i = 0; i < temp.size(); i++)
		{
			if (temp[i] > 0)
			{
				temp[i] = 1;
			}
			else
			{
				temp[i] = 0;
			}
		}
		hiddenLayerActivations.ChangeAllValuesInMatrix(temp);
	}
	else if (activation == "purelin")
	{
		for (int i = 0; i < temp.size(); i++)
		{
			temp[i] = A * temp[i];
		}
		hiddenLayerActivations.ChangeAllValuesInMatrix(temp);
	}
	else if (activation == "tanh")
	{
		for (int i = 0; i < temp.size(); i++)
		{
			temp[i] = tanh(temp[i]);
		}
		hiddenLayerActivations.ChangeAllValuesInMatrix(temp);
	}
	else if (activation == "relu")
	{
		for (int i = 0; i < temp.size(); i++)
		{
			temp[i] = (0 < temp[i]) ? temp[i] : 0;
		}
		hiddenLayerActivations.ChangeAllValuesInMatrix(temp);
	}
	else if (activation == "lerelu")
	{
		for (int i = 0; i < temp.size(); i++)
		{
			if (temp[i] < 0) temp[i] = L * temp[i];
			else temp[i] = temp[i];
		}
		hiddenLayerActivations.ChangeAllValuesInMatrix(temp);
	}

	return hiddenLayerActivations;
}
Matrix HiddenLayer::ApplyDerivative(Matrix hiddenLayerActivation)
{
	Matrix slope = hiddenLayerActivation;
	host_vector<float> temp = hiddenLayerActivation.GetAllMatrixValues();

	if (activation == "sigmoid")
	{
		for (int i = 0; i < temp.size(); i++)
		{
			temp[i] = exp(-temp[i]) / pow((1 + exp(-temp[i])),2) ;
			//temp[i] = temp[i] * (1 - temp[i]);
		}

		slope.ChangeAllValuesInMatrix(temp);
	}
	else if(activation == "binstep")
	{
		for (int i = 0; i < temp.size(); i++)
		{
			temp[i] = 0;
		}
		slope.ChangeAllValuesInMatrix(temp);
	}
	else if (activation == "purelin")
	{
		for (int i = 0; i < temp.size(); i++)
		{
			temp[i] = A;
		}
		slope.ChangeAllValuesInMatrix(temp);
	}
	else if (activation == "tanh")
	{
		for (int i = 0; i < temp.size(); i++)
		{
			temp[i] = 1 - tanh(temp[i]) * tanh(temp[i]);
		}
		slope.ChangeAllValuesInMatrix(temp);
	}
	else if (activation == "relu")
	{
		for (int i = 0; i < temp.size(); i++)
		{
			if (temp[i] >= 0) temp[i] = 1;
			else temp[i] = 0;
		}
		slope.ChangeAllValuesInMatrix(temp);
	}
	else if (activation == "lerelu")
	{
		for (int i = 0; i < temp.size(); i++)
		{
			if (temp[i] >= 0) temp[i] = 1;
			else temp[i] = L;
		}
		slope.ChangeAllValuesInMatrix(temp);
	}

	return slope;
}
int HiddenLayer::GetNeurons()
{
	return neurons;
}