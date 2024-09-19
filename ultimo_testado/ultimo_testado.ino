// Configurações dos pinos
const int latchPin = 10;  // Pino de latch do 74HC595
const int clockPin = 11;  // Pino de clock do 74HC595
const int dataPin = 9;    // Pino de dados do 74HC595
const int buttonPin = 3;  // Pino do botão
const int buzzerPin = 13; // Pino do buzzer

// Cores do LED RGB
const int redPin = 5;     // Pino do vermelho do LED RGB
const int greenPin = 7;   // Pino do verde do LED RGB
const int bluePin = 6;    // Pino do azul do LED RGB

// Pinos dos LEDs conectados diretamente ao Arduino
const int ledDirectPins[4] = {1, 2, 8, 12};

// Estado do jogo
int currentLED = 0;         // LED atual que está aceso
int gameSpeed = 1000;       // Intervalo inicial entre transições (em milissegundos)
bool gameActive = true;     // Estado do jogo

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Configura o botão com pull-up interno
  pinMode(buzzerPin, OUTPUT);

  // Configura os pinos do LED RGB
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Configura os pinos dos LEDs conectados diretamente
  for (int i = 0; i < 4; i++) {
    pinMode(ledDirectPins[i], OUTPUT);
  }

  // Inicializa o LED RGB como roxo
  setRGBColor(255, 0, 255); // Roxo

  // Configura o estado inicial dos LEDs
  updateLEDs();
  updateDirectLEDs();
}

void loop() {
  if (gameActive) {
    // Transição dos LEDs
    delay(gameSpeed); // Ajusta o atraso para a velocidade do jogo
    currentLED = (currentLED + 1) % 13; // Passa para o próximo LED (8 do 74HC595 + 4 diretos + 1 RGB)

    // Atualiza os LEDs e o buzzer
    if (currentLED < 8) {
      updateLEDs(); // Atualiza LEDs do 74HC595
    } else if (currentLED < 12) {
      updateDirectLEDs(); // Atualiza LEDs diretos
    }

    tone(buzzerPin, 1000, 100); // Toca um som curto no buzzer

    // Verifica se o botão foi pressionado
    if (digitalRead(buttonPin) == LOW) {
      if (currentLED == 12) {  // Se o LED RGB for o único aceso
        // Jogador acertou
        playWinMusic();
        increaseGameSpeed();  // Aumenta a velocidade do jogo
      } else {
        // Jogador errou
        playLoseMusic();
      }
      delay(5000); // Aguarda 5 segundos antes de reiniciar
      resetGame();
    }
  } else {
    resetGame();
  }
}

// Atualiza os LEDs usando o CI 74HC595
void updateLEDs() {
  digitalWrite(latchPin, LOW);
  // Se o LED RGB for o único aceso, apaga todos os LEDs do 74HC595
  shiftOut(dataPin, clockPin, MSBFIRST, (currentLED < 8) ? (1 << currentLED) : 0);
  digitalWrite(latchPin, HIGH);

  // Mantém o LED RGB aceso
  setRGBColor(255, 0, 255); // Roxo
}

// Atualiza os LEDs conectados diretamente ao Arduino
void updateDirectLEDs() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledDirectPins[i], (currentLED >= 8 && currentLED < 12) && (currentLED - 8 == i) ? HIGH : LOW);
  }

  // Se o LED RGB for o único aceso
  if (currentLED == 12) {
    setRGBColor(255, 0, 255); // Apenas o RGB aceso
  }
}

// Define a cor do LED RGB
void setRGBColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

// Toca a música quando o jogador ganha
void playWinMusic() {
  tone(buzzerPin, 2000, 500); // Exemplo de tom
}

// Toca a música quando o jogador erra
void playLoseMusic() {
  tone(buzzerPin, 500, 500); // Exemplo de tom
}

// Aumenta a velocidade do jogo
void increaseGameSpeed() {
  if (gameSpeed > 200) { // Limita a velocidade mínima
    gameSpeed -= 100; // Diminui o intervalo entre transições
  }
}

// Reinicia o jogo
void resetGame() {
  currentLED = 0;
  setRGBColor(255, 0, 255); // Roxo
  gameSpeed = 1000; // Reset da velocidade do jogo
  gameActive = true;
  updateLEDs();
  updateDirectLEDs();
  delay(1000); // Atraso antes de iniciar o próximo jogo
}
