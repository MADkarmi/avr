/*
 * Morse_Code.c
 *
 * Created: 2017-10-19 22:11:48
 * Author : Radoslaw_Madzia
 */ 
 
#include <avr/io.h>
#include <util/delay.h>
int dot = 250;


void letterId(char* character) {
  int i = 0;
  while(character[i] != '\0') {
    output(character[i]);
    i++;
  }
  _delay_ms(dot * 3);
}

void output(char ID) {
  PORTB = 0x03;
  if (ID == '.') {
    _delay_ms(dot);
  }
  else {
    _delay_ms(dot * 3);
  }
  PORTB = 0x00;
  _delay_ms(dot);
}
void main(void)
{
  Serial.begin(9600);

  
  DDRB |= 0x03;
  char* morseLetters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.","...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." };
  char* morseNumbers[] = {"-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

  
  while (1) {
      char info;
      if (Serial.available() > 0) {
        info = Serial.read();
        if (info >= 'a' && info <= 'z') 
          letterId(morseLetters[info - 'a']);
        else if(info >= 'A' && info <= 'Z') 
          letterId(morseLetters[info - 'A']);
        else if(info >= 0 || info <= 9) 
          letterId(morseNumbers[info]);
        else if(info == ' ') 
          _delay_ms(dot * 4);
      }
   }
}