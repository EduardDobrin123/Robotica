#include <Arduino.h>

// Definirea pinilor pentru LED-uri si butoane
const int rgbRedPin = 6;  
const int rgbGreenPin = 5;  
const int ledPins[] = {10, 9, 8, 7}; 
const int buttonStartPin = 2; 
const int buttonStopPin = 3;   

// Variabile de stare
bool charging = false;  // Indica daca incarcarea este in curs
bool stationAvailable = true;  // Starea statiei (libera/ocupata)
int currentLed = 0;  // LED-ul care clipeste in prezent

unsigned long lastUpdate = 0;  // Timpul ultimei actualizari
const unsigned long interval = 3000;  // Intervalul de 3 secunde pentru incarcare

void setup() {
  // Configurarea pinilor pentru LED-uri ca iesire
  pinMode(rgbRedPin, OUTPUT);
  pinMode(rgbGreenPin, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);  // LED-urile sunt stinse initial
  }

  // Configurarea pinilor pentru butoane ca intrare
  pinMode(buttonStartPin, INPUT_PULLUP);
  pinMode(buttonStopPin, INPUT_PULLUP);

  // Setarea initială a LED-ului RGB (verde = statie libera)
  digitalWrite(rgbGreenPin, HIGH);
  digitalWrite(rgbRedPin, LOW);
}

void loop() {
  unsigned long currentTime = millis();

  // Verificam daca butonul de start a fost apasat
  if (digitalRead(buttonStartPin) == LOW && stationAvailable) {
    startCharging();
  }

  // Verificam daca butonul de stop a fost apasat lung
  if (digitalRead(buttonStopPin) == LOW && !stationAvailable) {
    delay(1000);  // Așteptam 1 secunda pentru a considera apasarea lunga
    if (digitalRead(buttonStopPin) == LOW) {
      stopCharging();
    }
  }

  // Dacă incarcarea este in desfasurare, controlam LED-urile
  if (charging && currentTime - lastUpdate >= interval) {
    lastUpdate = currentTime;
    updateChargingProgress();
  }
}

void startCharging() {
  // Incepem procesul de incarcare
  stationAvailable = false;
  charging = true;
  currentLed = 0;  // Incepem cu primul LED

  // Setam LED-ul RGB la rosu (ocupat)
  digitalWrite(rgbGreenPin, LOW);
  digitalWrite(rgbRedPin, HIGH);

  // Pornim primul LED
  digitalWrite(ledPins[currentLed], HIGH);
}

void stopCharging() {
  // Oprim incarcarea si resetam statia la starea libera
  charging = false;
  stationAvailable = true;

  // Clipim toate LED-urile de 3 ori ca animatie de final
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      digitalWrite(ledPins[j], HIGH);
    }
    delay(500);
    for (int j = 0; j < 4; j++) {
      digitalWrite(ledPins[j], LOW);
    }
    delay(500);
  }

  // Resetam LED-urile de incarcare
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
  }

  // Setam LED-ul RGB la verde (statie libera)
  digitalWrite(rgbGreenPin, HIGH);
  digitalWrite(rgbRedPin, LOW);
}

void updateChargingProgress() {
  // LED-ul curent clipeste, iar cele anterioare raman aprinse continuu
  if (currentLed < 4) {
    for (int i = 0; i < currentLed; i++) {
      digitalWrite(ledPins[i], HIGH);  // LED-urile din urma sunt aprinse
    }

    // Clipirea LED-ului curent
    for (int i = 0; i < 3; i++) {
      digitalWrite(ledPins[currentLed], LOW);
      delay(500);
      digitalWrite(ledPins[currentLed], HIGH);
      delay(500);
    }

    // Trecem la urmatorul LED
    currentLed++;
  } else {
    // Finalizarea incarcarii: toate LED-urile clipesc de 3 ori
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
        digitalWrite(ledPins[j], HIGH);
      }
      delay(500);
      for (int j = 0; j < 4; j++) {
        digitalWrite(ledPins[j], LOW);
      }
      delay(500);
    }

    // Resetam toate LED-urile si marcăm statia ca libera
    for (int i = 0; i < 4; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    stationAvailable = true;
    charging = false;

    // Setam LED-ul RGB la verde (statie libera)
    digitalWrite(rgbGreenPin, HIGH);
    digitalWrite(rgbRedPin, LOW);
  }
}
