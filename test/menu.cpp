#include <Arduino.h>
#include <ArduinoUnitTests.h>
#include <ArduinoRadio.h>


unittest(update_menu_variables) {
  int cwToneFrequency = 33;
  String callsign = "unset";

  String c[]= {"one","two"};
  String d[] = {"three","four","five"};

  MenuItem menuItems[3] = {
    //label       suffix  value type  length  strVariable   intVariable       choices[]   numChoices
    {"CW Tone",   "Hz",  "0",   "int",    4,  NULL,         &cwToneFrequency, c,          2},
    {"Callsign",  "",    "0",   "string", 5,  &callsign,    NULL,             d,          3},
    {"IF",        "Hz",  "0",   "int",    8,  NULL,         NULL,             NULL,       0}
  };

  // no surprises, initial conditions are as we just set them
  assertEqual("0", menuItems[0].value);
  assertEqual(33, *(menuItems[0].intVariable));
  assertEqual("0", menuItems[1].value);
  assertEqual("unset", *(menuItems[1].strVariable));

  // updating the integer value produces integer zero instead of "0", overwriting 33
  updateMenuVariables(menuItems[0]);
  assertEqual(0, *(menuItems[0].intVariable));
  assertEqual("unset", *(menuItems[1].strVariable));

  // updating the string value produces string "0", overwriting "unset"
  updateMenuVariables(menuItems[1]);
  assertEqual(0, *(menuItems[0].intVariable));
  assertEqual("0", *(menuItems[1].strVariable));

}

unittest_main()
