#pragma once

#include <Arduino.h>

struct MenuItem
{
  public:
  String label;         //displayed to user when in setup menu
  String suffix;        //displayed after value  (example: kHz,
  String value;         //value translated to string for display and for storing to EEPROM (whitespace buffered)
  String type;          //string (alpha-numeric), int, picklist
  int length;           //maximum length in characters for value
  String *strVariable;  //string variable used in code (if set)
  int *intVariable;     //integer variable used in code (if set)
  String *choices;      //if type picklist, the choices array
  int numChoices;       //number of elements in the choices array
};

void updateMenuVariables(MenuItem m);
