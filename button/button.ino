const int buttonPin = 2; // Pino onde o botão está conectado
int buttonState = 0;     // Variável para armazenar o estado do botão

void setup() {
  // Configura o pino do botão como entrada com pull-up interno ativado
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Inicia a comunicação serial
  Serial.begin(9600);
}

void loop() {
  // Lê o estado do botão
  buttonState = digitalRead(buttonPin);

  // Verifica o estado do botão
  if (buttonState == LOW) {
    // Se o botão for pressionado (estado LOW)
    Serial.println("Botão pressionado");
  } else {
    // Se o botão não for pressionado (estado HIGH)
    Serial.println("Botão não pressionado");
  }

  // Atraso de 200ms para evitar mensagens rápidas demais no monitor serial
  delay(200);
}
