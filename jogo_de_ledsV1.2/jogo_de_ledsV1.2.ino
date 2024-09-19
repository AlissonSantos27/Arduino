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
const int ledDirectPins[4] = {12, 8, 2, 1};

// Estado do jogo
int currentLED = 0; // LED atual que está aceso
int gameSpeed = 700; // Velocidade inicial (em milissegundos) ajustada para um valor mais rápido
int level = 1;  // Nível atual do jogo
int maxLevel = 10;  // Número máximo de fases
bool gameActive = true; // Estado do jogo

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
  
  // Inicializa o LED RGB como lilás
  setRGBColor(200, 0, 200); // Lilás
  
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
    updateLEDs();
    updateDirectLEDs();
    
    tone(buzzerPin, 1000, 100); // Toca um som curto no buzzer

    // Verifica se o botão foi pressionado
    if (digitalRead(buttonPin) == LOW) {
      if (currentLED == 0) { // Se o LED 0 for o único aceso
        // Jogador acertou
        playWinMusic();
        increaseGameSpeed(); // Aumenta a velocidade do jogo gradualmente
        
      } else {
        // Jogador errou
        playLoseMusic();
      }
      delay(3000); // Aguarda 3 segundos antes de reiniciar
      gameActive = false;
    }
  } else {
    // Reinicia o jogo
    resetGame();
    delay(3000); // Atraso antes de iniciar o próximo jogo
  }
}

// Atualiza os LEDs usando o CI 74HC595
void updateLEDs() {
  byte ledState = 0;
  
  if (currentLED < 8) {
    ledState = 1 << currentLED; // Define o LED a ser aceso
  }
  
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, ledState);
  digitalWrite(latchPin, HIGH);
}

// Atualiza os LEDs conectados diretamente ao Arduino
void updateDirectLEDs() {
  for (int i = 0; i < 4; i++) {
    if (currentLED >= 8 && currentLED < 12 && (currentLED - 8 == i)) {
      digitalWrite(ledDirectPins[i], HIGH);
    } else {
      digitalWrite(ledDirectPins[i], LOW);
    }
  }
  
  // Atualiza o LED RGB
  if (currentLED == 12) {
    setRGBColor(200, 0, 200); // Lilás
  } else {
    setRGBColor(0, 0, 0); // Apaga o LED RGB
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
  // Apaga o LED 0
  byte ledState = 0;  // Apaga todos os LEDs do shift register
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, ledState);  // Desliga o LED 0
  digitalWrite(latchPin, HIGH);
  
  // Liga o LED RGB (LED 12) em lilás
  setRGBColor(200, 0, 200);
  
  tone(buzzerPin, 2000, 500); // Exemplo de tom
}

// Toca a música quando o jogador erra
void playLoseMusic() {
  tone(buzzerPin, 500, 500); // Exemplo de tom
}

// Aumenta a velocidade do jogo progressivamente ao longo de 10 fases
void increaseGameSpeed() {
  if (level < maxLevel) { 
    level++;  // Aumenta o nível do jogo
    gameSpeed = 700 - (level * 60); // Reduz a velocidade gradualmente (60 ms por nível)
  } else {
    gameSpeed = 100; // Limita a velocidade mínima para 100 ms
  }
}

// Reseta o jogo
void resetGame() {
  currentLED = 0;
  setRGBColor(200, 0, 200); // Lilás
  gameSpeed = 700; // Reset da velocidade inicial do jogo
  level = 1; // Reset do nível
  gameActive = true;
  updateLEDs();
  updateDirectLEDs();
}
