#include <dsplib.h>

#define NUM_SAMPLES 5000 //rozmiar tablicy przechowuj¹cej probki
#define NUM_COEF 57 //rozmiar tablicy przechowujacej wspolczynniki
int samples[NUM_SAMPLES]; //tablica przechowujaca probki wejsciowe
int result[NUM_SAMPLES]; //tablica przechowujaca probki wyjsciowe

//tablica przechowujaca wspolczynniki filtru
const int coef[] = {3,0,-7,-18,-34,-56,-81,-109,-136,-160,-175,-176,-158,-117,-48,
		53,186,351,546,766,1003,1249,1494,1727,1935,2110,2242,2324,2352,2324,
		2242,2110,1935,1727,1494,1249,1003,766,546,351,186,53,-48,-117,-158,
		-176,-175,-160,-136,-109,-81,-56,-34,-18,-7,0,3};

//implementacja funkcji blocfir
void blockfir(int* input, const int* filter, int* output, int numSamples, int numFilter){
	int i,j; //zmienne pomocnicze do petli
	long sample_value; //zmienna przechowujaca wartosc aktualne przetwarzanej probki
	for(i=0; i<numSamples; i++){ //petla przechodzaca po wszystkich probkach
		sample_value = 0;
		for(j=0; j<numFilter; j++){ //petla przechodzaca po wszystkich wspolczynnikach
			if(i<j)
				break;
			sample_value = _smaci(sample_value, input[i-j], filter[j]);//obliczanie wartosci wyjsciowej przetwarzanej probki z uzyciem instrukcji MAC
		}
		sample_value = (int)(_sround(sample_value) >> 15);//konwersja do Q15 wykonuj¹c przesuniecie bitowe
		output[i] = sample_value;//zapisanie wartosci przetwarzanej probki to tablicy przechowujacej wartosci wyjsciowe
	}
}
//funkcja generuj¹ca sygnal piloksztaltny
void saw(int* buffer, unsigned int buflen, int d){
	static int akum = 0; // deklaracja akumulatora i jego pocz¹tkowej wartoœci
	int i = 0; // numer obliczanej próbki
	while(i<buflen){

		buffer[i]= akum; // przypisanie próbce wartoœci akumulatora
		akum = akum + d; // zwiêkszenie wartoœci akumulatora o krok amplitudy
		i++; // zwiêkszenie numeru próbki
	}
}

void main(void) {

	//GENEROWANIE SZUMU BIALEGO
	//void rand16init(void);
	//rand16((DATA*) samples, NUM_SAMPLES);

	//GENEROWANIE PILY
	saw(samples, NUM_SAMPLES, 137);

	//FILTRACJA Z UZYCIEM ZAIMPLEMENTOWANEGO FILTRU
	blockfir(samples, coef, samples, NUM_SAMPLES, NUM_COEF);

	//FILTRACJA Z UZYCIEM DSPLIB
	//int buf[59] = {0};
	//fir((DATA*) samples, (DATA*) coef, (DATA*) result, (DATA*) buf, 5000, 57);

	//FILTRACJA PROBKA PO PROBCE
	//int i;
	//int buf[59] = {0};
	//for(i = 0; i<NUM_SAMPLES; i++){
		//fir((DATA*) &samples[i], (DATA*) coef, (DATA*) &samples[i], (DATA*) buf, 1, 57);
	//}

	while (1); // do not exit
}
