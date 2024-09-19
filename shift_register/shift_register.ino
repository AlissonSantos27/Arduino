// Pinos de conexão com o 74HC595
const int dataPin = 2;   // Pino conectado ao pino DS (SER) do 74HC595
const int latchPin = 3;  // Pino conectado ao pino ST_CP (RCLK) do 74HC595
const int clockPin = 4;  // Pino conectado ao pino SH_CP (SRCLK) do 74HC595

void setup() {
  // Define os pinos como saídas
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void loop() {
  // Acende os LEDs um por um
  for (int i = 0; i < 8; i++) {
    // Envia o bit correspondente ao LED que será aceso
    digitalWrite(latchPin, LOW);                // Prepara para enviar dados
    shiftOut(dataPin, clockPin, MSBFIRST, 1 << i); // Envia o bit deslocado
    digitalWrite(latchPin, HIGH);               // Atualiza os LEDs
    delay(500);                                 // Aguarda meio segundo
  }
  
  // Apaga os LEDs um por um
  for (int i = 0; i < 8; i++) {
    digitalWrite(latchPin, LOW);                // Prepara para enviar dados
    shiftOut(dataPin, clockPin, MSBFIRST, ~(1 << i)); // Envia o bit invertido
    digitalWrite(latchPin, HIGH);               // Atualiza os LEDs
    delay(500);                                 // Aguarda meio segundo
  }
}
