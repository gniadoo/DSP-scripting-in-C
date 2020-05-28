#include <dsplib.h>
#include <math.h>


#define NUM_SAMPLES 5000 // rozmiar tablicy
#define M_PI 3.14159265358979323846 // warto�c liczby pi
int samples[NUM_SAMPLES]; // tablica na pr�bki
int sinus_samp[NUM_SAMPLES]; // tablica na pr�bki funkcji sinus
int d = 137; // round(f*1.36533) krok amplitudy

// ZAD 1
void saw(int* buffer, unsigned int buflen, int d){
	static int akum = 0; // deklaracja akumulatora i jego pocz�tkowej warto�ci
	int i = 0; // numer obliczanej pr�bki
	while(i<buflen){

		buffer[i]= akum; // przypisanie pr�bce warto�ci akumulatora
		akum = akum + d; // zwi�kszenie warto�ci akumulatora o krok amplitudy
		i++; // zwi�kszenie numeru pr�bki
	}
}

// ZAD 2
void rect(int* buffer, unsigned int buflen, int d, int prog){
	static int akum = 0; // deklaracja akumulatora i jego pocz�tkowej warto�ci
	int i = 0; // numer obliczanej pr�bki
	while(i<buflen){
		akum = akum + d; // zwi�kszenie warto�ci akumulatora o krok amplitudy
		if(akum > prog){
			buffer[i]= 32767; // przypisanie pr�bce warto�ci maksymalnej
		}
		else{
			buffer[i] = -32768; // przypisanie pr�bce warto�ci minimalnej
		}
			i++; // zwi�kszenie numeru pr�bki
		}
}

// ZAD 3
void tri(int* buffer, unsigned int buflen, int d){
	static int akum = 0; // deklaracja akumulatora i jego pocz�tkowej warto�ci
	int i = 0; // numer obliczanej pr�bki
	while(i<buflen){
		buffer[i] = ((akum < 0 ? -akum : akum) - 16384) << 1; // obliczenie sygna�u tr�jk�tnego z pi�okszta�tnego
		akum = akum + d; // zwi�kszenie warto�ci akumulatora o krok amplitudy
		i++; // zwi�kszenie numeru pr�bki
	}
}

// ZAD 4
void sint(int* buffer, unsigned int buflen, int d){
	static int akum = 0; // deklaracja akumulatora i jego pocz�tkowej warto�ci
	static int pom = 0;	// deklaracja zmiennej pomocniczej i jej pocz�tkowej warto�ci (symbolizuje  oryginaln� warto�� k�ta fazowego)

	long a1 = round(M_PI*4096);								//deklaracja sta�ych warto�ci wsp�czynnik�w szeregu
	long a3 = round((-pow(M_PI,3)/(1*2*3))*4096);			//
	long a5 = round((pow(M_PI,5)/(1*2*3*4*5))*4096);		//
	long a7 = round((-pow(M_PI,7)/(1*2*3*4*5*6*7))*4096);	//

	int i = 0; // numer obliczanej pr�bki
	while(i<buflen){

		if((akum = akum < 0 ? -akum : akum) > 16384){ // sprawdzenie czy warto�� k�ta fazowego nale�y do I �wiartki
			akum = 32768 - akum; // je�li nie to zamie� warto�� na podan�
		}

		int x2 = _smpy(akum, akum); // obliczone pot�gi argumentu
		int x3 = _smpy(x2, akum);	//
		int x5 = _smpy(x2, x3);		//
		int x7 = _smpy(x2, x5);		//

		long y = a1*akum + a3*x3 + a5*x5 + a7*x7; // warto�� wyra�enia przez konwersj�
		y = (y + (1<<11)) >> 12; // konwersja z formatu Q27 na Q15 oraz na typ int
		y = (int) y; 			 //

		if(pom < 0){		//
			buffer[i] = -y;	// odwr�cenie znaku wyniku jeli oryginalna warto�� k�ta
		}					// fazowego by�a ujemna
		else{				//
			buffer[i] = y;	//
		}					//
		pom = pom + d; // zwi�kszenie warto�ci zmiennej pomocniczej o krok amplitudy
		akum = pom; // przypisanie akumulatorowi warto�ci zmiennej pomocniczej
		i++; // zwi�kszenie numeru pr�bki
	}
}

void main(void) {

	// INICJALIZACJA GENERATORA LICZB LOSOWYCH
	void rand16init(void);

	// WYWO�ANIE FUNKCJI DO ZADANIA 1
	//saw(samples, NUM_SAMPLES, d);

	// WYWO�ANIE FUNKCJI DO ZADANIA 2
	//rect(samples, NUM_SAMPLES, d, 16384);

	// WYWO�ANIE FUNKCJI DO ZADANIA 3
	//tri(samples, NUM_SAMPLES, d);

	// WYWO�ANIE FUNKCJI DO ZADANIA 4
	//sint(samples, NUM_SAMPLES, d);

	// WYWO�ANIE FUNKCJI DO ZADANIA 5
	//sine((DATA*)samples,(DATA*)sinus_samp, NUM_SAMPLES);

	// WYWO�ANIE FUNKCJI DO ZADANIA 6
	//rand16((DATA*) samples, NUM_SAMPLES);

	while (1); // do not exit
}
