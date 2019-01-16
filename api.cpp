

#include "api.h"
#include "stdafx.h"
#include "NeuralNetwork.h"
#include"Wczyt_konwert.h"
int iteracje;
void aranzacja(host_vector<float> &targ, host_vector<float> &m)
{

	int x = 0;
	int y = 0;
	int x2 = 0;
	int y2 = 0;
	int l = 0;
	float buforX = 0;
	float buforY = 0;
	int w = (targ.size() / 10) - 1;
	//std::cout << "ROZM=" << targ.size() << endl;;
	for (int i = 0; i < 50000; i++)
	{
		x = rand() % w;//49999
		x *= 10;

		y = x * 40;

		if (i % 10000 == 0)
		{
			///	std::cout <<"liczba:"<< i / 10000 << endl;
		}

		do
		{
			x2 = rand() % w;
			//l = x2 / 10;
			//l = x2 - l * 10;
			//	x2 = x2 - l;
			x2 *= 10;

		} while (x2 == x);

		y2 = x2 * 40;


		for (int lel = 0; lel < 10; lel++)
		{
			buforX = targ[x + lel];

			targ[x + lel] = targ[x2 + lel];
			targ[x2 + lel] = buforX;
		}

		for (int lel = 0; lel < 400; lel++)
		{
			buforY = m[x + lel];

			m[x + lel] = m[x2 + lel];
			m[x2 + lel] = buforY;
		}


	}
	std::cout << "KONIEC\n";

	return;
}


void GenerateRandomVector(host_vector<float> &u) {
	for (int i = 0; i < u.size(); i++)
	{
		u[i] = rand() % 10;
	}
}

/*

int pomocnicza = 10;
int pomoc = 0;
int wybor = 1;
double czas = 0;
double przyspieszenie = 0.2;
*/
#pragma region HWND



HWND hProgressBar;
HWND przycisk1;
HWND przycisk2;
HWND Box;

HWND przycisk3;
HWND przycisk4;

HWND przycisk5;
HWND przycisk6;
HWND przycisk7;
HWND przycisk8;
HWND przycisk9;
HWND przycisk10;

HWND Text1;
HWND Text2;
HWND Text3;
HWND Text4;
HWND Text5;

HWND Text6;

HWND Text7;
HWND Text8;
HWND Text9;
HWND Text10;
#pragma endregion

#define Start 8
#define Otworz 7


using namespace std;
int Pasek_ladowania = 0;
char sNazwaPliku[MAX_PATH] = "";

class dane
{
public:
unsigned short int liczba_neuronow;
unsigned short int liczba_iteracji;
unsigned short int liczba_warstw;
unsigned short waga_maks, waga_min;



};
dane info;
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	int wmId;
	//FreeConsole();
	OPENFILENAME ofn;
	PAINTSTRUCT ps;
	HDC hdc;
	int k = 0;
	float x = 0;
	switch (Message) {

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		
		info.liczba_neuronow=GetDlgItemInt(hwnd, 2, 0, 1);
		info.liczba_iteracji = GetDlgItemInt(hwnd, 3, 0, 1);
		

		//wmEvent = HIWORD(wParam);
		//if ((HWND)lParam == przycisk1)
		if (wmId == Otworz)
		{
			
			//case 7:
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFilter = TEXT("Pliki graficzne (*.bmp)\0*.bmp\0Wszystkie pliki\0*.*\0");
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFile = LPSTR(sNazwaPliku);
			ofn.lpstrFile[0] = NULL;
			ofn.lpstrDefExt = TEXT("bmp");
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			cout << sNazwaPliku;
			if (GetOpenFileName(&ofn))
			{
				cout << "\nOtworzono plik\n";
				HDC hdc;
				PAINTSTRUCT ps;
				hdc = BeginPaint(hwnd, &ps);
				HBITMAP hbmObraz;
				hbmObraz = (HBITMAP)LoadImage(NULL, sNazwaPliku, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
				HDC hdcNowy = CreateCompatibleDC(hdc);
				SelectObject(hdcNowy, hbmObraz);

				HBITMAP hbmOld = (HBITMAP)SelectObject(hdcNowy, hbmObraz);

				BITMAP bmInfo;
				GetObject(hbmObraz, sizeof(bmInfo), &bmInfo);
				hdc = GetDC(hwnd);
				BitBlt(hdc, 500, 350, bmInfo.bmWidth, bmInfo.bmHeight, hdcNowy, 0, 0, SRCCOPY);


				ReleaseDC(hwnd, hdc);
				SelectObject(hdcNowy, hbmOld);
				DeleteDC(hdcNowy);
			}
			else
				cout << "Nie otworzono pliku\n";
		}
		if (wmId == Start)
		{


			host_vector<float> jedn;

			int counter = 0;

			for (int i = 0; i < 200; i++)
			{
				for (int k = 0; k < 10; k++)
				{
					if (k == counter)jedn.push_back(1);
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

				string x = "";	//Wklejamy go do nowego pliku
				for (int z = 0; z < targets.size(); z++)
				{

					stringstream ss;
					string h;
					ss << targets[z];
					h = ss.str();
					x += h;
					if (z % 10 != 0 || z == 0)
						x += ",";
					if (z % 10 == 0 && z != 0)
					{
						x += "\n";

						plik.write(x.c_str(), x.length());
						x = "";
					}


				}
				x = "";
				for (int z = 0; z < matrixValues.size(); z++)
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
				for (int z = 0; z < matrixValuesX.size(); z++)
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




			Matrix X = Matrix(ile_wczyt, 400, matrixValues);
			Matrix Y = Matrix(ile_wczyt, 10, targets);

			vector<int> size = { info.liczba_neuronow, 10 };

			vector<float> layerA = { float(-1 / sqrt(ile_wczyt)),  float(1 / sqrt(ile_wczyt)) };
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
			net.SetEpochs(info.liczba_iteracji);
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
			for (int k = 0; k < 400; k++)
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
			for (int k = 0; k < 400; k++)
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

			for (int i = 0; i < result2.GetRows(); i++)
			{
				wynik = 0;
				wartosc = u[i*result2.GetColumns()];
				for (int y = 1; y < 10; y++)
				{
					if (u[i*result2.GetColumns() + y] > wartosc)
					{

						wartosc = u[i*result2.GetColumns() + y];
						wynik = y;
					}
				}
				res.push_back(wynik);
			//	cout << "WYNIK przewidywany :" << wynik << endl;
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
				//cout << "WYNIK prawdziwy:" << wynik << endl;
			}


			int good = 0;
			int all = result2.GetRows();

			for (int i = 0; i < res.size(); i++)
			{
				if (res[i] == tar[i]) good++;
			}

			float acc = float(100) * float(good) / float(all);
			cout << "Wynik w %: " << acc << endl;


			stringstream konw_wynik;
			string wynikowy_string;
			konw_wynik << wynik;
			wynikowy_string = konw_wynik.str();


			SetWindowText(Text1, TEXT(wynikowy_string.c_str()));
		}

		break;

		/* Upon destruction, tell the main thread to stop */
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_TIMER:
		
		SendMessage(hProgressBar, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 10));
		SendMessage(hProgressBar, PBM_SETPOS, (WPARAM)iteracje, 0);
		//SendMessage(hProgressBar, PBM_DELTAPOS, (WPARAM)5, 0); // zwiêksza wartoœæ o 5
		break;


	/* All other messages (a lot of them) are processed using default procedures */
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);

		return 0;
	}
}

void a()
{
	HINSTANCE hInstance = 0;
	HINSTANCE hPrevInstance = 0;
	LPSTR lpCmdLine = 0;
	int nCmdShow = 0;
	WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

}


/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

			 /* zero out the struct and set the stuff we want to modify */
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc; /* This is where we will send messages to */
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "WindowClass", "Sieci Neuronowe", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		800, /* width */
		600, /* height */
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}


	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);
	HBITMAP hbmObraz;
	hbmObraz = (HBITMAP)LoadImage(NULL, "Siecn.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HDC hdcNowy = CreateCompatibleDC(hdc);
	SelectObject(hdcNowy, hbmObraz);





	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcNowy, hbmObraz);

	BITMAP bmInfo;
	GetObject(hbmObraz, sizeof(bmInfo), &bmInfo);
	hdc = GetDC(hwnd);
	BitBlt(hdc, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, hdcNowy, 0, 0, SRCCOPY);


#pragma region Obiekty



	//Text6 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE,
	//	360, 400, 50, 30, hwnd, (HMENU)6, hInstance, NULL);
	//SetWindowText(Text6, "wwww");
	//Text7 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
	//	500, 0, 700, 600, hwnd, (HMENU)6, hInstance, NULL);

	TextOut(hdc, 0, 410, TEXT("Postep:"), 10);
	TextOut(hdc, 0, 440, TEXT("Liczba_neuronow->"), 20);
	TextOut(hdc, 0, 500, TEXT("Liczba_warstw------>"), 22);
	TextOut(hdc, 0, 470, TEXT("Liczba_epoch-------->"), 22);
	TextOut(hdc, 0, 530, TEXT("leleleleleleXD-------->"), 23);
	TextOut(hdc, 550, 300, TEXT("Wybrana bitmapa"), 20);
	TextOut(hdc, 550, 150, TEXT("Wynik dzialnia sieci->"), 25);

	EndPaint(hwnd, &ps);

	przycisk1 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Otworz plik"), WS_CHILD | WS_VISIBLE,
		550, 65, 150, 30, hwnd, (HMENU)Otworz, hInstance, NULL);
	przycisk2 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Start"), WS_CHILD | WS_VISIBLE,
		550, 30, 150, 30, hwnd, (HMENU)Start, hInstance, NULL);
	Box = CreateWindowEx(WS_EX_CLIENTEDGE, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		CBS_DROPDOWN, 550, 100, 152, 200, hwnd, NULL, hInstance, NULL);
	SendMessage(Box, CB_ADDSTRING, 0, (LPARAM) "Nauka Sieci");
	SendMessage(Box, CB_ADDSTRING, 1, (LPARAM) "Uruchom");
	SendMessage(Box, CB_SETCURSEL, 0, (LPARAM) "Nauka Sieci");

	//przycisk3 = CreateWindowEx(0, TEXT("BS_GROUPBOX"), TEXT("Start"), WS_CHILD | WS_VISIBLE,
	//	0, 400, 500, 600, hwnd, (HMENU)7, hInstance, NULL);

	Text1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
		700, 150, 30, 20, hwnd, (HMENU)1, hInstance, NULL);
	Text2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
		130, 440, 30, 20, hwnd, (HMENU)2, hInstance, NULL);
	Text3 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
		130, 470, 30, 20, hwnd, (HMENU)3, hInstance, NULL);
	Text4 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
		130, 500, 30, 20, hwnd, (HMENU)4, hInstance, NULL);
	Text5 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | SS_LEFT,
		130, 530, 30, 20, hwnd, (HMENU)5, hInstance, NULL);



	//SetWindowText(Text1, "Postep:");

	hProgressBar = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
		60, 410, 270, 20, hwnd, (HMENU)200, hInstance, NULL);


	ReleaseDC(hwnd, hdc);
	SelectObject(hdcNowy, hbmOld);
	DeleteDC(hdcNowy);
	

	if (SetTimer(hwnd, 100, 500, NULL) == 0)
		MessageBox(hwnd, TEXT("timer nie zadzialal"), 0, MB_ICONSTOP);
#pragma endregion

	while (GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}




