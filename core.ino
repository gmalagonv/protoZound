/******** Load AVR timer interrupt macros ********/
#include <avr/interrupt.h>

/******** Sine wave parameters ********/
#define PI2     6.283185 // 2 * PI - saves calculating it later
#define AMP     127      // Multiplication factor for the sine wave
#define OFFSET  128      // Offset shifts wave to just positive values

/******** Lookup table ********/
#define LENGTH  256  // The length of the waveform lookup table
byte wave[LENGTH];   // Storage for the waveform

/******** Waveform parameters ********/
#define SINE     0
#define RAMP     1
#define TRIANGLE 2
#define SQUARE   3
#define RANDOM   4

//int potPin = 1 ;//// PIN TEMPERATURE SENSOR

void setup() {
pinMode(2, INPUT); //SINE 
pinMode(3, INPUT); //RAMP
pinMode(4, INPUT); //TRIANGLE
pinMode(5, INPUT); //SQUARE
//pinMode(6, INPUT); //RANDOM
//Serial.begin(9600);// temperature sensor
  /******** Populate the waveform lookup table with a sine wave ********/



 
  /******** Set timer1 for 8-bit fast PWM output ********/
  pinMode(9, OUTPUT);       // Make timer's PWM pin an output
  TCCR1B  = (1 << CS10);    // Set prescaler to full 16MHz
  TCCR1A |= (1 << COM1A1);  // PWM pin to go low when TCNT1=OCR1A
  TCCR1A |= (1 << WGM10);   // Put timer into 8-bit fast PWM mode
  TCCR1B |= (1 << WGM12); 

  /******** Set up timer 2 to call ISR ********/
  TCCR2A = 0;               // We need no options in control register A
  TCCR2B = (1 << CS21);     // Set prescaller to divide by 8
  TIMSK2 = (1 << OCIE2A);   // Set timer to call ISR when TCNT2 = OCRA2
  //OCR2A = 15;               // sets the frequency of the generated wave
  sei();                    // Enable interrupts to generate waveform!
}



/******** Called every time TCNT2 = OCR2A ********/
ISR(TIMER2_COMPA_vect) {  // Called each time TCNT2 == OCR2A
  static byte index=0;    // Points to successive entries in the wavetable
  OCR1AL = wave[index++]; // Update the PWM output
  asm("NOP;NOP");         // Fine tuning
  TCNT2 = 6;              // Timing to compensate for time spent in ISR
}


void waveform(byte w) {
 switch(w) {

   case SINE: 
    for (int i=0; i<LENGTH; i++) 
      {float v = OFFSET+(AMP*sin((PI2/LENGTH)*i));
      wave[i]=int(v);
    }
    break;

  case RAMP:
    for (int i=0; i<LENGTH; i++) {
      wave[i]=i;
    }
    break;

  case TRIANGLE:
    for (int i=0; i<LENGTH; i++) {
      if (i<(LENGTH/2)) {
        wave[i]=i*2;
      } else {
        wave[i]=(LENGTH-1)-(i*2);
      }
    }
    break;
    
  case SQUARE:
    for (int i=0; i<(LENGTH/2); i++) {
      wave[i]=255;
    }
    break;

  case RANDOM:
    randomSeed(3);
    for (int i=0; i<LENGTH; i++) {
      wave[i]=random(255);
    }
      break;
  }
}

  void decay(void) {
  int i, j;
  for (j=0; j<128; j++) {
    for (i=0; i<LENGTH; i++) {
      if (wave[i]<127) {
        wave[i]++;
      }
      if (wave[i]>127) {
        wave[i]--;
      }
    }
    delay(0.5);
  }
  //cli();
}

void loop() {  
  //////////////START TEMPERATURE SENSOR
/*
  int val2;
  int dat;
  
  val2 = analogRead(potPin);
  
  dat = (125*val2)>>8 ;
  Serial.print("Temp : ") ;
  Serial.print(dat) ;
  Serial.println("C");
  delay(1);
  
  //////////////END TEMPERATURE SENSOR
  */
  int sinebutton =  digitalRead(2);
  int rampbutton =  digitalRead(3);
  int trianglebutton =  digitalRead(4);
  int squarebutton =  digitalRead(5);
  int randombutton =  0;//digitalRead(6);
  int val = analogRead(A0);
  

if (val <= 15){//1
    OCR2A = 451;
}
if (15 < val && val <= 57){//2
    OCR2A = 402;
}
if (57 < val && val <= 99){//3
    OCR2A = 338;
}
if (99 < val && val <= 141){//4
    OCR2A = 301;
} 
if (141 < val && val <= 183){//5
    OCR2A = 268;
} 
if (183 < val && val <= 225){//6
    OCR2A = 225;
} 
if (225 < val && val <= 267){//7
    OCR2A = 201;
} 
if (267 < val && val <= 309){//8
    OCR2A = 169;
} 
if (309 < val && val <= 351){//9
    OCR2A = 151;
} 
if (351 < val && val <= 393){//10
    OCR2A = 134;
} 
if (393 < val && val <= 435){//11
    OCR2A = 113;
} 
if (435 < val && val <= 477){//12
    OCR2A = 100;
} 
 if (477 < val && val <= 519){//13
    OCR2A = 84;
} 
if (519 < val && val <= 561){//14
    OCR2A = 75;
}  
if (561 < val && val <= 603){//15
    OCR2A = 67;
}  
if (603 < val && val <= 645){//16
    OCR2A = 56;
} 
if (645 < val && val <= 687){//17
    OCR2A = 50;
} 
if (687 < val && val <= 729){//18
    OCR2A = 42;
} 
if (729 < val && val <= 771){//19
    OCR2A = 38;
} 
if (771 < val && val <= 813){//20
    OCR2A = 34;
} 
if (813 < val && val <= 855){//21
    OCR2A = 28;
} 
if (855 < val && val <= 897){//22
    OCR2A = 25;
} 
if (897 < val && val <= 939){//23
    OCR2A = 21;
} 
if (939 < val && val <= 981){//24
    OCR2A = 19;
}    
if (981 < val){//25
    OCR2A = 17;
}
  if (sinebutton == HIGH){
      waveform(SINE);
             //decay();  
  }
  if (rampbutton == HIGH){
      waveform(RAMP);
    // decay();  
  } 
  if (trianglebutton == HIGH){
      waveform(TRIANGLE);  
  }
  if (squarebutton == HIGH){
      waveform(SQUARE);  
  }
 /* if (randombutton == HIGH){
      waveform(RANDOM);
  }*/
  if (sinebutton == LOW && rampbutton == LOW && trianglebutton == LOW && squarebutton == LOW && randombutton == LOW){
      decay();                   
  }
} 







 
