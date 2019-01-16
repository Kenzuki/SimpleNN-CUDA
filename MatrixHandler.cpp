
#include "Matrix.h"
#include <iostream>

using namespace std;

MatrixHandler::MatrixHandler()
{
}


MatrixHandler::~MatrixHandler()
{
}

int MatrixHandler::CheckIfThereAreTooManyValuesInMatrix(Matrix* matrix, host_vector<short int> values) {

	int numberOfValues = values.size();

	if (matrix->GetElementsCount() >= numberOfValues) {
		return OK;
	}
	else {
		return ERROR_MATRIX_OUT_OF_BOUNDS;
	}
}

int MatrixHandler::GetNoError() {
	return OK;
}