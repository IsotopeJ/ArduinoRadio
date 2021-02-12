#include "ArduinoRadio.h"

void updateMenuVariables(MenuItem m){
  if(m.strVariable != NULL) {
    m.value.trim();
    *(m.strVariable) = m.value;
  }

  if(m.intVariable != NULL) {
    m.value.trim();
    *(m.intVariable) = m.value.toInt();
  }
}
