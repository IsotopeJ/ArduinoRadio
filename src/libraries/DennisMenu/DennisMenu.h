/*   Framework for a simple menu that stores values in EEPROM
 * 
 * 
 */

#ifndef MenuItem_h
#define MenuItem_h

//#include <arduino.h>
#include <EEPROM.h>

const String alphanumeric = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const long baseTen[10] = {0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

 //type long is right justified
 //type string is left justified
struct MenuItem
{
  public:
  String label;         //displayed to user when in setup menu
  String suffix;        //displayed after value  (example: kHz, 
  String value;         //value translated to string for display and for storing to EEPROM (whitespace buffered)
  String type;          //string (alpha-numeric), int, picklist
  int length;           //string length of value as displayed and stored in EEPROM
  long maximum;          //maximum length (string) or value (int)
  long minimum;          //minimum length (string) or value (int)
  String *strVariable;  //string variable used in code (if set)
  long *intVariable;     //integer variable used in code (if set)
  String *choices;      //if type picklist, the choices array
  int numChoices;       //number of elements in the choices array
  String factoryValue;  //value to revert to when "factory reset   
};

template <size_t NUM_TEST>
class MenuTest{
	public:
	MenuItem items[NUM_TEST];
};

template <size_t NUM_MENU_ITEMS>
class DennisMenu{
	
	public:
	MenuItem items[NUM_MENU_ITEMS];
	
	//Menu(byte numItems, MenuItem list[NUM_MENU_ITEMS]);
	
	void setMenuVariables(MenuItem m);
	void menuForward();
	void menuBackward();
	void resetCurrentValueDigit();
	void valueDigitLeft();
	void valueDigitRight();
	
	void increaseValue();	//increment value by some amount, return new value
	void decreaseValue(); //decrement value by some amount, return new value
	
	String label();
	String value();
	String suffix();
	
	
	//private:
	byte numberOfMenuItems;
	byte currentMenuItem;// = 0;         TODO:  FIX THIS !!!!!!!!!!!!!!!!!!!
	byte currentValueDigit;// = 0;
	
  
	
};
 

 




String readEEPROM(char address, char len);

void writeEEPROM(char address, String data);

void padStringWithSpaces(String s, int len);





#endif
