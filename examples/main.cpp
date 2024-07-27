#include <Arduino.h>
//#define DEBUG
#include <TCode.h>

#if CONFIG_IDF_TARGET_ESP32 // ESP32/PICO-D4
#include "esp32/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32C3
#include "esp32c3/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32C6
#include "esp32c6/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32H2
#include "esp32h2/rom/rtc.h"
#else 
#error Target CONFIG_IDF_TARGET is not supported
#endif

using namespace TCode;


TButton button(12,"Test");
TCodeAxis stroke_axis("Stroke",{AxisType::Linear,0},0.5f);
TCodeAxis surge_axis("Surge",{AxisType::Linear,1},0.5f);
TCodeAxis sway_axis("Sway",{AxisType::Linear,2},0.5f);
TCodeAxis twist_axis("Twist",{AxisType::Rotation,0},0.5f);
TCodeAxis roll_axis("Roll",{AxisType::Rotation,1},0.5f);
TCodeAxis pitch_axis("Pitch",{AxisType::Rotation,2},0.5f);
TCodeAxis vibe0_axis("Vibe 1",{AxisType::Vibration,0},0.0f);
TCodeAxis vibe1_axis("Vibe 2",{AxisType::Vibration,1},0.0f);
TCodeAxis valve_axis("Valve",{AxisType::Auxiliary,0},0.0f);
TCodeAxis suck_axis("Suck",{AxisType::Auxiliary,1},0.0f);
TCodeAxis lube_axis("Lube",{AxisType::Auxiliary,2},0.0f);
#define axis_count 11
TCodeAxis* axis_pointers[axis_count] = {&stroke_axis,&surge_axis,&sway_axis,&twist_axis,&roll_axis,&pitch_axis,&vibe0_axis,&vibe1_axis,&valve_axis,&suck_axis,&lube_axis};

TCodeManager tcode;
Settings::SettingsESP32 settings(DEFAULT_FILE_NAME);

void verbose_print_reset_reason(int reason)
{
  switch ( reason)
  {
    case 1  : Serial.println ("Vbat power on reset");break;
    case 3  : Serial.println ("Software reset digital core");break;
    case 4  : Serial.println ("Legacy watch dog reset digital core");break;
    case 5  : Serial.println ("Deep Sleep reset digital core");break;
    case 6  : Serial.println ("Reset by SLC module, reset digital core");break;
    case 7  : Serial.println ("Timer Group0 Watch dog reset digital core");break;
    case 8  : Serial.println ("Timer Group1 Watch dog reset digital core");break;
    case 9  : Serial.println ("RTC Watch dog Reset digital core");break;
    case 10 : Serial.println ("Instrusion tested to reset CPU");break;
    case 11 : Serial.println ("Time Group reset CPU");break;
    case 12 : Serial.println ("Software reset CPU");break;
    case 13 : Serial.println ("RTC Watch dog Reset CPU");break;
    case 14 : Serial.println ("for APP CPU, reseted by PRO CPU");break;
    case 15 : Serial.println ("Reset when the vdd voltage is not stable");break;
    case 16 : Serial.println ("RTC Watch dog reset digital core and rtc module");break;
    default : Serial.println ("NO_MEAN");
  }
}

void print_reset_reason()
{
  delay(1000);
  Serial.print("CPU0 reset reason: ");
  verbose_print_reset_reason(rtc_get_reset_reason(0));

  Serial.print("CPU1 reset reason: ");
  verbose_print_reset_reason(rtc_get_reset_reason(1));
}

void print_heap()
{
  multi_heap_info_t info_value;
  heap_caps_get_info(&info_value,MALLOC_CAP_DEFAULT);
  size_t total_size = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
  Serial.println("==================");
  Serial.println("    HEAP INFO    ");
  Serial.println("==================");

  float percent_bytes_allocated = ((float)info_value.total_free_bytes / (float)total_size)  * 100.0f;

  Serial.print("Bytes Free :");
  Serial.print(percent_bytes_allocated);
  Serial.println('%');
}

void print_sys()
{
  Settings::SettingsUsage usage;
  settings.getSystemUsage(usage);
  Serial.println("==================");
  Serial.println("  FILE SYS INFO    ");
  Serial.println("==================");
  Serial.print("Space Available:");
  Serial.println(usage.spaceAvailable);
  Serial.print("File Size:");
  Serial.println(usage.sizeOfFile);
  Serial.print("Space Used by SPIFFS:");
  Serial.println(usage.spaceUsed);
}

void setup() {
  LogHandler::setLogLevel(LogLevel::VERBOSE);
  Serial.begin(115200);
  
  settings.init();
  tcode.setSettingManager(&settings);
  tcode.setOutputStream(&Serial);

  for(int i = 0; i < axis_count; i++)
  {
    tcode.registerAxis(axis_pointers[i]);
  }

  tcode.registerInterface(&button);

  print_reset_reason();
  print_sys();
  print_heap();
  Serial.println("================");
  
  delay(1000);
  
}

void loop() {
  bool received = false;
  while(Serial.available())
  {
    tcode.read((char)Serial.read());
    if(received == false)
      received = true;
  }
  tcode.updateInterfaces();
  
  if(received)
  {
    //Serial.println((int)logging.getLogLevel());
    //print_sys();
    //print_heap();
  }

  for (int i = 0; i < axis_count; i++)
  {
    if (axis_pointers[i]->changed())
    {
      float value = axis_pointers[i]->getPosition();
      Serial.print(">");
      Serial.print(i);
      Serial.print(":");
      Serial.println(value);
    }
  }

  /*
  Serial.print(">A0:");
  Serial.println(test_axis.getPosition());
  */
}

