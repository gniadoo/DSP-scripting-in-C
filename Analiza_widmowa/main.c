#include <dsplib.h>
#include <testsignal.h> //za³¹czenie tablicy sygna³u klarnetu
#include <hamming.h> //za³¹czenie tablicy zawieraj¹cej wspó³czynniki okna
#define NX 2048 //zdefiniowanie rozmiaru tablicy
int autocor[NX]; //tablica przechowuj¹ca wspó³czynniki przesuniête bitowo na potrzeby funkcji autokorelacji
int result[NX]; //tablica przechowuj¹ca wspó³czynniki autokorelacji
int derivative[513]; //tablica przechowuj¹ca wartosci pochodnej

//Funkcja obliczaj¹ca widmo amplitudowe z widma zespolonego
void amp(DATA *buffer, int buflen){

	int k = 0;
	int j = 2;

	for(; j < buflen; j=j+2){ //pêtla iteruj¹ca po wszystkich elementach tablicy
		short r = buffer[j]; //zapisanie wartosci rzeczywistej
		short i = buffer[j+1]; //zapisanie wartosci urojonej
		short r2 = _smpy(r,r); //obliczenie kwadratu czêci rzeczywistej
		short i2 = _smpy(i,i); //obliczenie kwadratu czêci urojonej
		short result = r2 + i2; //sumowanie poprzednio obliczonych wartosci
		buffer[k] = result; //nadpisanie tablicy poprzednio obliczon¹ sum¹
		k = k+1; //przejscie na kolejn¹ próbkê
	}
	sqrt_16((DATA*) buffer, (DATA*) buffer, 1022); //obliczenie modu³u liczby zespolonej - powstaje widmo amplitudowe
}

//Funkcja przemna¿aj¹ca próbki sygna³y przez wspó³czynniki okna
void okno(DATA *buffer, int buflen){

		int y = hamming[0]; //zainicjowanie tablicy z wspó³czynnikami okna Hamminga
		int i = 0;
		for (i; i < buflen; i++) { //pêtla iteruj¹ca po wszystkich elementach tablicy
			buffer[i] = _smpy(buffer[i], hamming[i]); //przemno¿enie kolejnych próbek przez wspó³czynniki okna
		}
}

//Funkcja wyszukuj¹ca maksimum funkcji widma
int max(DATA *buffer, int* derivative, int buflen){

	int i = 0;
	int m = 0; //pocz¹tkowy indeks próbki maksimum
	int val = 0; //pocz¹tkowa wartosæ próbki maksimum

	for(i; i < buflen; i++){ //pêtla iteruj¹ca po wszystkich elementach tablicy obliczaj¹ca wartosci pochodnej
		if(buffer[i] < 430 && buffer[i+1] < 430){ //sprawdzenie czy s¹siednie próbki nie nale¿¹ do poziomu pod³ogi
			derivative[i] = 0;					  //szumowej, której wartoæ ustalono na 430, jesli nale¿¹, to wartosæ
		}										  //pochodnej tej próbki jest równa 0
		else {
			derivative[i] = buffer[i+1] - buffer[i];//je¿eli chocia¿ jedna ma amplitude wiêksz¹ ni¿ poziom pod³ogi,
		}											//to obliczana jest wartosc pochodnej
		if(i != 0){ //pominiêcie sprawdzania próbki o indeksie 0
			if(derivative[i-1] > 0 && derivative[i] < 0){ //sprawdzenie czy wystêpuje zmiana znaku pomiêdzy sasiednimi próbkami
				if(buffer[i] > val){ //sprawdzenie czy dotychczasowa wartosæ maksimum jest mniejsza od wartosci aktualnie przetwarzanej próbki
					m = i;			//jeli jest to nadpisz indeks maksimum
					val = buffer[i];//i jego wartosc
				}
			}

		}
	}
	return m; //zwracanie wartosci indeksu znalezionego maksimum
}

//Funkcja obliczaj¹ca czêstotliwoœæ sk³adowej widmowej dla podanego indeksu
int maxf(int m){

	int coef = 48000/2048;
	return coef * m;

}

//Funkcja wykonuj¹ca dzielenie próbek tablicy przez 16 na potrzeby funkcji autokorelacji
void copytoauto(DATA *buffer,DATA *bufferout, int buflen){

	int i;
	for(i=0; i<buflen; i++){ //pêtla iteruj¹ca po wszystkich elementach tablicy
		bufferout[i]= (buffer[i] >> 4); //wykonane pzesuniêcie bitowe by unikn¹æ przepe³nienia zakresu
	}
}

//Funkcja wyszukuj¹ca maksimum funkcji autokorelacji
int max_index_auto(DATA *buffer, int buflen){

	int i = 0;
	int val = 0; //pocz¹tkowa wartosæ próbki maksimum
	int m = 0; //pocz¹tkowy indeks próbki maksimum
	int derivative[1000]; //tablica przechowuj¹ca wartoci pochodnej

	for(i; i < buflen; i++) { //pêtla iteruj¹ca po wszystkich elementach tablicy
		derivative[i] = buffer[i+1] - buffer[i]; //obliczanie wartosci pochodnej dla danej próbki
		if(i!=0){ //pominiêcie sprawdzania próbki o indeksie 0
			if(derivative[i-2] > 0 && derivative[i] < 0){ //sprawdzenie zmiany znaku dla dwóch próbek uwzglêdniaj¹c p³askie maksimum wystepuj¹ce miêdzy nimi
				if(buffer[i] > val){ //sprawdzenie czy dotychczasowa wartosæ maksimum jest mniejsza od wartosci aktualnie przetwarzanej próbki
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
