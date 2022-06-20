#include <Arduino.h>

// pinos utilizados
#define A0 2
#define D0 4
// variáveis globais
float freq = 0;
int let_PA = 0, let_PD = 0;
int cnt_zr = 0, cnt_pc = 0;
long int tempoI = 0, tempoF = 0, tempoR = 0;
// declaração de funções
float calc_freq();


void setup() {
  //Configura portas do SW18010P
  pinMode(A0, INPUT);
  pinMode(D0, INPUT);
  Serial.begin(9600); 
}

void loop() {
  //Leitura dos valores e apresentação na serial
  freq = calc_freq();
  delay(5);
  Serial.print("\nFrequencia: ");
  Serial.println(freq);
}

float calc_freq() {
  //Calcula a frequencia do sinal
  float fr = 0, med = 0;
  tempoI = millis();
  while(tempoR < 250){ //verifica média de picos em 1s
    tempoR = millis() - tempoI;
    let_PA = digitalRead(A0);
    Serial.println("Let_PA: "+let_PA);
    if(let_PA > 800){
      cnt_pc++;
    } else{
      cnt_zr++;
    }
  }
  //Testes Serial
  Serial.print("\nContagem de picos: ");
  Serial.print(cnt_pc);
  Serial.print("\nContagem de zeros: ");
  Serial.println(cnt_zr);
  //Calcula a frequencia
  if(cnt_pc > cnt_zr && cnt_pc > 0 && cnt_zr > 0){
    med = 1000/cnt_zr;
    fr = 1/(med/1000);
  } else if(cnt_pc < cnt_zr && cnt_pc > 0 && cnt_zr > 0){
    med = 1000/cnt_pc;
    fr = 1/(med/1000);
  } else{
    fr = 0;
  }
  return fr;
}