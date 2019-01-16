#include <iostream>
#include <fstream>
#include <stdio.h>
#include<string.h>
#include<string>
#include <cstdlib>
#include <sstream> 
using namespace std;

void wczytanie_informacji2(int &dlugosc_wiersza, fstream &plik, int &wysokosc,int &offbit,int &bitcount)
{
	unsigned char biWidth[4]; 						//Zmienne do przechwywania informacji z naglowkow
	unsigned char biHight[4];
	unsigned char dane_poczatkowe[54];
	unsigned char offbits[4];
	unsigned char bitek[2];
	plik.seekg(28, ios_base::beg); 					//Idziemy do pozycji na ktorej jest biWidth
	plik.read(reinterpret_cast<char*>(bitek), 2);	//Wczytujemy te dane
	bitcount=(int)bitek[1] * 256 + (int)bitek[0];
	plik.seekg(10, ios_base::beg);					//Idziemy do pozycji na ktorej jest biWidth
	plik.read(reinterpret_cast<char*>(offbits), 4);	//Wczytujemy te dane
	offbit= (int)offbits[3] * 256 * 256 * 256 + (int)offbits[2] * 256 * 256 + (int)offbits[1] * 256 + (int)offbits[0];//Offbit
	plik.seekg(18, ios_base::beg); 					//Idziemy do pozycji na ktorej jest biWidth
	plik.read(reinterpret_cast<char*>(biWidth), 4);	//Wczytujemy te dane
	plik.read(reinterpret_cast<char*>(biHight), 4);	//Wczytujemy te dane
	plik.seekg(0, ios_base::beg);					//Wracamy na poczatek					
	dlugosc_wiersza = (int)biWidth[3] * 256 * 256 * 256 + (int)biWidth[2] * 256 * 256 + (int)biWidth[1] * 256 + (int)biWidth[0];
	wysokosc = (int)biHight[3] * 256 * 256 * 256 + (int)biHight[2] * 256 * 256 + (int)biHight[1] * 256 + (int)biHight[0];
	return;
}
void kopiuj_naglowek(fstream &plik_odczyt, ofstream *plik_zapis,int p)
{
		unsigned char dane_poczatkowe[54];
		plik_odczyt.read(reinterpret_cast<char*>(dane_poczatkowe), 54);		//Pobieramy caly naglowek
		
		plik_zapis[p].write( reinterpret_cast<char*>(dane_poczatkowe), 54 );	//Wklejamy go do nowego pliku

	return;
}
/*
char * zlacz( char * a, char * b ) {
    int aa = strlen( a );
    int bb = strlen( b );
    int cc = a + b;
    char * c = new char[ cc + 1 ];
    c[ cc ] = 0;
    for( int i = 0; i < aa; i++ ) c[ i ] = a[ i ];
    
    for( int i = aa; i < cc; i++ ) c[ i ] = b[ i - aa ];
    
    return c;
}

*/


int main()
{
	char wybor;
	fstream plik;
	fstream plik_20;
	
	
	plik.open("digits_BM.bmp", std::ios::in | ios::binary); //Otworzenie plikow
	plik_20.open("digits_BM_20.bmp", std::ios::in | ios::binary); //Otworzenie plikow
	
	

    
              
    
   
    
    //liczba<<bufor[bufor_dlugosc-i]; // przepisujemy te zera	
    
    
	int dlugosc_wiersza = 0;
	int wysokosc = 0;
	int offbit = 0;
	int bitcount = 0;
	int bufor_dlugosc = 0;
	int pom = 0;
	//plik.seekg( 0, std::ios::end );
	if (plik.good() == true and plik_20.good()==true) 
	{

		wczytanie_informacji2(dlugosc_wiersza, plik, wysokosc,offbit,bitcount);
		
		
		
		
		
		
		
		if (bitcount == 24)
		{
			bufor_dlugosc = dlugosc_wiersza * 3; 							// Kazdy pixel ma 3 kolory dla danego trybu


			pom = bufor_dlugosc;

			while (bufor_dlugosc % 4 != 0) 									// Uwzgledniamy zera dopelniajace do dzielnika przez 4
				bufor_dlugosc += 1;

			unsigned char*bufor = new unsigned char[bufor_dlugosc];	  	 	// tworzymy zmienna do wczytywania wiersz po wiersz
			pom = bufor_dlugosc - pom; 										// pom mowi nam o tym ile jest zer na koncu wiesza

			unsigned char wart1 = 0;
			unsigned char wart2 = 0;
			unsigned char wart3 = 0;
			int x = 0;
			unsigned char w = 0;

			int k = 0;
			plik.seekg(offbit, ios_base::beg); 								// Idziemy do pozycji na ktorej jest biWidth
			
			int liczba_plikow=dlugosc_wiersza*wysokosc/400;
			cout<<endl<<liczba_plikow<<endl;
			
			
			string a="liczba";
			string xd="rzad";
			string  b=".bmp";
			ofstream *liczba=new ofstream[liczba_plikow];
			unsigned char dane_poczatkowe[54];
			plik_20.read(reinterpret_cast<char*>(dane_poczatkowe), 54);		//Pobieramy caly naglowek
			string *nazwy=new string[liczba_plikow];
			
			for(int i=0;i<liczba_plikow;i++)
			{
			int ssdd=i/100;
			
			stringstream ss;
			ss << i;
			string str = ss.str();
			
			
			stringstream s2;
			s2 << ssdd;
			string sx = s2.str();
			
			string wynik=xd+sx+a+str+b;
			nazwy[i]=wynik;
			char sciezka[1024];
			strcpy(sciezka, wynik.c_str());

			liczba[i].open( sciezka,  std::ios::out | ios::binary );  
			liczba[i].write( reinterpret_cast<char*>(dane_poczatkowe), 54 );	
			liczba[i].close();
			}
			
		
			int licznik_wiersza=0;
			int licznik_XD=0;
			while (!plik.eof())
			{
		
			
				plik.read(reinterpret_cast<char*>(bufor), bufor_dlugosc); 	// Wczytujemy caly wiersz
				if (!plik.eof())
				{
					
					for (int i = 0; i<bufor_dlugosc - pom; i += 3) 			// nastepnie wpisujemy piksele usrednione do przerobionego
					{
						
						//liczjik_XD=(i+(licznik_wiersza)*100)/60;
						licznik_XD=(licznik_wiersza/20)*100+i/60;
						if(!liczba[licznik_XD].is_open())
						{
						
						char sciezka[1024];
						strcpy(sciezka, nazwy[licznik_XD].c_str());
						liczba[licznik_XD].open( sciezka,  std::ios::out | ios::binary|ios::app );  
						}
						 //liczba[licznik_XD].seekp( 0, ios_base::end );
						 liczba[licznik_XD]<< bufor[i ];
						 liczba[licznik_XD]<<bufor[i + 1];
						 liczba[licznik_XD]<<bufor[i+2];
						 
						 if(licznik_XD%100==0)
						 {
						 for(int k=0;k<licznik_XD;k++)
						 if(liczba[k].is_open())
						 liczba[k].close();
						}
						
					}
						
					//for(int i=0;i<pom;i++)
				//	liczba[licznik_XD]<<bufor[bufor_dlugosc-i]; // przepisujemy te zera	
					
				}
				if(licznik_wiersza%100==0)
				{
					cout<<"Postep="<<licznik_wiersza/10<<"%"<<endl;
				}
				licznik_wiersza++;
				
			}
			
			for(int i=0;i<liczba_plikow;i++)
			{
				if(liczba[i].is_open())
			liczba[i].close();
			}
			
			delete[] bufor;

		}
		
			
	}
	else
	{
		cout << "Nie otworzono pliku!" << endl;
	}

	plik.close();
	plik_20.close();
	return 0;

}



