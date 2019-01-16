
#include "InputLayer.h"
extern int iteracje;
//extern HWND  hProgressBar;
class NeuralNetwork
{
public:
	NeuralNetwork(vector<int> networkSize, vector<vector<float>> valuesRange, vector<string> activation);
	~NeuralNetwork();
	void FitData(Matrix X);
	void Train(Matrix y);
	void SetEpochs(int epochs);
	Matrix Predict(Matrix X);
	
private:
	int numberOfHiddenLayers;
	int epochs;
	float learningRate;
	vector<int> networkSize;
	vector<vector<float>> valuesRange;
	vector<string> activation;
	vector<HiddenLayer> hiddenLayers;
	InputLayer input;
	Matrix error;

	void CreateHiddenLayers();
	void CreateInputLayer(Matrix X);

};

