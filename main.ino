// Definições das notas em frequências (em Hz)
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

// Melodia da música do Super Mario (simplificada)
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};

// Durações das notas (em ms)
int noteDurations[] = {
  125, 125, 125, 125,
  125, 125, 125, 125,
  125, 125, 125, 125,
  125, 125, 125, 125,

  125, 125, 125, 125,
  125, 125, 125, 125,
  125, 125, 125, 125,
  125, 125, 125, 125,

  125, 125, 125, 125,
  125, 125, 125, 125,
  125, 125, 125, 125,
  125, 125, 125, 125
};

// Pinos e limiar do sensor e buzzer
const int sensorPin = A0;      // Pino do sensor
const int buzzerPin = 9;       // Pino do buzzer
const int threshold = 100;     // Limiar de detecção

// Estado do buzzer
bool isPlaying = false;
unsigned long playStartTime = 0;
const unsigned long playDuration = 20000; // Duração total para tocar a música (em ms)

// Função para tocar a música do Super Mario
void playSuperMario() {
  for (int thisNote = 0; thisNote < sizeof(melody) / sizeof(int); thisNote++) {
    int noteDuration = noteDurations[thisNote];
    if (melody[thisNote] != 0) {
      tone(buzzerPin, melody[thisNote], noteDuration);
    }
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}

// Função para tocar a música do Super Mario em loop, até que o tempo total se esgote
void playSuperMarioLoop() {
  unsigned long currentMillis = millis();
  if (currentMillis - playStartTime < playDuration) {
    playSuperMario();
  } else {
    noTone(buzzerPin);
    isPlaying = false;
  }
}

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);  // Inicia a comunicação serial a 9600 bps
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  Serial.print("Valor do sensor: ");
  Serial.println(sensorValue);

  // Filtro simples para suavizar a leitura
  static int previousValue = 0;
  int smoothedValue = (sensorValue + previousValue) / 2;
  previousValue = sensorValue;

  if (smoothedValue < threshold) {
    Serial.println("Objeto detectado");
    if (!isPlaying) {
      isPlaying = true;
      playStartTime = millis(); // Inicia o cronômetro
    }
    if (isPlaying) {
      playSuperMarioLoop();  // Toca a música em loop até a duração total se esgotar
    }
  } else {
    if (isPlaying) {
      noTone(buzzerPin); // Para o buzzer se o sensor não detectar mais o objeto
      isPlaying = false;
    }
  }

  delay(100);  // Pequeno atraso para evitar leitura excessiva
}
