#include <Arduino.h>
#include <DennisMenu.h>
#include <EEPROM.h>


/*
void DennisDennisMenu<size_t NUM_MENU_ITEMS>::Menu(std::initializer_list MenuItem list[NUM_MENU_ITEMS]){
	this.numberOfMenuItems = NUM_MENU_ITEMS;
	this.items = list;
}


void DennisDennisMenu<size_t NUM_MENU_ITEMS>::~Menu(){
	//delete[] items;
}*/

template <size_t NUM_MENU_ITEMS> 
String DennisMenu<NUM_MENU_ITEMS>::label(){
	return this.items[currentMenuItem].label;
}

template <size_t NUM_MENU_ITEMS> 
String DennisMenu<NUM_MENU_ITEMS>::value(){
	return this.items[currentMenuItem].value;
}

template <size_t NUM_MENU_ITEMS> 
String DennisMenu<NUM_MENU_ITEMS>::suffix(){
	return this.items[currentMenuItem].suffix;
}

template <size_t NUM_MENU_ITEMS> 
void DennisMenu<NUM_MENU_ITEMS>::menuForward(){
	this.currentMenuItem++;
	if(this.currentMenuItem == this.numberOfMenuItems) this.currentMenuItem = 0;
	this.resetCurrentValueDigit();
}

template <size_t NUM_MENU_ITEMS> 
void DennisMenu<NUM_MENU_ITEMS>::menuBackward(){
	this.currentMenuItem--;
	if(this.currentMenuItem < 0) this.currentMenuItem = this.numberOfMenuItems - 1;
	this.resetCurrentValueDigit();
}

template <size_t NUM_MENU_ITEMS> 
void DennisMenu<NUM_MENU_ITEMS>::resetCurrentValueDigit(){
	if(this.items[this.currentMenuItem].type == "string"){
		this.currentValueDigit = 0;												//left justified
	}
	else if(this.items[this.currentMenuItem].type == "long"){
		this.currentValueDigit = this.items[this.currentMenuItem].length - 1;	//right-justified
	}
}

template <size_t NUM_MENU_ITEMS> 
void DennisMenu<NUM_MENU_ITEMS>::valueDigitLeft(){
	if(this.currentValueDigit == 0) return;
	this.currentValueDigit--;
}

template <size_t NUM_MENU_ITEMS> 
void DennisMenu<NUM_MENU_ITEMS>::valueDigitRight(){
	if(this.currentValueDigit == this.items[this.currentMenuItem].length - 1) return;
	if(this.items[this.currentMenuItem].type == 'string'){
		if(this.item[currentMenuItem].value.charAt(this.currentValueDigit) == " ") return;  //don't allow advance if current digit is whitespace
	}
	this.currentValueDigit++;
}

template <size_t NUM_MENU_ITEMS> 
void DennisMenu<NUM_MENU_ITEMS>::increaseValue(){
	if(this.items[this.currentMenuItem].type == 'long'){
		long adder = baseTen[this.items[this.currentMenuItem].length - this.currentValueDigit];
		long newValue = this.items[this.currentMenuItem].value.toInt();
		newValue += adder;
		if(newValue > this.items[this.currentMenuItem].maximum) return;
		this.items[this.currentMenuItem].value = String(newValue, 10);
		leftPadWithSpaces(this.items[this.currentMenuItem].value, this.items[this.currentMenuItem].length);
	}
	else if(this.items[this.currentMenuItem].type == 'string'){
		char currentChar = this.items[currentMenuItem].value[this.currentValueDigit];
		
		byte charIndex = alphanumeric.indexOf(currentChar);
		if(charIndex < 0 ) charIndex = 0;
		
		//byte charIndex;
		//char *cPtr = std::find(std::begin(alphanumeric), std::end(alphanumeric), currentChar);
		//if(cPtr != std::end(alphanumeric)) charIndex = std::distance(alphanumeric, cPtr);	//found
		//else charIndex = 0;  //error occurred if not found, reset to 0
		
		if(charIndex == alphanumeric.length() - 1){
			charIndex = 0;
		}
		else{
			charIndex++;
		}	
		this.items[this.currentMenuItem].value[this.currentValueDigit] = alphanumeric[charIndex];
	}
	else if(this.items[this.currentMenuItem].type == 'picklist'){
		//TODO
	}
}

template <size_t NUM_MENU_ITEMS> 
void DennisMenu<NUM_MENU_ITEMS>::decreaseValue(){
	if(this.items[this.currentMenuItem].type == 'long'){
		long adder = baseTen[this.items[this.currentMenuItem].length - this.currentValueDigit];
		long newValue = this.items[this.currentMenuItem].value.toInt();
		newValue -= adder;
		if(newValue < this.items[this.currentMenuItem].minimum) return;
		this.items[this.currentMenuItem].value = String(newValue, 10);
		leftPadWithSpaces(this.items[this.currentMenuItem].value, this.items[this.currentMenuItem].length);
	}
	else if(this.items[this.currentMenuItem].type == 'string'){
		char currentChar = this.items[currentMenuItem].value[this.currentValueDigit];
		
		byte charIndex = alphanumeric.indexOf(currentChar);
		if(charIndex < 0 ) charIndex = 0;
		
		//byte charIndex;
		//char *cPtr = std::find(std::begin(alphanumeric), std::end(alphanumeric), currentChar);
		//if(cPtr != std::end(alphanumeric)) charIndex = std::distance(alphanumeric, cPtr);	//found
		//else charIndex = 0;  //error occurred if not found, reset to 0
		
		if(charIndex == 0){
			charIndex = alphanumeric.length() - 1;
		}
		else{
			charIndex--;
		}	
		this.items[this.currentMenuItem].value[this.currentValueDigit] = alphanumeric[charIndex];
	}
	else if(this.items[this.currentMenuItem].type == 'picklist'){
		//TODO
	}
}



void leftPadWithSpaces(String s, int len){
	while(s.length() < len){
		s = " " + s;
	}
}

template <size_t NUM_MENU_ITEMS> 
void DennisMenu<NUM_MENU_ITEMS>::setMenuVariables(MenuItem m){
  //String s = m.value;
  //s.trim();
  //if(m.strVariable != NULL) *(m.strVariable) = s;
  //if(m.intVariable != NULL) *(m.intVariable) = s.toInt();
}


String readEEPROM(char address, char len){
  char data[100];  //max length
  char checksum;
  char calcChecksum = 0;
  for(int i=0; i < len; i++){
    data[i] = EEPROM.read(address + i);
    calcChecksum ^= data[i];
  }
  data[len] = 0;  //terminate character array with null char indicating end of string
  checksum = EEPROM.read(address + len);

  if(calcChecksum != checksum){
    return "";
    //Serial.println("OH NO, BAD Checksum!     calc: " +(String)calcChecksum+ "    read: " + checksum);
  }
  return String(data);
}


void writeEEPROM(char address, String data){
  char calcChecksum = 0;
  int strLength = data.length();
  for(int i =0; i< strLength; i++){
    if(EEPROM.read(address+i) != data[i]) EEPROM.write(address+i, data[i]); //read current value at EEPROM address, only perform a write if value is different
    calcChecksum ^= data[i];
  }
  if(EEPROM.read(address+strLength) != calcChecksum) EEPROM.write(address+strLength, calcChecksum);  //again only write to EEPROM if necessary
  Serial.println("writing eeprom:  " + data + "   checksum: " + calcChecksum);
}
