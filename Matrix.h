#pragma once

#include "MatrixHandler.h""

class MatrixHandler;

__global__ void cudaMultiplicationMatrix(float* inMatrixA, float* inMatrixB, float* outMatrix, int rowsA, int columnsA, int rowsB, int columnsB);
__global__ void cudaMultiplicationScalar(float* inMatrixValues, float* outMatrixValues, float value, int matrixRows, int matrixColumns);
__global__ void cudaTranspose(float* inMatrixValues, float* outMatrixValues, int rows, int columns);
__global__ void cudaTransposeShared(float* inMatrixValues, float* outMatrixValues, int rows, int columns);
__global__ void cudaAddition(float* inMatrixA, float* inMatrixB, float* outMatrix, int rows, int columns);
__global__ void cudaSubtraction(float* inMatrixA, float* inMatrixB, float* outMatrix, int rows, int columns);
__global__ void cudaSumAll(float* matrix, int rows, int columns, float* outValue);
__global__ void cudaAdditionScalar(float* inMatrixValues, float* outMatrixValues, float value, int matrixRows, int matrixColumns);
__global__ void cudaMultiplication(float* inMatrixA, float* inMatrixB, float* outMatrix, int rows, int columns);
__global__ void cudaMultiplicationMatrix2(float* inMatrixA, float* inMatrixB, float* outMatrix, int rowsA, int columnsA, int rowsB, int columnsB);

class Matrix
{
public:
	Matrix(int size, host_vector<float> values);
	Matrix(int rows = 1, int columns = 1);
	Matrix(int rows, int columns, host_vector<float> &values);
	~Matrix();

#pragma region Opertatory
	void operator=(Matrix matrix);
	Matrix operator*(float value);
	Matrix operator*(Matrix matrix);
	Matrix operator%(Matrix matrix);
	Matrix operator+(Matrix matrix);
	Matrix operator+(float value);
	Matrix operator-(Matrix matrix);
#pragma endregion

	int GetColumns();
	int GetRows();
	int GetElementsCount();
	void ChangeValueInMatrix(int row, int column, float value);
    float GetMatrixValue(int row, int column);

	host_vector<float> GetAllMatrixValues();

	void SetColumns(int columns);
	void SetRows(int rows);
	void ChangeAllValuesInMatrix(host_vector<float> &values);
	void PrintMatrix();
	void PrintRow(int row, int start = 1, int end = -1);
	void PrintColumn(int column, int start = 1, int end = -1);

	void TransposeMatrixCPU();
	Matrix TransposeMatrixGPU();
	void TransposeMatrixGPU_Shared();
	float SumAllElements();

private:
	MatrixHandler handler;
	int numberOfRows = 1;
	int numberOfColumns = 1;
	int numberOfElements;
	host_vector<float> matrixValues;

	void AttributeValuesToMatrix(host_vector<float> values);
	void AttributeZerosToMatrix();
	void AttributeOnesToMatrix();
	void SetElementsCount();
};

