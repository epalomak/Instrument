#include <Keypad.h>
#include <Wire.h>
#include <avr/wdt.h>
#include "rgb_lcd.h"

//Defining all the necessary notes and their frequencies 
  #define A2  110
  #define AS2 117
  #define B2  123
  #define C3  131
  #define CS3 139
  #define D3  147
  #define DS3 156
  #define E3  165
  #define F3  175
  #define FS3 185
  #define G3  196
  #define GS3 208
  #define A3  220
  #define AS3 233
  #define B3  247
  #define C4  262
  #define CS4 277
  #define D4  294
  #define DS4 311
  #define E4  330
  #define F4  349
  #define FS4 370
  #define G4  392
  #define GS4 415
  #define A4  440
  #define AS4 466
  #define B4  494
  #define C5  523
  #define CS5 554
  #define D5  587
  #define DS5 622
  #define E5  659
  #define F5  698
  #define FS5 740
  #define G5  784
  #define GS5 831
  #define A5  880
  #define AS5 932
  #define B5  988
  #define C6  1047
  #define CS6 1109
  #define D6  1175
  #define DS6 1245
  #define E6  1319
  #define F6  1397
  #define FS6 1480
  #define G6  1568
  #define GS6 1661
  #define A6  1760
  #define AS6 1865
  #define B6  1976
  #define C7  2093
  #define CS7 2217
  #define D7  2349

int notes[][7] =      //An int array that defines used keys and octaves
{ // first octave
  {A2, B2, CS3, D3, E3, FS3, GS3}, //a-major
  {A2, B2, C3, D3, E3, F3, G3}, // a-minor
  {B2, CS3, DS3, E3, F3, GS3, AS3}, // h-major
  {B2, CS3, D3, E3, FS3, G3, A3}, // h-minor
  {C3, D3, E3, F3, G3, A3, B3}, // c-major
  {C3, DS3, F3, G3, GS3, AS3, B3}, // c-minor
  {D3, E3, FS3, G3, A3, B3, CS4}, // d-major
  {D3, E3, F3, G3, A3, AS3, C4}, // d-minor
  {E3, FS3, GS3, A3, B3, CS4, DS4}, //e-major
  {E3, FS3, G3, A3, B3, C4, D4}, // e-minor

// second octave
  {A3, B3, CS4, D4, E4, FS4, GS4}, //a-major
  {A3, B3, C4, D4, E4, F4, G4}, // a-minor
  {B3, CS4, DS4, E4, F4, GS4, AS4}, // h-major
  {B3, CS4, D4, E4, FS4, G4, A4}, // h-minor
  {C4, D4, E4, F4, G4, A4, B4}, // c-major
  {C4, DS4, F4, G4, GS4, AS4, B4}, // c-minor
  {D4, E4, FS4, G4, A4, B5, CS5}, // d-major
  {D4, E4, F4, G4, A5, AS5, C5}, // d-minor
  {E4, FS4, GS4, A4, B4, CS5, DS5}, //e-major
  {E4, FS4, G4, A4, B4, C5, D5}, // e-minor

// third octave
  {A4, B4, CS5, D5, E5, FS5, GS5}, //a-major
  {A4, B4, C5, D5, E5, F5, G5}, // a-minor
  {B4, CS5, DS5, E5, F5, GS5, AS5}, // h-major
  {B4, CS5, D5, E5, FS5, G5, A5}, // h-minor
  {C5, D5, E5, F5, G5, A5, B5}, // c-major
  {C5, DS5, F5, G5, GS5, AS5, B5}, // c-minor
  {D5, E5, FS5, G5, A5, B5, CS6}, // d-major
  {D5, E5, F5, G5, A5, AS5, C6}, // d-minor
  {E5, FS5, GS5, A5, B5, CS6, DS6}, //e-major
  {E5, FS5, G5, A5, B5, C6, D6}, // e-minor

// forth octave
  {A5, B5, CS6, D6, E6, FS6, GS6}, //a-major
  {A5, B5, C6, D6, E6, F6, G6}, // a-minor
  {B5, CS6, DS6, E6, F6, GS6, AS6}, // h-major
  {B5, CS6, D6, E6, FS6, G6, A6}, // h-minor
  {C6, D6, E6, F6, G6, A6, B6}, // c-major
  {C6, DS6, F6, G6, GS6, AS6, B6}, // c-minor
  {D6, E6, FS6, G6, A6, B6, CS7}, // d-major
  {D6, E6, F6, G6, A6, AS6, C7}, // d-minor
  {E6, FS6, GS6, A6, B6, CS7, DS7}, //e-major
  {E6, FS6, G6, A6, B6, C7, D7}, // e-minor

};

//Keyboard configuration
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

//Keyboard pin configuration
byte rowPins[ROWS] = {5, 4, 3, 2}; 
byte colPins[COLS] = {9, 8, 7, 6}; 

//Initialization of the keyboard
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// Initialization of the LCD screen
rgb_lcd lcd;
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

// Determination of the used pins, the first pin is on the registers.
const int trigPin = B00000100;
const int echoPin = 16;
const int interruptPin = 11;
const int potPin = A0;

// Characters reserved for button presses
char key = NULL;
char key2 = NULL;

// Variables for length and time
long cm, t;

// Time interrupt counter
volatile uint16_t timerCounter = 0;


void setup()
{
  //Initialization of the LCD screen
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC mode, skaalattu 1024
  OCR1A = 15624; // 16MHz / 1024 / 1Hz - 1
  TIMSK1 = (1 << OCIE1A); // Enable Timer/Counter 1 Compare Match A interrupt

  // pin configuration
  DDRB = trigPin;
  pinMode(echoPin, INPUT);
  pinMode(potPin, INPUT);
  
  // Enable ADEN to activate the ADC
  ADCSRA |= (1 << ADEN);
  // Set the voltage reference to AVcc.
  ADMUX |= (1 << REFS0); 
  // Clear ADLAR to ensure correct results at the basic level
  ADMUX &= ~(1 << ADLAR); 

  // mute the speaker
  noTone(12);

  // Initialize the watchdog
  wdt_enable(WDTO_8S);
}

// Function for capturing keyboard input
void check_key()
{
  lcd.clear();
  lcd.print("valitse savel");
  lcd.setCursor(0, 2);
  lcd.print( "0 - 9");

  // Wait for the keyboard to be pressed with the correct value
  while (!key)
  {
    key = keypad.getKey();
    // Verify that incorrect keys have not been used
    if (key =='A' ||key =='B' ||key =='C' ||key =='D' || key == '*' || key == '#')
      key = NULL;
  }

  lcd.clear();
  lcd.print("Valitse oktaavi");
  lcd.setCursor(0, 2);
  lcd.print("A - D");
  // Wait for the keyboard to be pressed with the correct value
  while (!key2)
  {
    key2 = keypad.getKey();
    // Verify that incorrect keys have not been used
    if (key2 =='0' || key2 =='1' || key2 =='2' || key2 =='3' || key2 =='4' || key2 =='5' || key2 =='6' || key2 =='7' || key2 =='8' ||key2 =='9' ||  key2 == '*' || key2 == '#')
      key2 = NULL;
  }
  lcd.clear();
  lcd.print("Savel ");
  lcd.print(key);
  lcd.setCursor(0, 2);
  lcd.print("Oktaavi ");
  lcd.print(key2);
}
void loop()
{
  // If no values have been entered on the keypad yet, call the check_key function
  if (!key2 && !key)
    check_key();
  
  // Check if the user has triggered the button interrupt or if the timer interrupt has reached its limit
  if (digitalRead(interruptPin) == HIGH ||  timerCounter >= 1800)
  {
    lcd.clear();
    noTone(12);
    delay(500);
    key = NULL;
    key2 = NULL;
    timerCounter = 0;
  }

  // Set the trigPin state low using registers
  PORTB ^= trigPin;
  delayMicroseconds(4);
  // Set the trigPin state high using registers and the ultrasonic sensor emits ultrasound
  PORTB = trigPin;
  delayMicroseconds(10);
  // Set the trigPin state low using registers

  // Calculate the time it took for the ultrasound to return to the sensor
  t = pulseIn(echoPin, HIGH);

  // Convert the measured time to centimeters
  cm = t / 29 / 2;

  // Call the function that reads the potentiometer value to get the note duration
  int dur = note_duration();

  // Call the function that plays the note
  tone_to_play(cm,(int)key -48, dur);

  // Reset the watchdog
  wdt_reset();

}

// Function that performs AD conversion and obtains the potentiometer value
int readPotentiometer(int pin)
{
  // Set ADC conversion
  ADCSRA |= (1 << ADSC);  // Start conversion

  // Wait for conversion to complete
  while (ADCSRA & (1 << ADSC))
  ;

  // Return the read value
  return ADC;
}

int note_duration()
{
  
  ADMUX &= 0xF8;
  ADMUX |= (1 << REFS0) | (potPin & 0x07);

  // Call the function that reads the potentiometer value
  int val = readPotentiometer(potPin);

  // Convert the read value to note duration in 2-16 parts of a second
  int dur = map(val, 150, 1000, 2, 16);

  return dur;
}  

// Functions that determine which note to play based on distance
void tone_to_play(long cm, int mod, int dur)
{
  int noteplay = 0;
  int oct;

  // Add mod, which changes to the next octave from the array
  if (key2 == 'A')
    oct = 0;
  else if (key2 == 'B')
    oct = 10;
  else if (key2 == 'C')
    oct = 20;
  else if (key2 == 'D')
    oct = 30;

  // Check the distance and set the correct note. mod changes the pitch and oct changes the octave
  if (cm >= 2 && cm < 5)
    noteplay = notes[oct + mod][0];
  if (cm >= 5 && cm < 8)
    noteplay = notes[oct + mod][1];
  if (cm >= 8 && cm < 11)
    noteplay = notes[oct + mod][2];
  if (cm >= 11 && cm < 14)
    noteplay = notes[oct + mod][3];
  if (cm >= 14 && cm < 17)
    noteplay = notes[oct + mod][4];
  if (cm >=  17 && cm < 20)
    noteplay = notes[oct + mod][5];
  if (cm >= 20 && cm < 23)
    noteplay = notes[oct + mod][6];
  if (cm > 21 && dur == 0)
    noTone(12);
  // If distance is found, play a sound
  else
  {
    // Define the duration of the note in seconds/dur
    int notedur = 1000/dur;
    // Input the note into the speaker
    tone(12, noteplay, notedur);
    // A delay 1.3 times the duration of the note to distinguish the shortest note intervals.
    delay(notedur* 1.3);
  }
}

ISR(TIMER1_COMPA_vect)
{
  timerCounter++; // Increment the counter every second
}
