#include <Arduino.h>
#include <arduinoFFT.h>

// valores definidos
#define SAMPLES 64             //Must be a power of 2
#define SAMPLING_FREQUENCY 200 //Hz, must be less than 10000 due to ADC

// pinos utilizados
#define A0 2
#define D0 4
// variáveis globais
arduinoFFT FFT = arduinoFFT();
unsigned int sampling_period_us;
unsigned long microseconds;
double freq = 0;
int let_PA = 0, let_PD = 0;
double vals[SAMPLES], img[SAMPLES];


// declaração de funções
double calc_freq();


void setup() {
  //Configura portas do SW18010P
  pinMode(A0, INPUT);
  pinMode(D0, INPUT);
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
  Serial.begin(9600); 
}

void loop() {
  //Leitura dos valores e apresentação na serial
  // popula um vetor por um periodo de 200ms

  for (int x = 0 ; x < SAMPLES ; x++){
    microseconds = micros();    //Overflows after around 70 minutes!
    vals[x] = analogRead(A0);
    img[x] = 0;
    while(micros() < (microseconds + sampling_period_us)){}
  }
  freq = calc_freq();
  Serial.print("Freq: ");
  Serial.println(freq);
  delay(100);
}

double calc_freq() {
  double fr = 0;
  FFT.Windowing(vals, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vals, img, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vals, img, SAMPLES);
  fr = FFT.MajorPeak(vals, SAMPLES, SAMPLING_FREQUENCY);
  return fr;
}