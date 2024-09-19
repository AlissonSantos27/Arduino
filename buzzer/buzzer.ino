const int buzzerPin = 8; // Pino onde o buzzer passivo está conectado

void setup() {
  // Configura o pino do buzzer como saída
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Toca uma frequência no buzzer (1000 Hz, por exemplo)
  tone(buzzerPin, 1000); // Gera uma frequência de 1000 Hz
  delay(1000);           // Aguarda 1 segundo

  // Para de tocar
  noTone(buzzerPin);     // Para o som
  delay(1000);           // Aguarda 1 segundo
}
