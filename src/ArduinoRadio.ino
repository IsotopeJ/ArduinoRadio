#include <DennisMenu.h>

long cwToneFrequency;
String callsign;

String c[]= {"one","two"};
String d[] = {"three","four","five"};


MenuItem myMenu[3] = {
  //label       suffix  value   type      length    max   min     strVariable   intVariable       choices[]   numChoices  factoryReset
  {"CW Tone",   "Hz",   "0",    "int",    4,        1000, 100,    NULL,         &cwToneFrequency, c,          2,          " 100"},
  {"Callsign",  "",     "0",    "string", 5,        0,    0,      &callsign,    NULL,             d,          3,          "AB1CDE"},
  {"IF",        "Hz",   "0",    "int",    8,        0,    0,      NULL,         NULL,             NULL,       0,          "12000000"}
};

DennisMenu<3> test = {
  {
  //label       suffix  value   type      length    max   min     strVariable   intVariable       choices[]   numChoices  factoryReset
  {"CW Tone",   "Hz",   "0",    "int",    4,        1000, 100,    NULL,         &cwToneFrequency, c,          2,          " 100"},
  {"Callsign",  "",     "0",    "string", 5,        0,    0,      &callsign,    NULL,             d,          3,          "AB1CDE"},
  {"IF",        "Hz",   "0",    "int",    8,        0,    0,      NULL,         NULL,             NULL,       0,          "12000000"}
  }
};


DennisMenu<3> setupMenu = {
  {
  //label       suffix  value   type      length    max   min     strVariable   intVariable       choices[]   numChoices  factoryReset
  {"CW Tone",   "Hz",   "0",    "int",    4,        1000, 100,    NULL,         &cwToneFrequency, c,          2,          " 100"},
  {"Callsign",  "",     "0",    "string", 5,        0,    0,      &callsign,    NULL,             d,          3,          "AB1CDE"},
  {"IF",        "Hz",   "0",    "int",    8,        0,    0,      NULL,         NULL,             NULL,       0,          "12000000"}
  }
};


void setup() {
  // put your setup code here, to run once:

  Serial.println(setupMenu.label());
 
  Serial.begin(9600);
  while(!Serial){}
  Serial.println("Starting");


  writeEEPROM(0, "1000");
  writeEEPROM(5, "AB1NX");
  writeEEPROM(11, "14000001");

  String test = readEEPROM(0,20);
  Serial.println(test);

  //initialize menu item values from memory
  int memoryLocation = 0;
  for(int i=0; i< setupMenu.numberOfMenuItems; i++){
    String memValue = readEEPROM(memoryLocation, setupMenu.items[i].length);
    if(memValue.length() != 0){  //length 0 returned from readEEPROM indicates error
      setupMenu.items[i].value = memValue;
      setupMenu.setMenuVariables(setupMenu.items[i]);
    }
    else{
      //ERROR state, checksum is wrong
    }
    memoryLocation += (setupMenu.items[i].length + 1);  //skip extra spot since we have a checksum
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  
  for(int i=0; i< setupMenu.numberOfMenuItems; i++){
    Serial.println("Item:  " + setupMenu.items[i].label + "       value: "+setupMenu.items[i].value);
    if(setupMenu.items[i].strVariable != NULL) Serial.println("HAS String variable");
    if(setupMenu.items[i].intVariable != NULL) Serial.println("HAS int variable");
    for(int j =0; j< setupMenu.items[i].numChoices; j++){
      Serial.println("     "+setupMenu.items[i].choices[j]);
    }
  }
  while(true){};

}
