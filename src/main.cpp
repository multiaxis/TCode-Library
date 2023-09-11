#include <Arduino.h>
#include <TCode.h>

TCodeAxis test_axis("Linear1",{TCode_Channel_Type::Linear,0});
TCode tcode_manager;

void setup() {
  Serial.begin(115200);
  tcode_manager.registerAxis(&test_axis);
}

void loop() {
  while(Serial.available())
  {
    tcode_manager.inputChar(Serial.read());
  }
  int position = tcode_manager.axisRead("Linear1");
  if(test_axis.changed())
  {
    Serial.println(position);
  } 
}
