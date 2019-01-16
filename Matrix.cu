#include "Matrix.h"
#include "iostream"

#define NUMBER_OF_THREADS 1024

Matrix::Matrix(int size, host_vector<float> values) {
	this->numberOfRows = size;
	this->numberOfColumns = size;
	this->numberOfElements = size * size;
	AttributeValuesToMatrix(values);
}
Matrix::Matrix(int rows, int columns) {
	this->numberOfRows = rows;
	this->numberOfColumns = columns;
	this->numberOfElements = rows * columns;
	AttributeOnesToMatrix();
}

Matrix::Matrix(int rows, int columns, host_vector<float> &values) {
	this->numberOfRows = rows;
	this->numberOfColumns = columns;
	this->numberOfElements = rows * columns;
	ChangeAllValuesInMatrix(values);
}

Matrix::~Matrix() {}

void Matrix::operator=(Matrix matrix) {
	this->SetRows(matrix.GetRows());
	this->SetColumns(matrix.GetColumns());
	this->ChangeAllValuesInMatrix(matrix.GetAllMatrixValues());
}
Matrix Matrix::operator*(float value) {
	Matrix outMatrix(this->GetRows(), this->GetColumns());

	host_vector<float> h_inMatrixValues = this->GetAllMatrixValues();
	host_vector<float> h_outMatrixValues;
	device_vector<float> d_inMatrixValues = h_inMatrixValues;
	device_vector<float> d_outMatrixValues = h_inMatrixValues;

	float *d_in_pointer = raw_pointer_cast(d_inMatrixValues.data());
	float *d_out_pointer = raw_pointer_cast(d_outMatrixValues.data());

	int blocks = this->GetRows() / NUMBER_OF_THREADS;
	blocks += 1;

	cudaMultiplicationScalar<<<blocks, NUMBER_OF_THREADS>>>(d_in_pointer, d_out_pointer, value, this->GetRows(), this->GetColumns());

	cudaThreadSynchronize();

	h_outMatrixValues = d_outMatrixValues;
	outMatrix.ChangeAllValuesInMatrix(h_outMatrixValues);

	return outMatrix;
}
Matrix Matrix::operator*(Matrix matrix) {
	Matrix outMatrix(this->GetRows(), matrix.GetColumns());

	host_vector<float> h_inMatrixA = this->GetAllMatrixValues();
	host_vector<float> h_inMatrixB = matrix.GetAllMatrixValues();
	host_vector<float> h_outMatrix = outMatrix.GetAllMatrixValues();
	device_vector<float> d_inMatrixA = h_inMatrixA;
	device_vector<float> d_inMatrixB = h_inMatrixB;
	device_vector<float> d_outMatrix = h_outMatrix;

	float *d_in_pointerA = raw_pointer_cast(d_inMatrixA.data());
	float *d_in_pointerB = raw_pointer_cast(d_inMatrixB.data());
	float *d_out_pointer = raw_pointer_cast(d_outMatrix.data());

	//int blocks = this->GetRows() / NUMBER_OF_THREADS;
	//blocks += 1;
	int numberOfElements = this->GetRows()*matrix.GetColumns();

	int blocks = numberOfElements / NUMBER_OF_THREADS;
	blocks++;
	int threads = numberOfElements / blocks;

	cudaMultiplicationMatrix2 << <blocks, threads >> >(d_in_pointerA, d_in_pointerB, d_out_pointer, this->GetRows(), this->GetColumns(), matrix.GetRows(), matrix.GetColumns());

	cudaThreadSynchronize();

	h_outMatrix = d_outMatrix;
	outMatrix.ChangeAllValuesInMatrix(h_outMatrix);

	return outMatrix;
}
#pragma region RESZTA



Matrix Matrix::operator%(Matrix matrix) {
	Matrix outMatrix(this->GetRows(), this->GetColumns());

	host_vector<float> h_inMatrixA = this->GetAllMatrixValues();
	host_vector<float> h_inMatrixB = matrix.GetAllMatrixValues();
	host_vector<float> h_outMatrix = outMatrix.GetAllMatrixValues();
	device_vector<float> d_inMatrixA = h_inMatrixA;
	device_vector<float> d_inMatrixB = h_inMatrixB;
	device_vector<float> d_outMatrix = h_outMatrix;

	float *d_in_pointerA = raw_pointer_cast(d_inMatrixA.data());
	float *d_in_pointerB = raw_pointer_cast(d_inMatrixB.data());
	float *d_out_pointer = raw_pointer_cast(d_outMatrix.data());

	int blocks = this->GetRows() / NUMBER_OF_THREADS;
	blocks += 1;

	cudaMultiplication << <blocks, NUMBER_OF_THREADS >> > (d_in_pointerA, d_in_pointerB, d_out_pointer, this->GetRows(), this->GetColumns());

	cudaThreadSynchronize();

	h_outMatrix = d_outMatrix;
	outMatrix.ChangeAllValuesInMatrix(h_outMatrix);
	return outMatrix;
}
Matrix Matrix::operator+(Matrix matrix) {
	Matrix outMatrix(this->GetRows(), this->GetColumns());

	host_vector<float> h_inMatrixA = this->GetAllMatrixValues();
	host_vector<float> h_inMatrixB = matrix.GetAllMatrixValues();
	host_vector<float> h_outMatrix = outMatrix.GetAllMatrixValues();
	device_vector<float> d_inMatrixA = h_inMatrixA;
	device_vector<float> d_inMatrixB = h_inMatrixB;
	device_vector<float> d_outMatrix = h_outMatrix;

	float *d_in_pointerA = raw_pointer_cast(d_inMatrixA.data());
	float *d_in_pointerB = raw_pointer_cast(d_inMatrixB.data());
	float *d_out_pointer = raw_pointer_cast(d_outMatrix.data());

	int blocks = this->GetRows() / NUMBER_OF_THREADS;
	blocks += 1;

	cudaAddition << <blocks, NUMBER_OF_THREADS >> > (d_in_pointerA, d_in_pointerB, d_out_pointer, this->GetRows(), this->GetColumns());

	cudaThreadSynchronize();

	h_outMatrix = d_outMatrix;
	outMatrix.ChangeAllValuesInMatrix(h_outMatrix);
	return outMatrix;
}
Matrix Matrix::operator+(float value) {
	Matrix outMatrix(this->GetRows(), this->GetColumns());

	host_vector<float> h_inMatrixValues = this->GetAllMatrixValues();
	host_vector<float> h_outMatrixValues;
	device_vector<float> d_inMatrixValues = h_inMatrixValues;
	device_vector<float> d_outMatrixValues = h_inMatrixValues;

	float *d_in_pointer = raw_pointer_cast(d_inMatrixValues.data());
	float *d_out_pointer = raw_pointer_cast(d_outMatrixValues.data());

	int blocks = this->GetRows() / NUMBER_OF_THREADS;
	blocks += 1;

	cudaAdditionScalar << <blocks, NUMBER_OF_THREADS >> >(d_in_pointer, d_out_pointer, value, this->GetRows(), this->GetColumns());

	cudaThreadSynchronize();

	h_outMatrixValues = d_outMatrixValues;
	outMatrix.ChangeAllValuesInMatrix(h_outMatrixValues);

	return outMatrix;
}
Matrix Matrix::operator-(Matrix matrix) {
	Matrix outMatrix(this->GetRows(), this->GetColumns());

	host_vector<float> h_inMatrixA = this->GetAllMatrixValues();
	host_vector<float> h_inMatrixB = matrix.GetAllMatrixValues();
	host_vector<float> h_outMatrix = outMatrix.GetAllMatrixValues();
	device_vector<float> d_inMatrixA = h_inMatrixA;
	device_vector<float> d_inMatrixB = h_inMatrixB;
	device_vector<float> d_outMatrix = h_outMatrix;

	float *d_in_pointerA = raw_pointer_cast(d_inMatrixA.data());
	float *d_in_pointerB = raw_pointer_cast(d_inMatrixB.data());
	float *d_out_pointer = raw_pointer_cast(d_outMatrix.data());

	int blocks = this->GetRows() / NUMBER_OF_THREADS;
	blocks += 1;

	cudaSubtraction << <blocks, NUMBER_OF_THREADS >> > (d_in_pointerA, d_in_pointerB, d_out_pointer, this->GetRows(), this->GetColumns());

	cudaThreadSynchronize();

	h_outMatrix = d_outMatrix;
	outMatrix.ChangeAllValuesInMatrix(h_outMatrix);
	return outMatrix;
}
int Matrix::GetColumns() {
	return numberOfColumns;
}
int Matrix::GetRows() {
	return numberOfRows;
}
int Matrix::GetElementsCount() {
	return numberOfElements;
}
void Matrix::ChangeValueInMatrix(int row, int column, float value) {
	this->matrixValues[row*column + column] = value;
}
float Matrix::GetMatrixValue(int row, int column) {
	return this->matrixValues[row*this->GetColumns() + column];
}
host_vector<float> Matrix::GetAllMatrixValues() {
	return this->matrixValues;
}
void Matrix::SetColumns(int columns) {
	this->numberOfColumns = columns;
	this->SetElementsCount();
}
void Matrix::SetRows(int rows) {
	this->numberOfRows = rows;
	this->SetElementsCount();
}
void Matrix::ChangeAllValuesInMatrix(host_vector<float> &values) {
	this->matrixValues = values;
}
void Matrix::AttributeValuesToMatrix(host_vector<float> values) {
	if (this->handler.CheckIfThereAreTooManyValuesInMatrix(this, values) == this->handler.GetNoError())
		for (int i = 0; i < this->GetElementsCount(); i++)
		{
			this->matrixValues.push_back(values[i]);
		}
	else
	{
		AttributeZerosToMatrix();
	}
}
void Matrix::AttributeZerosToMatrix() {
	for (int i = 0; i < this->GetElementsCount(); i++)
	{
		this->matrixValues.push_back(0);
	}
}
void Matrix::AttributeOnesToMatrix() {
	for (int i = 0; i < this->GetElementsCount(); i++)
	{
		this->matrixValues.push_back(1);
	}
}
void Matrix::SetElementsCount() {
	this->numberOfElements = this->numberOfColumns * this->numberOfRows;
}
void Matrix::PrintMatrix() {
	for (int i = 0; i < this->GetRows(); i++)
	{
		for (int j = 0; j < this->GetColumns(); j++)
		{
			cout << this->GetMatrixValue(i, j) << "\t";
		}
		cout << endl;
	}
}

void Matrix::TransposeMatrixCPU(){
	host_vector<float> temp = this->GetAllMatrixValues();
	host_vector<float> outMatrix = temp;

	int rows = this->GetRows();
	int columns = this->GetColumns();

	for (int i = 0; i < rows; i++)
	{
		for (int k = 0; k < columns; k++)                                                                                                   
		{                                                        
			outMatrix[rows * k + i] = temp[i * columns + k];
		}
	}

	this->SetColumns(rows);
	this->SetRows(columns);
	this->ChangeAllValuesInMatrix(outMatrix);
}

Matrix Matrix::TransposeMatrixGPU(){
	device_vector<float> inMatrix = this->GetAllMatrixValues();
	device_vector<float> outMatrix = inMatrix;

	float* d_in_pointer = raw_pointer_cast(inMatrix.data());
	float* d_out_pointer = raw_pointer_cast(outMatrix.data());

	int numberOfElements = this->GetElementsCount();

	int rows = this->GetRows();
	int columns = this->GetColumns();

	int blocks = columns / NUMBER_OF_THREADS;
	blocks += 1;
	int threads = columns / blocks;
	threads += 1;

	cudaTranspose << < blocks, threads >> > (d_in_pointer, d_out_pointer, rows, columns);
	cudaThreadSynchronize();

	host_vector<float> transValues = outMatrix;

	Matrix newMatrix = Matrix(columns, rows, transValues);

	return newMatrix;
}

void Matrix::TransposeMatrixGPU_Shared() {
	device_vector<float> inMatrix = this->GetAllMatrixValues();
	device_vector<float> outMatrix = inMatrix;

	float* d_in_pointer = raw_pointer_cast(inMatrix.data());
	float* d_out_pointer = raw_pointer_cast(outMatrix.data());

	int rows = this->GetRows();
	int columns = this->GetColumns();

	int blocks = columns / NUMBER_OF_THREADS;
	blocks += 1;
	int threads = columns / blocks;
	threads += 1;

	cudaTransposeShared << < blocks, threads, (2 * columns * sizeof(float) + 4) >> >(d_in_pointer, d_out_pointer, rows, columns);
	cudaThreadSynchronize();

	host_vector<float> transValues = outMatrix;

	this->SetColumns(rows);
	this->SetRows(columns);
	this->ChangeAllValuesInMatrix(transValues);
}

float Matrix::SumAllElements()
{
	device_vector<float> matrix = this->GetAllMatrixValues();

	int rows = this->GetRows();
	int columns = this->GetColumns();

	vector<float> temp(rows, 0);
	host_vector<float> h_temp = temp;
	vector<float>().swap(temp);
	device_vector<float> sum = h_temp;

	//cout << h_temp[1];

	float* matrix_pointer = raw_pointer_cast(matrix.data());
	float* values_pointer = raw_pointer_cast(sum.data());

	int blocks = this->GetRows() / NUMBER_OF_THREADS;
	blocks += 1;

	cudaSumAll << < blocks, NUMBER_OF_THREADS >> > (matrix_pointer, rows, columns, values_pointer);
	cudaThreadSynchronize();

	h_temp = sum;
	float result = h_temp[0];

	return result;
}

void Matrix::PrintRow(int row, int start, int end) {
	if (end == -1)
	{
		end = this->GetRows()-1;
	}

	for (int i = start; i <= end; i++)
	{
		cout << this->GetMatrixValue(row, i) << "\t";
	}
	cout << endl;
}

void Matrix::PrintColumn(int column, int start, int end) {
	if (end == -1)
	{
		end = this->GetColumns() - 1;
	}

	for (int i = start; i <= end; i++)
	{
		cout << this->GetMatrixValue(i, column) << endl;
	}
}

#pragma endregion

__global__ void cudaMultiplicationScalar(float* inMatrixValues, float* outMatrixValues, float value, int matrixRows, int matrixColumns) {

	int index = threadIdx.x + blockIdx.x * blockDim.x;

	if (index < matrixRows)
	{
		for (int i = index * matrixColumns; i < (index + 1) * matrixColumns; i++)
		{
			outMatrixValues[i] = inMatrixValues[i] * value;
		}
	}

	return;
}

__global__ void cudaMultiplicationMatrix(float* inMatrixA, float* inMatrixB, float* outMatrix, int rowsA, int columnsA, int rowsB, int columnsB) {

	int index = threadIdx.x + blockIdx.x * blockDim.x;

	if (index < rowsA)
	{
		for (int i = index * columnsB; i < (index + 1) * columnsB; i++)
		{
			float value = 0;
			for (int k = index * columnsA, int m = i % columnsB; k < columnsA * (index + 1); k++, m += columnsB)
			{
				value = value + inMatrixA[k] * inMatrixB[m];
			}
			outMatrix[i] = value;
		}
	}
	return;
}
__global__ void cudaMultiplicationMatrix2(float* inMatrixA, float* inMatrixB, float* outMatrix, int rowsA, int columnsA, int rowsB, int columnsB) {

	int index = threadIdx.x + blockIdx.x * blockDim.x;

	if(rowsA*columnsB>index)
	{ 
	// rowsA X columnsB
	outMatrix[index] = 0;

	int row_index = index / columnsB;
	//rows_index = index - rows_index * columnsB;
	int column_index = index -(row_index*columnsB);
	//colums_index = index - colums_index * rowsA;
	int MatrixA_indeks = row_index * columnsA;
	int MatrixB_indeks =column_index;


	for (int i = 0; i < columnsA; i++)
		outMatrix[index]+=inMatrixA[MatrixA_indeks + i] * inMatrixB[MatrixB_indeks + columnsB * i];
	}
	return;
}

__global__ void cudaTranspose(float* inMatrixValues, float* outMatrixValues, int rows, int columns) {
	int index = threadIdx.x + blockIdx.x * blockDim.x;
	int x = (rows - 1)*columns;

	if (index < columns)
	{
		for (int i = index, int k = 0, int m = 0; i <= x + index; i += columns, m += 1)
		{
			outMatrixValues[m + index * rows] = inMatrixValues[i];

			if (k == 1) k = 0;
			else k++;
		}
	}
	return;
}

__global__ void cudaTransposeShared(float* inMatrixValues, float* outMatrixValues, int rows, int columns) {
	extern __shared__ short int temp[];

	int index = threadIdx.x + blockIdx.x * blockDim.x;
	int x = (rows - 1)*columns;

	if (index < columns)
	{
		for(int i = index, int k = 0, int m = 0; i <= x + index; i += columns, m+=1)
		{
			temp[k + 2*index] = inMatrixValues[i];
			outMatrixValues[m + index * rows] = temp[k + 2*index];
			
			if (k == 1) k = 0;
			else k++;
		}
	}
	return;
}

__global__ void cudaAddition(float* inMatrixA, float* inMatrixB, float* outMatrix, int rows, int columns) {

	int index = threadIdx.x + blockDim.x * blockIdx.x;

	if (index < rows)
	{
		for (int i = index * columns; i < (index + 1) * columns; i++)
		{
			outMatrix[i] = inMatrixA[i] + inMatrixB[i];
		}
	}
	return;
}

__global__ void cudaSubtraction(float* inMatrixA, float* inMatrixB, float* outMatrix, int rows, int columns) {

	int index = threadIdx.x + blockDim.x * blockIdx.x;

	if (index < rows)
	{
		for (int i = index * columns; i < (index + 1) * columns; i++)
		{
			outMatrix[i] = inMatrixA[i] - inMatrixB[i];
		}
	}
	return;
}

__global__ void cudaSumAll(float* matrix, int rows, int columns, float* outValue) {

	int index = threadIdx.x + blockDim.x * blockIdx.x;

	if (index < rows)
	{
		for (int i = index * columns; i < columns * (index + 1); i++)
		{
			outValue[index] = outValue[index] + matrix[i];
		}
	}

	__syncthreads();

	if (index == 0)
	{
		float buffer;

		for (int i = index; i < rows; i++)
		{
			buffer = buffer + outValue[i];
		}

		outValue[index] = buffer;
	}

	return;
}

__global__ void cudaAdditionScalar(float* inMatrixValues, float* outMatrixValues, float value, int matrixRows, int matrixColumns)
{
	int index = threadIdx.x + blockIdx.x * blockDim.x;

	if (index < matrixRows)
	{
		for (int i = index * matrixColumns; i < (index + 1) * matrixColumns; i++)
		{
			outMatrixValues[i] = inMatrixValues[i] + value;
		}
	}

	return;
}
__global__ void cudaMultiplication(float* inMatrixA, float* inMatrixB, float* outMatrix, int rows, int columns)
{
	int index = threadIdx.x + blockDim.x * blockIdx.x;

	if (index < rows)
	{
		for (int i = index * columns; i < (index + 1) * columns; i++)
		{
			outMatrix[i] = inMatrixA[i] * inMatrixB[i];
		}
	}
	return;
}