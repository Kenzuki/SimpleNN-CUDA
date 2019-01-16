#include "NeuralNetwork.h"
#include <fstream>
#include <sstream> 
#include "stdafx.h"
//#include "stdafx.h"
//#include"api.h"
#define INITIAL_VALUE 1
#define kupa 0


NeuralNetwork::NeuralNetwork(vector<int> networkSize, vector<vector<float>> valuesRange, vector<string> activation)
{
	this->numberOfHiddenLayers = networkSize.size();
	this->networkSize = networkSize;
	this->valuesRange = valuesRange;
	this->activation = activation;
	epochs = 10;
	learningRate = 0.03;

	CreateHiddenLayers();
}

NeuralNetwork::~NeuralNetwork()
{
}

void NeuralNetwork::CreateHiddenLayers()
{
	for (int i = 0; i < numberOfHiddenLayers; i++)
	{
		hiddenLayers.push_back(HiddenLayer(activation[i], INITIAL_VALUE, networkSize[i], valuesRange[i]));
	}
}

void NeuralNetwork::CreateInputLayer(Matrix X) 
{
	input = InputLayer(X);
}

void NeuralNetwork::FitData(Matrix X) {
	CreateInputLayer(X);
	for (int i = 0; i < numberOfHiddenLayers; i++)
	{
		if(i == 0)
		{
			hiddenLayers[i].InitializeWeights(input.GetNumberOfSamples());
			hiddenLayers[i].InitializeBiases(input.GetNumberOfSamples());
		}
		else
		{
			hiddenLayers[i].InitializeWeights(hiddenLayers[i - 1].GetNeurons());
			hiddenLayers[i].InitializeBiases(hiddenLayers[i - 1].GetNeurons());
		}
	}
}

void NeuralNetwork::SetEpochs(int epochs)
{
	this->epochs = epochs;
}

void NeuralNetwork::Train(Matrix y) {

	for (iteracje = 0; iteracje < epochs; iteracje++)
	{
		//SendMessage(hProgressBar, (WM_USER + 2), (WPARAM)iteracje, 0);
		Matrix hiddenLayerInput = Matrix();
		vector<Matrix> hiddenLayerActivations;
		vector<Matrix> slope;
		vector<Matrix> error_hidden;
		vector<Matrix> delta_hidden;

		for (int k = 0; k < numberOfHiddenLayers; k++)
		{
		
			hiddenLayerActivations.push_back(Matrix());
			if (k == 0)
			{

				hiddenLayerInput = input.GetInputs() * hiddenLayers[k].GetWeights();//TU BYL

				cudaThreadSynchronize();
				hiddenLayerInput = hiddenLayerInput + hiddenLayers[k].GetBiases();

				cudaThreadSynchronize();
				hiddenLayerActivations[k] = hiddenLayers[k].ApplyActivation(hiddenLayerInput);//TU BYL
			
			}
			else
			{
				error_hidden.push_back(Matrix());
				delta_hidden.push_back(Matrix());

				cudaThreadSynchronize();
				hiddenLayerInput = hiddenLayerActivations[k-1] * hiddenLayers[k].GetWeights();
				cudaThreadSynchronize();
				hiddenLayerInput = hiddenLayerInput + hiddenLayers[k].GetBiases();		
				cudaThreadSynchronize();
				hiddenLayerActivations[k] = hiddenLayers[k].ApplyActivation(hiddenLayerInput);
			}
		}



		cudaThreadSynchronize();
		error = y - hiddenLayerActivations[numberOfHiddenLayers - 1];
		cout << "Iteracja:" << iteracje << endl;

	//	if (iteracje==0)
	//	{
	//		ofstream plik("wykres.txt");
	//		plik.close();
	//	}
		
		if (0)
		{
			ofstream plik("wykres.txt", std::ios::out | ios::app);
			host_vector<float> lel = error.GetAllMatrixValues();
			host_vector<float>  wyjscie=y.GetAllMatrixValues();
			int wart = 5000;
			int wielkosc = lel.size() / wart;
			float * naprawde = new float[wielkosc];
			for (int ssij = 0; ssij < wielkosc; ssij++)
				naprawde[ssij] = 0;
			
			int rly = 0;
			for (int z = 0; z < lel.size(); z++)
			{
				if (z % wart == 0 && z!=0)
					{
						naprawde[rly] = naprawde[rly] / wart;
						rly++;	
					}
				naprawde[rly] += lel[z];
				if (z == lel.size() - 1)
						naprawde[rly] = naprawde[rly] / wart;
					
			}

			string x;	//Wklejamy go do nowego pliku
			for(int z=0; z<int(lel.size() / wart);z++)
			{ 

			stringstream ss;
			string h;
			ss << naprawde[z];
			h= ss.str();
			x += h;
			x += ",";

			
			}
			x += "\n";

			plik.write(x.c_str(), x.length());


		}

		for (int k = 0; k < numberOfHiddenLayers; k++)
		{
			slope.push_back(Matrix());
			slope[k] = hiddenLayers[k].ApplyDerivative(hiddenLayerActivations[k]);

		}
		cudaThreadSynchronize();
		Matrix deltaOutput = (error % slope[numberOfHiddenLayers - 1]) * learningRate;


		int m = numberOfHiddenLayers - 1;
		cudaThreadSynchronize();
		for (int k = numberOfHiddenLayers - 1; k > 0; k--)
		{
			if (k == numberOfHiddenLayers - 1)
			{

				error_hidden[k - 1] = deltaOutput * hiddenLayers[k].GetWeights().TransposeMatrixGPU();
				cudaThreadSynchronize();
				//error_hidden[k - 1].PrintMatrix();
				//cout << endl;
				//cout << "Krok 9" << endl;
				delta_hidden[k - 1] = error_hidden[k - 1] % slope[m - 1];
				//delta_hidden[k - 1].PrintMatrix();
				//cout << endl;
			}
			else
			{
				error_hidden[k - 1] = delta_hidden[k] * hiddenLayers[k].GetWeights().TransposeMatrixGPU();
				cudaThreadSynchronize();
				delta_hidden[k - 1] = error_hidden[k - 1] % slope[m - 1];
			}
			m--;
		}
		cudaThreadSynchronize();
		//cout << "Krok 10 i 11" << endl;
		for (int k = numberOfHiddenLayers - 1; k >= 0; k--)
		{
			if (k == numberOfHiddenLayers - 1)
			{				
				hiddenLayers[k].UpdateWeights(hiddenLayers[k].GetWeights() + (hiddenLayerActivations[k - 1].TransposeMatrixGPU() * deltaOutput * learningRate));
				hiddenLayers[k].UpdateBiases(hiddenLayers[k].GetBiases() + (deltaOutput.SumAllElements() * learningRate));
			}
			else if(k != 0)
			{
				hiddenLayers[k].UpdateWeights(hiddenLayers[k].GetWeights() + (hiddenLayerActivations[k - 1].TransposeMatrixGPU() * delta_hidden[k] * learningRate));
				hiddenLayers[k].UpdateBiases(hiddenLayers[k].GetBiases() + (delta_hidden[k].SumAllElements() * learningRate));
			}
			else
			{
				hiddenLayers[k].UpdateWeights(hiddenLayers[k].GetWeights() + (input.GetInputs().TransposeMatrixGPU() * delta_hidden[k] * learningRate));
				hiddenLayers[k].UpdateBiases(hiddenLayers[k].GetBiases() + (delta_hidden[k].SumAllElements() * learningRate));
			}
		}
		//if (iteracje == epochs - 1)
	//	{
			//hiddenLayerActivations[1].PrintMatrix();
			//cout << endl;
	//	}
	}
}

Matrix NeuralNetwork::Predict(Matrix X) {
	CreateInputLayer(X);

	Matrix hiddenLayerInput = Matrix();
	vector<Matrix> hiddenLayerActivations;

	for (int k = 0; k < numberOfHiddenLayers; k++)
	{
		hiddenLayerActivations.push_back(Matrix());
		if (k == 0)
		{
			hiddenLayerInput = input.GetInputs() * hiddenLayers[k].GetWeights();
			hiddenLayerInput = hiddenLayerInput + hiddenLayers[k].GetBiases();
			//hiddenLayerInput.PrintMatrix();
			hiddenLayerActivations[k] = hiddenLayers[k].ApplyActivation(hiddenLayerInput);
//			hiddenLayerActivations[k].PrintMatrix();
		}
		else
		{
			hiddenLayerInput = hiddenLayerActivations[k - 1] * hiddenLayers[k].GetWeights();
			hiddenLayerInput = hiddenLayerInput + hiddenLayers[k].GetBiases();
			//hiddenLayerInput.PrintMatrix();
			hiddenLayerActivations[k] = hiddenLayers[k].ApplyActivation(hiddenLayerInput);
//			hiddenLayerActivations[k].PrintMatrix();
		}
	}

	Matrix output = hiddenLayerActivations[numberOfHiddenLayers - 1];

	return output;
}