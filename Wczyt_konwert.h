
#include <sstream> 
#include <fstream>
#include<iostream>
#include <thrust\host_vector.h>
#include <thrust\device_vector.h>

using namespace thrust;
using namespace std;
void wczytanie_informacji2(int &dlugosc_wiersza, fstream &plik, int &wysokosc, int &offbit, int &bitcount)
{

	unsigned char biWidth[4]; // Zmienne do przechwywania informacji z naglowkow
	unsigned char biHight[4];
	unsigned char dane_poczatkowe[54];
	unsigned char offbits[4];
	unsigned char bitek[2];
	plik.seekg(28, ios_base::beg); // Idziemy do pozycji na ktorej jest biWidth
	plik.read(reinterpret_cast<char*>(bitek), 2);//Wczytujemy te dane
	bitcount = (int)bitek[1] * 256 + (int)bitek[0];
	plik.seekg(10, ios_base::beg); // Idziemy do pozycji na ktorej jest biWidth
	plik.read(reinterpret_cast<char*>(offbits), 4);//Wczytujemy te dane
	offbit = (int)offbits[3] * 256 * 256 * 256 + (int)offbits[2] * 256 * 256 + (int)offbits[1] * 256 + (int)offbits[0];//offbit
	plik.seekg(18, ios_base::beg); // Idziemy do pozycji na ktorej jest biWidth
	plik.read(reinterpret_cast<char*>(biWidth), 4);//Wczytujemy te dane
	plik.read(reinterpret_cast<char*>(biHight), 4);//Wczytujemy te dane
	plik.seekg(0, ios_base::beg);// wracamy na poczatek


	dlugosc_wiersza = (int)biWidth[3] * 256 * 256 * 256 + (int)biWidth[2] * 256 * 256 + (int)biWidth[1] * 256 + (int)biWidth[0];
	wysokosc = (int)biHight[3] * 256 * 256 * 256 + (int)biHight[2] * 256 * 256 + (int)biHight[1] * 256 + (int)biHight[0];
	return;
}


void FromByte(unsigned char c, bool b[8])
{
	for (int i = 0; i < 8; ++i)
		b[i] = (c & (1 << i)) != 0;
}
#define MAX_PATH 260

host_vector<float> rozpocznij_konw(char Nazwa[MAX_PATH] )
{

	

	fstream plik;

	plik.open(Nazwa, std::ios::in | ios::binary); //Otworzenie plikow

	int dlugosc_wiersza = 0;
	int wysokosc = 0;
	int offbit = 0;
	int bitcount = 0;
	int bufor_dlugosc = 0;
	int pom = 0;
	if (plik.good() == true) {

		wczytanie_informacji2(dlugosc_wiersza, plik, wysokosc, offbit, bitcount);
		//cout << "BITCOUNT:" << bitcount << endl;
		if (bitcount == 24)
		{
			bufor_dlugosc = dlugosc_wiersza * 3; 					// Kazdy pixel ma 3 kolory dla danego trybu


			pom = bufor_dlugosc;

			while (bufor_dlugosc % 4 != 0) 								// Uwzgledniamy zera dopelniajace do dzielnika przez 4
				bufor_dlugosc += 1;

			unsigned char*bufor = new unsigned char[bufor_dlugosc];	    // tworzymy zmienna do wczytywania wiersz po wiersz
			pom = bufor_dlugosc - pom; 									// pom mowi nam o tym ile jest zer na koncu wiesza

			unsigned char wart1 = 0;
			unsigned char wart2 = 0;
			unsigned char wart3 = 0;
			
			unsigned char w = 0;
			//cout << pom << endl;
			host_vector<float> matrixValues(dlugosc_wiersza*wysokosc);
			//GenerateRandomVector(matrixValues);
			int k = 0;
			plik.seekg(offbit, ios_base::beg); // Idziemy do pozycji na ktorej jest biWidth
			while (!plik.eof())
			{
				plik.read(reinterpret_cast<char*>(bufor), bufor_dlugosc); // Wczytujemy caly wiersz

				if (!plik.eof())
				{

					for (int i = 0; i<bufor_dlugosc - pom; i += 3) 			// nastepnie wpisujemy piksele usrednione do przerobionego
					{
						wart1 = bufor[i + 2];
						wart2 = bufor[i + 1];
						wart3 = bufor[i];
						w = (wart1 + wart2 + wart3) / 3;

						if (w > 65)
							w = 1;
						else
							w = 0;

						if (k<dlugosc_wiersza*wysokosc)
							matrixValues[k] = w;
						k++;
					}

				}
			}

			delete[] bufor;

			//Matrix obraz = Matrix(wysokosc, dlugosc_wiersza);


			//obraz.ChangeAllValuesInMatrix(matrixValues);
			host_vector<float> matrixValues2(dlugosc_wiersza*wysokosc);
		

			for (int i = 0; i < 400; i++)
			{
				matrixValues2[i] = matrixValues[399 - i];
			}

			return matrixValues2;

		}


		if (bitcount == 1)
		{
			bool ostatni = 0;
			int Ofchar = 0;
			bufor_dlugosc = ceil(float(float(dlugosc_wiersza) / 8));
			if (bufor_dlugosc != dlugosc_wiersza / 8)
			{
				ostatni = 1;
				Ofchar = dlugosc_wiersza - (bufor_dlugosc - 1) * 8;
				Ofchar--;
			}


			pom = bufor_dlugosc;

			while (bufor_dlugosc % 4 != 0) 								// Uwzgledniamy zera dopelniajace do dzielnika przez 4
				bufor_dlugosc += 1;

			unsigned char*bufor = new unsigned char[bufor_dlugosc];

			pom = bufor_dlugosc - pom;
			//cout << pom << endl;
			//cout << dlugosc_wiersza << endl;
			host_vector<float> matrixValues(dlugosc_wiersza*wysokosc);
			//GenerateRandomVector(matrixValues);
			int k = 0;
			int x = 7;
			plik.seekg(offbit, ios_base::beg); // Idziemy do pozycji na ktorej jest biWidth
			while (!plik.eof())
			{
				plik.read(reinterpret_cast<char*>(bufor), bufor_dlugosc); // Wczytujemy caly wiersz

				if (!plik.eof())
				{

					for (int i = 0; i<bufor_dlugosc - pom; i++) 			// nastepnie wpisujemy piksele usrednione do przerobionego
					{
						bool buul[8];

						FromByte(bufor[i], buul);
						//wart3 = bufor[i];
						if (i == bufor_dlugosc - pom - 1 && ostatni == 1)
						{

							x = Ofchar;

						}

						if (k<dlugosc_wiersza*wysokosc)
							for (int j = 7; j >= 7 - x; j--) {
								matrixValues[k] = int(buul[j]);
								k++;
								//cout << int(buul[j]) << " ";
							}
						x = 7;
						if (i == bufor_dlugosc - pom - 1)
						{
							//cout << endl;
						}


					}

				}
			}

			delete[] bufor;

			//Matrix obraz = Matrix(wysokosc, dlugosc_wiersza);
			//obraz.ChangeAllValuesInMatrix(matrixValues);

			//return obraz;
		}

	}
	else
	{
		cout << "Nie otworzono pliku!" << endl;
		//return 0;
	}


	plik.close();
	//return 0;

}

