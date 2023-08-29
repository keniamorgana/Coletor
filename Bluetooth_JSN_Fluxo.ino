// Programa: Sensor JSN - Envio de dados via Bluetooth
// Autor: Kênia Morgana de Vasconcelos Saraceni

// DEFINIÇÕES DE PINOS
#define pinTrigger 13
#define pinEcho 12
#define pinFluxo 2

// DECLARAÇÃO DE FUNÇÕES
float leituraSimples();
float calcularDistancia();
void sonarBegin(byte trig ,byte echo);
void fluxoBegin();
float calcularFluxo();
void Vazao();

// DECLARAÇÃO DE VARIÁVEIS
float distancia;
double calculoVazao;
volatile int contador;
float fluxoAcumulado = 0;
float metroCubico = 0;
float ContaAgua = 0;
float fluxo;
 
void setup()
{
  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");
  sonarBegin(pinTrigger,pinEcho);
  void fluxoBegin();
}

void loop() {

  distancia = calcularDistancia();
  fluxo = calcularFluxo();

  if  (distancia > 35)
  {
  Serial.println("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  delay(10);
  }
  else
  {
  Serial.println("Desligar coletor");
  }

  Serial.println("Litros por minuto:");
  Serial.println(calculoVazao);
  Serial.println("Gasto total em L:");
  Serial.println(fluxoAcumulado);
  Serial.println("Metros cubicos totais:");
  Serial.println(metroCubico);
  Serial.println("R$:");
  Serial.println(ContaAgua);

}

// IMPLEMENTO DE FUNÇÕES

void sonarBegin(byte trig ,byte echo) {

  #define divisor 58.0
  #define intervaloMedida 5 // MÁXIMO 35 mS PARA ATÉ 6,0M MIN 5mS PARA ATÉ 0,8M
  #define qtdMedidas 20 // QUANTIDADE DE MEDIDAS QUE SERÃO FEITAS
  
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT);
  
  digitalWrite(trig, LOW); // DESLIGA O TRIGGER E ESPERA 500 uS
  delayMicroseconds(500);

}

float calcularDistancia() {

  float leituraSum = 0;
  float resultado = 0;
  
  for (int index = 0; index < qtdMedidas; index++) {

    delay(intervaloMedida);
    leituraSum += leituraSimples();

  }

  resultado = (float) leituraSum / qtdMedidas;
  resultado = resultado + 2.2;
  
  return resultado;

}

float leituraSimples() {

  long duracao = 0; 
  float resultado = 0;
  
  digitalWrite(pinTrigger, HIGH); 

  delayMicroseconds(10); 
  digitalWrite(pinTrigger, LOW);

  duracao = pulseIn(pinEcho, HIGH);

  resultado = ((float) duracao / divisor);
  
  return resultado;

}

void fluxoBegin()
{
  pinMode(2,INPUT);
  attachInterrupt(0,Vazao,RISING);
}

float calcularFluxo()
{
  contador = 0;
  interrupts();
  delay(1000);
  noInterrupts();

  calculoVazao = (contador * 2.25);
  fluxoAcumulado = fluxoAcumulado + (calculoVazao / 1000);
  metroCubico = fluxoAcumulado / 1000;
  ContaAgua = metroCubico *3,59;
  calculoVazao = calculoVazao * 60;
  calculoVazao = calculoVazao / 1000;
}

void Vazao()
{
  contador ++;
}