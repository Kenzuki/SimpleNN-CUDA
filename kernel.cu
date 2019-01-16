

#include <sstream> 
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <time.h>
#include "api.h"
using namespace std;



int main()
{
	srand(time(NULL));
	a();


#pragma region kernel
/*





	host_vector<float> jedn;

	int counter = 0;

	for (int i = 0; i < 200; i++)
	{
		for (int k = 0; k < 10; k++)
		{
			if(k == counter)jedn.push_back(1);
			else jedn.push_back(0);
		}
		counter++;
		if (counter == 10) counter = 0;
	}


	string N1 = "liczby/rzad";
	string N2 = "liczba";
	string N3 = ".bmp";


	Matrix liczby[5000];
	host_vector<float> matrixValues;
	host_vector<float> matrixValues2(400);
	host_vector<float> targets;
	int value = 9;

	// 0- 0000 0000 01
	//1 - 0000 0000 10
	int ile_wczyt = 5000;
	int tab[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
	for (int i = 0; i < ile_wczyt; i++)
	{

		if (i % 500 == 0 && i != 0)
		{
			tab[value] = 0;
			value--;
			tab[value] = 1;
		}

		for (int kk = 0; kk < 10; kk++)
			targets.push_back(tab[kk]);

		char adres[260];

		stringstream liczba;
		liczba << i;
		string liczba_zamieniona = liczba.str();

		stringstream liczba2;
		liczba2 << int(i / 100);
		string liczba_zamieniona2 = liczba2.str();


		string wynik = N1 + liczba_zamieniona2 + N2 + liczba_zamieniona + N3;
		strcpy(adres, wynik.c_str());
		//cout << adres<<endl;
		matrixValues2 = rozpocznij_konw(adres);
		for (int k = 0; k < matrixValues2.size(); k++)
		{
			matrixValues.push_back(matrixValues2[k]);
		}
	}

	aranzacja(targets, matrixValues);

	if (0)
	{
		ofstream plik("wynik.txt", std::ios::out | ios::app);
		ofstream plik2("Macierz.txt", std::ios::out | ios::app);
		ofstream plik3("test.txt", std::ios::out | ios::app);
		//plik.open("wykres.txt", );
		host_vector<float> matrixValuesX(400);
		matrixValuesX = rozpocznij_konw("C:/Users/Patryk/source/repos/neural_network/neural_network/liczby/rzad6liczba657.bmp");

		string x="";	//Wklejamy go do nowego pliku
		for (int z = 0; z<targets.size(); z++)
		{

			stringstream ss;
			string h;
			ss << targets[z];
			h = ss.str();
			x += h;
			if(z%10!=0 || z==0)
			x += ",";
			if (z % 10 == 0 && z != 0)
			{
				x += "\n";

				plik.write(x.c_str(), x.length());
				x = "";
			}


		}
		x = "";
		for (int z = 0; z<matrixValues.size(); z++)
		{

			stringstream ss;
			string h;
			ss << matrixValues[z];
			h = ss.str();
			x += h;
			if (z % 400 != 0 || z == 0)
				x += ",";

			if (z % 400 == 0 && z != 0)
			{
				x += "\n";

				plik2.write(x.c_str(), x.length());
				x = "";
			}


		}
		for (int z = 0; z<matrixValuesX.size(); z++)
		{

			stringstream ss;
			string h;
			ss << matrixValuesX[z];
			h = ss.str();
			x += h;
			if (z % 400 != 0 && z != 0)
				x += ",";

			if (z % 400 == 0 && z != 0)
			{
				x += "\n";

				plik3.write(x.c_str(), x.length());
				x = "";
			}


		}


		


		plik.close();
		plik2.close();
		plik3.close();
	}
		
	//std::system("pause");


	Matrix X = Matrix(ile_wczyt, 400, matrixValues);
	Matrix Y = Matrix(ile_wczyt, 10, targets);
	//Y.PrintMatrix();
	

	vector<int> size = { 35, 10 };

	vector<float> layerA = { float(-1/sqrt(ile_wczyt)),  float(1 / sqrt(ile_wczyt)) };
	//vector<float> layerB = { 0, 1 };
	vector<float> output = { float(-1 / sqrt(ile_wczyt)),  float(1 / sqrt(ile_wczyt)) };
	vector<vector<float>> range = { layerA,output };
	vector<string> activation = { "tanh","sigmoid" };

	//vector<float> temp = { 1,0,1,0,1,0,1,1,0,1,0,1};d
	//host_vector<float> x = temp;
	//vector<float> temp2 = { 1,0,1,0,0,1};
	//host_vector<float> y = temp2;

	//Matrix X = Matrix(3, 4, x);
	//Matrix Y = Matrix(3, 2, y);

	NeuralNetwork net = NeuralNetwork(size, range, activation);
	net.FitData(X);
	auto start = std::chrono::system_clock::now();

	net.Train(Y);

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> czas = end - start;

	printf("Uplynelo czasu na cpu :%f \n", czas);
	std::system("pause");

	host_vector<float> matrixValuesX(400);
	matrixValuesX = rozpocznij_konw("C:/Users/Patryk/source/repos/neural_network/neural_network/liczby/rzad6liczba657.bmp");
	cout << endl;
	for (int k = 0; k <400; k++)
	{

		if (k % 20 == 0 && k != 0)
			cout << endl;

		if (matrixValuesX[k] == 1)
			cout << "* ";

		if (matrixValuesX[k] == 0)
			cout << ". ";
		//if (k == 19)
		//	cout << endl;

		
	}

	cout << endl;
	Matrix test = Matrix(1, 400, matrixValuesX);
	
	Matrix result = net.Predict(test);
	cout << endl;
	result.PrintMatrix();
	host_vector<float> u = result.GetAllMatrixValues();

    int wynik = 0;
	float wartosc = u[0];
	for (int y = 1; y < 10; y++)
	{
	
		if (u[y] > wartosc)
		{
			
			wartosc = u[y];
			wynik = y;
		}
	}
	cout << "WYNIK :" << wynik << endl;

	matrixValuesX = rozpocznij_konw("tescik.bmp");
	cout << endl;
	for (int k = 0; k <400; k++)
	{

		if (k % 20 == 0 && k != 0)
			cout << endl;

		if (matrixValuesX[k] == 1)
			cout << "* ";

		if (matrixValuesX[k] == 0)
			cout << ". ";
		//if (k == 19)
		//	cout << endl;


	}

	cout << endl;
	 test = Matrix(1, 400, matrixValuesX);

	 result = net.Predict(test);
	cout << endl;
	result.PrintMatrix();
	 u = result.GetAllMatrixValues();

	 wynik = 0;
	 wartosc = u[0];
	for (int y = 1; y < 10; y++)
	{

		if (u[y] > wartosc)
		{

			wartosc = u[y];
			wynik = y;
		}
	}
	cout << "WYNIK :" << wynik << endl;

	host_vector<float> matrixValues11;
	host_vector<float> matrixValues22(400);
	host_vector<float> targets11;

	int value2 = 9;

	// 0- 0000 0000 01
	//1 - 0000 0000 10
	
	int tab2[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

	for (int i = 0; i < ile_wczyt; i++)
	{

		if (i % 500 == 0 && i != 0)
		{
			tab2[value] = 0;
			value2--;
			tab2[value] = 1;
		}

		for (int kk = 0; kk < 10; kk++)
			targets11.push_back(tab[kk]);

		char adres[260];

		stringstream liczba;
		liczba << i;
		string liczba_zamieniona = liczba.str();

		stringstream liczba2;
		liczba2 << int(i / 100);
		string liczba_zamieniona2 = liczba2.str();


		string wynik = N1 + liczba_zamieniona2 + N2 + liczba_zamieniona + N3;
		strcpy(adres, wynik.c_str());
		//cout << adres<<endl;
		matrixValues22 = rozpocznij_konw(adres);
		for (int k = 0; k < matrixValues22.size(); k++)
		{
			matrixValues11.push_back(matrixValues22[k]);
		}
	}



	Matrix test2 = Matrix(1, 400, matrixValues11);

	//aranzacja(targets, matrixValues);
	X = Matrix(ile_wczyt, 400, matrixValues);
	Y = Matrix(ile_wczyt, 10, targets);

	Matrix result2 = net.Predict(X);
	cout << endl;
	u = result2.GetAllMatrixValues();
	host_vector<float> res;

	for (int i = 0; i < result2.GetRows();i++)
	{
		wynik = 0;
		wartosc = u[i*result2.GetColumns()];
		for (int y = 1; y < 10; y++)
		{
			if (u[i*result2.GetColumns()+y] > wartosc)
			{

				wartosc = u[i*result2.GetColumns() + y];
				wynik = y;
			}
		}
		res.push_back(wynik);
		cout << "WYNIK przewidywany :" << wynik << endl;
	}

	host_vector<float> ys = Y.GetAllMatrixValues();
	host_vector<float> tar;

	for (int i = 0; i < Y.GetRows(); i++)
	{
		wynik = 0;
		wartosc = ys[i*result2.GetColumns()];
		for (int y = 1; y < 10; y++)
		{
			if (ys[i*result2.GetColumns() + y] > wartosc)
			{

				wartosc = ys[i*result2.GetColumns() + y];
				wynik = y;
			}
		}
		tar.push_back(wynik);
		cout << "WYNIK prawdziwy:" << wynik << endl;
	}


	int good = 0;
	int all = result2.GetRows();

	for (int i = 0; i < res.size(); i++)
	{
		if (res[i] == tar[i]) good++;
	}

	float acc = float(100) * float(good) / float(all);
	cout << "Wynik w %: " << acc << endl;






*/
#pragma endregion
	std::system("pause");

}

