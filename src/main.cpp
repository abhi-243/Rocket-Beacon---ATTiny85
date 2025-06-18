#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

volatile bool shouldWake = false;

// Watchdog Timer Interrupt - triggers after 1s to wake from sleep
ISR(WDT_vect) {
  shouldWake = true;
}

// Sleep for the specified duration (ms), in 1-second intervals using the watchdog
void sleepFor(unsigned long ms) {
  while (ms > 0) {
    shouldWake = false;

    // Configure watchdog for 1s interrupt
    MCUSR &= ~(1 << WDRF);
    WDTCR |= (1 << WDCE) | (1 << WDE);
    WDTCR = (1 << WDIE) | (1 << WDP2) | (1 << WDP1); // 1s period

    set_sleep_mode(SLEEP_MODE_IDLE);  // Use idle sleep mode
    cli();
    sleep_enable();
    sei();
    sleep_cpu();                       // Wait for WDT interrupt
    sleep_disable();

    if (shouldWake) ms -= 1000;
  }
}

// Morse code mapping: ASCII character to bit-encoded Morse pattern
struct t_mtab {
  char c;
  uint8_t pat;
};

// Lookup table for Morse patterns
struct t_mtab morsetab[] = {
  {'+', 42}, {'-', 97}, {'=', 49}, {'.', 106}, {',', 115}, {'?', 76}, {'/', 41},
  {'A', 6},  {'B', 17}, {'C', 21}, {'D', 9},   {'E', 2},   {'F', 20}, {'G', 11},
  {'H', 16}, {'I', 4},  {'J', 30}, {'K', 13},  {'L', 18},  {'M', 7},  {'N', 5},
  {'O', 15}, {'P', 22}, {'Q', 27}, {'R', 10},  {'S', 8},   {'T', 3},  {'U', 12},
  {'V', 24}, {'W', 14}, {'X', 25}, {'Y', 29},  {'Z', 19},
  {'1', 62}, {'2', 60}, {'3', 56}, {'4', 48},  {'5', 32},
  {'6', 33}, {'7', 35}, {'8', 39}, {'9', 47},  {'0', 63}
};

#define N_MORSE  (sizeof(morsetab)/sizeof(morsetab[0]))
#define SPEED    13
#define txpin    1
#define DOTLEN   (1200 / SPEED)
#define DASHLEN  (3 * DOTLEN)

// Transmit a dash (3 units long)
void dash() {
  digitalWrite(txpin, HIGH);
  delay(DASHLEN);
  digitalWrite(txpin, LOW);
  delay(DOTLEN);
}

// Transmit a dot (1 unit long)
void dit() {
  digitalWrite(txpin, HIGH);
  delay(DOTLEN);
  digitalWrite(txpin, LOW);
  delay(DOTLEN);
}

// Transmit a single character in Morse code
void send(char c) {
  if (c == ' ') {
    delay(7 * DOTLEN); // Word space
    return;
  }

  for (int i = 0; i < N_MORSE; i++) {
    if (morsetab[i].c == c) {
      uint8_t p = morsetab[i].pat;
      while (p != 1) {
        if (p & 1) dash();
        else dit();
        p >>= 1;
      }
      delay(2 * DOTLEN); // Inter-character space
      return;
    }
  }
}

// Transmit an entire string as Morse code
void sendmsg(char *str) {
  while (*str) send(*str++);
}

void setup() {
  pinMode(txpin, OUTPUT);
  digitalWrite(txpin, LOW); // Keep transmitter idle

  // Disable unused hardware for power saving
  ADCSRA &= ~(1 << ADEN);
  power_adc_disable();
  power_timer1_disable();
  power_usi_disable();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Global sleep mode setting
}

void loop() {
  sendmsg("VK2ASN VK2ASN VK2ASN TESTING TESTING ");
  sleepFor(15000); // Sleep for 15 seconds
}