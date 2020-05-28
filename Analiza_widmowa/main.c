#include <dsplib.h>
#include <testsignal.h> //za��czenie tablicy sygna�u klarnetu
#include <hamming.h> //za��czenie tablicy zawieraj�cej wsp�czynniki okna
#define NX 2048 //zdefiniowanie rozmiaru tablicy
int autocor[NX]; //tablica przechowuj�ca wsp�czynniki przesuni�te bitowo na potrzeby funkcji autokorelacji
int result[NX]; //tablica przechowuj�ca wsp�czynniki autokorelacji
int derivative[513]; //tablica przechowuj�ca wartosci pochodnej

//Funkcja obliczaj�ca widmo amplitudowe z widma zespolonego
void amp(DATA *buffer, int buflen){

	int k = 0;
	int j = 2;

	for(; j < buflen; j=j+2){ //p�tla iteruj�ca po wszystkich elementach tablicy
		short r = buffer[j]; //zapisanie wartosci rzeczywistej
		short i = buffer[j+1]; //zapisanie wartosci urojonej
		short r2 = _smpy(r,r); //obliczenie kwadratu cz�ci rzeczywistej
		short i2 = _smpy(i,i); //obliczenie kwadratu cz�ci urojonej
		short result = r2 + i2; //sumowanie poprzednio obliczonych wartosci
		buffer[k] = result; //nadpisanie tablicy poprzednio obliczon� sum�
		k = k+1; //przejscie na kolejn� pr�bk�
	}
	sqrt_16((DATA*) buffer, (DATA*) buffer, 1022); //obliczenie modu�u liczby zespolonej - powstaje widmo amplitudowe
}

//Funkcja przemna�aj�ca pr�bki sygna�y przez wsp�czynniki okna
void okno(DATA *buffer, int buflen){

		int y = hamming[0]; //zainicjowanie tablicy z wsp�czynnikami okna Hamminga
		int i = 0;
		for (i; i < buflen; i++) { //p�tla iteruj�ca po wszystkich elementach tablicy
			buffer[i] = _smpy(buffer[i], hamming[i]); //przemno�enie kolejnych pr�bek przez wsp�czynniki okna
		}
}

//Funkcja wyszukuj�ca maksimum funkcji widma
int max(DATA *buffer, int* derivative, int buflen){

	int i = 0;
	int m = 0; //pocz�tkowy indeks pr�bki maksimum
	int val = 0; //pocz�tkowa wartos� pr�bki maksimum

	for(i; i < buflen; i++){ //p�tla iteruj�ca po wszystkich elementach tablicy obliczaj�ca wartosci pochodnej
		if(buffer[i] < 430 && buffer[i+1] < 430){ //sprawdzenie czy s�siednie pr�bki nie nale�� do poziomu pod�ogi
			derivative[i] = 0;					  //szumowej, kt�rej warto� ustalono na 430, jesli nale��, to wartos�
		}										  //pochodnej tej pr�bki jest r�wna 0
		else {
			derivative[i] = buffer[i+1] - buffer[i];//je�eli chocia� jedna ma amplitude wi�ksz� ni� poziom pod�ogi,
		}											//to obliczana jest wartosc pochodnej
		if(i != 0){ //pomini�cie sprawdzania pr�bki o indeksie 0
			if(derivative[i-1] > 0 && derivative[i] < 0){ //sprawdzenie czy wyst�puje zmiana znaku pomi�dzy sasiednimi pr�bkami
				if(buffer[i] > val){ //sprawdzenie czy dotychczasowa wartos� maksimum jest mniejsza od wartosci aktualnie przetwarzanej pr�bki
					m = i;			//jeli jest to nadpisz indeks maksimum
					val = buffer[i];//i jego wartosc
				}
			}

		}
	}
	return m; //zwracanie wartosci indeksu znalezionego maksimum
}

//Funkcja obliczaj�ca cz�stotliwo�� sk�adowej widmowej dla podanego indeksu
int maxf(int m){

	int coef = 48000/2048;
	return coef * m;

}

//Funkcja wykonuj�ca dzielenie pr�bek tablicy przez 16 na potrzeby funkcji autokorelacji
void copytoauto(DATA *buffer,DATA *bufferout, int buflen){

	int i;
	for(i=0; i<buflen; i++){ //p�tla iteruj�ca po wszystkich elementach tablicy
		bufferout[i]= (buffer[i] >> 4); //wykonane pzesuni�cie bitowe by unikn�� przepe�nienia zakresu
	}
}

//Funkcja wyszukuj�ca maksimum funkcji autokorelacji
int max_index_auto(DATA *buffer, int buflen){

	int i = 0;
	int val = 0; //pocz�tkowa wartos� pr�bki maksimum
	int m = 0; //pocz�tkowy indeks pr�bki maksimum
	int derivative[1000]; //tablica przechowuj�ca wartoci pochodnej

	for(i; i < buflen; i++) { //p�tla iteruj�ca po wszystkich elementach tablicy
		derivative[i] = buffer[i+1] - buffer[i]; //obliczanie wartosci pochodnej dla danej pr�bki
		if(i!=0){ //pomini�cie sprawdzania pr�bki o indeksie 0
			if(derivative[i-2] > 0 && derivative[i] < 0){ //sprawdzenie zmiany znaku dla dw�ch pr�bek uwzgl�dniaj�c p�askie maksimum wystepuj�ce mi�dzy nimi
				if(buffer[i] > val){ //sprawdzenie czy dotychczasowa wartos� maksimum jest mniejsza od wartosci aktualnie przetwarzanej pr�bki
					val = buffer[i];//jeli jest to nadpisz wartosc maksimum
					m = i;			//i jego indeks
				}
			}
		}

	}
	return m; //zwracanie wartosci indeksu znalezionego maksimum
}

void main(void) {

//int x = testsignal[0];									// ZAD 1

//rfft((DATA*) testsignal, 2048, SCALE); 					// ZAD 2
//amp((DATA*) testsignal, 2048);		 					//


//okno((DATA*) testsignal, 2048);							//
//rfft((DATA*) testsignal, 2048, SCALE);					// ZAD 3
//amp((DATA*) testsignal, 2048);							//

//int max_index;											//
//int max_f;												//
//rfft((DATA*) testsignal, 2048, SCALE);					// ZAD 4
//amp((DATA*) testsignal, 2048);							//
//max_index = max((DATA*) testsignal, derivative, 512);		//
//max_f = maxf(max_index);									//

//int max_ind_cor;
//copytoauto((DATA*) testsignal,(DATA*) autocor, 2048);		//
//acorr((DATA*) autocor, (DATA*) result, 1000, 1000, bias);	// ZAD 5
//max_ind_cor = max_index_auto((DATA*) result, 1000);		//

	while (1); // do not exit
}
