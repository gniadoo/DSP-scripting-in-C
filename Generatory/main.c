#include <dsplib.h>
#include <math.h>


#define NUM_SAMPLES 5000 // rozmiar tablicy
#define M_PI 3.14159265358979323846 // wartoœc liczby pi
int samples[NUM_SAMPLES]; // tablica na próbki
int sinus_samp[NUM_SAMPLES]; // tablica na próbki funkcji sinus
int d = 137; // round(f*1.36533) krok amplitudy

// ZAD 1
void saw(int* buffer, unsigned int buflen, int d){
	static int akum = 0; // deklaracja akumulatora i jego pocz¹tkowej wartoœci
	int i = 0; // numer obliczanej próbki
	while(i<buflen){

		buffer[i]= akum; // przypisanie próbce wartoœci akumulatora
		akum = akum + d; // zwiêkszenie wartoœci akumulatora o krok amplitudy
		i++; // zwiêkszenie numeru próbki
	}
}

// ZAD 2
void rect(int* buffer, unsigned int buflen, int d, int prog){
	static int akum = 0; // deklaracja akumulatora i jego pocz¹tkowej wartoœci
	int i = 0; // numer obliczanej próbki
	while(i<buflen){
		akum = akum + d; // zwiêkszenie wartoœci akumulatora o krok amplitudy
		if(akum > prog){
			buffer[i]= 32767; // przypisanie próbce wartoœci maksymalnej
		}
		else{
			buffer[i] = -32768; // przypisanie próbce wartoœci minimalnej
		}
			i++; // zwiêkszenie numeru próbki
		}
}

// ZAD 3
void tri(int* buffer, unsigned int buflen, int d){
	static int akum = 0; // deklaracja akumulatora i jego pocz¹tkowej wartoœci
	int i = 0; // numer obliczanej próbki
	while(i<buflen){
		buffer[i] = ((akum < 0 ? -akum : akum) - 16384) << 1; // obliczenie sygna³u trójk¹tnego z pi³okszta³tnego
		akum = akum + d; // zwiêkszenie wartoœci akumulatora o krok amplitudy
		i++; // zwiêkszenie numeru próbki
	}
}

// ZAD 4
void sint(int* buffer, unsigned int buflen, int d){
	static int akum = 0; // deklaracja akumulatora i jego pocz¹tkowej wartoœci
	static int pom = 0;	// deklaracja zmiennej pomocniczej i jej pocz¹tkowej wartoœci (symbolizuje  oryginaln¹ wartoœæ k¹ta fazowego)

	long a1 = round(M_PI*4096);								//deklaracja sta³ych wartoœci wspó³czynników szeregu
	long a3 = round((-pow(M_PI,3)/(1*2*3))*4096);			//
	long a5 = round((pow(M_PI,5)/(1*2*3*4*5))*4096);		//
	long a7 = round((-pow(M_PI,7)/(1*2*3*4*5*6*7))*4096);	//

	int i = 0; // numer obliczanej próbki
	while(i<buflen){

		if((akum = akum < 0 ? -akum : akum) > 16384){ // sprawdzenie czy wartoœæ k¹ta fazowego nale¿y do I æwiartki
			akum = 32768 - akum; // jeœli nie to zamieñ wartoœæ na podan¹
		}

		int x2 = _smpy(akum, akum); // obliczone potêgi argumentu
		int x3 = _smpy(x2, akum);	//
		int x5 = _smpy(x2, x3);		//
		int x7 = _smpy(x2, x5);		//

		long y = a1*akum + a3*x3 + a5*x5 + a7*x7; // wartoœæ wyra¿enia przez konwersj¹
		y = (y + (1<<11)) >> 12; // konwersja z formatu Q27 na Q15 oraz na typ int
		y = (int) y; 			 //

		if(pom < 0){		//
			buffer[i] = -y;	// odwrócenie znaku wyniku jeli oryginalna wartoœæ k¹ta
		}					// fazowego by³a ujemna
		else{				//
			buffer[i] = y;	//
		}					//
		pom = pom + d; // zwiêkszenie wartoœci zmiennej pomocniczej o krok amplitudy
		akum = pom; // przypisanie akumulatorowi wartoœci zmiennej pomocniczej
		i++; // zwiêkszenie numeru próbki
	}
}

void main(void) {

	// INICJALIZACJA GENERATORA LICZB LOSOWYCH
	void rand16init(void);

	// WYWO£ANIE FUNKCJI DO ZADANIA 1
	//saw(samples, NUM_SAMPLES, d);

	// WYWO£ANIE FUNKCJI DO ZADANIA 2
	//rect(samples, NUM_SAMPLES, d, 16384);

	// WYWO£ANIE FUNKCJI DO ZADANIA 3
	//tri(samples, NUM_SAMPLES, d);

	// WYWO£ANIE FUNKCJI DO ZADANIA 4
	//sint(samples, NUM_SAMPLES, d);

	// WYWO£ANIE FUNKCJI DO ZADANIA 5
	//sine((DATA*)samples,(DATA*)sinus_samp, NUM_SAMPLES);

	// WYWO£ANIE FUNKCJI DO ZADANIA 6
	//rand16((DATA*) samples, NUM_SAMPLES);

	while (1); // do not exit
}
