#include <Arduino.h>
#include <TCode.h>

Settings settingManager;
TCodeAxis a1("Testing");

void setup() {
  Serial.begin(115200);
  a1.set(9999,TCode_Axis_Extention_Type::Time,100,TCode_Axis_Ramp_Type::EaseInOut);
  a1.set(0,TCode_Axis_Extention_Type::Time,100,TCode_Axis_Ramp_Type::EaseInOut);
  a1.set(9999,TCode_Axis_Extention_Type::Time,100,TCode_Axis_Ramp_Type::EaseInOut);
  a1.set(0,TCode_Axis_Extention_Type::Time,100,TCode_Axis_Ramp_Type::EaseInOut);
  a1.set(9999,TCode_Axis_Extention_Type::Time,100,TCode_Axis_Ramp_Type::EaseInOut);
}

void loop() {
  int position = a1.getPosition();
  Serial.println(position);
}
