#pragma once

#include <vector>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <thrust\host_vector.h>
#include <thrust\device_vector.h>

using namespace std;
using namespace thrust;

class Matrix;

class MatrixHandler
{
public:
	MatrixHandler();
	~MatrixHandler();

	int CheckIfThereAreTooManyValuesInMatrix(Matrix* matrix, host_vector<short int> values);
	int GetNoError();

private:
	enum errors
	{
		OK = 0,
		ERROR_MATRIX_OUT_OF_BOUNDS = 1
	};

};

