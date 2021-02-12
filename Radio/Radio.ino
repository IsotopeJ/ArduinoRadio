
#include <EEPROM.h>
#include <Radio.h>

#define NUMBER_OF_MENUITEMS 3

const char alphanumeric[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";



int cwToneFrequency;
String callsign;

String c[]= {"one","two"};
String d[] = {"three","four","five"};


MenuItem menuItems[NUMBER_OF_MENUITEMS] = {
  //label       suffix  value type  length  strVariable   intVariable       choices[]   numChoices
  {"CW Tone",   "Hz",  "0",   "int",    4,  NULL,         &cwToneFrequency, c,          2},
  {"Callsign",  "",    "0",   "string", 5,  &callsign,    NULL,             d,          3},
  {"IF",        "Hz",  "0",   "int",    8,  NULL,         NULL,             NULL,       0}
};


void updateMenuVariables(MenuItem m){
  if(m.strVariable != NULL) m.*strVariable = m.value.trim();
  if(m.intVariable != NULL) m.*intVariable = m.value.trim().toInt();
}



void setup() {
  // put your setup code here, to run once:

 
  Serial.begin(9600);
  while(!Serial){}
  Serial.println("Starting");

  menuItems[1].value = "test";
  
 // for(int i = 0; i < 36; i++){
 //   EEPROM.write(i, alphanumeric[i]);
 // }

  writeEEPROM(0, "1000");
  writeEEPROM(5, "AB1NX");
  writeEEPROM(11, "14000001");

  String test = readEEPROM(0,20);
  Serial.println(test);

  //initialize menu item values from memory
  int memoryLocation = 0;
  for(int i = 0; i< NUMBER_OF_MENUITEMS; i++){
    String memValue = readEEPROM(memoryLocation, menuItems[i].length);
    if(memValue.length() != 0){
      menuItems[i].value = memValue;
      updateMenuVariables(menuItems[i]);
    }
    else{
      //ERROR state, checksum is wrong
    }
    memoryLocation += (menuItems[i].length + 1);  //skip extra spot since we have a checksum
  }


}

void loop() {
  // put your main code here, to run repeatedly:

  
  for(MenuItem m : menuItems){
    Serial.println("Item:  " + m.label + "       value: "+m.value);
    if(m.strVariable != NULL) Serial.println("HAS String variable");
    if(m.intVariable != NULL) Serial.println("HAS int variable");
    for(int i =0; i< m.numChoices; i++){
      Serial.println("     "+m.choices[i]);
    }

  }
  while(true){};

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
