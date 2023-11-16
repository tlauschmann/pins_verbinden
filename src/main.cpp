#include <Arduino.h>

// Pins für Eingänge und Relais/Led-Ausgang
uint8_t in_pins[12] = {2, 3, 8, 9, 10, 11, 14, 15, 16, 17, 18, 19};
uint8_t out_pins[5] = {4, 5, 6, 7, 13};

// Der Zustand der Ausgänge
uint8_t output = LOW;

// Die Lösung des Rätsels
uint8_t lsg[12] = {14, 15, 16, 17, 18, 19, 2, 3, 8, 9, 10, 11};


// Prüfe, ob die übergebenen Arrays gleich sind.
bool arrays_are_equal(uint8_t arr1[], uint8_t arr2[], int size) {
  for (int i = 0; i < size; i++) {
    if (arr1[i] != arr2[i]) {
      return false;
    }
  }
  return true;
}


void setup() {
    // Pullupwiederstände an Eingängen aktivieren
    for(uint8_t i = 0; i < 12; i++){
        pinMode(in_pins[i], INPUT_PULLUP);
    }

    // Ausgänge initieren
    for(uint8_t i = 0; i < 5; i++){
        pinMode(out_pins[i], OUTPUT);
    }

    // UART starten
    Serial.begin(115200);
}

void loop() {
    // Aktuelle Kombination der Eingänge
    uint8_t pin_state[12] = {0};

    // nacheinander jeden Eingang auf GND schalten
    for(uint8_t i = 0; i < 12; i++){
        pinMode(in_pins[i], OUTPUT);
        digitalWrite(in_pins[i], LOW);

        // Prüfen welcher Eingang auf GND gezogen wird
        for(uint8_t j = 0; j < 12; j++){
            if (i == j) continue;
            if (digitalRead(in_pins[j]) == LOW) pin_state[j] = in_pins[i];
        }

        // Eingang wieder als PULLUP einstellen
        pinMode(in_pins[i], INPUT_PULLUP);
        delay(1);
    }
    Serial.print("\nArray: ");
    for (int i = 0; i < 12; i++) {
        Serial.print(pin_state[i]);
        Serial.print(" ");
    }

    // Ermitteln, ob das Rätsel gelöst ist, ein Reset vorliegt oder gar nix
    if (arrays_are_equal(pin_state, lsg, 12)){
        output = HIGH;
    }
    if (pin_state[11] == 18){
        output = LOW;
    }

    // output auf Ausgänge schreiben
    Serial.print("\nOutput: " + (String) output);
    for (uint8_t i = 0; i < 5; i++){
        digitalWrite(out_pins[i], output);
    }
}