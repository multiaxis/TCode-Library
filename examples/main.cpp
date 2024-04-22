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

SettingsESP32 settings(DEFAULT_FILE_NAME);

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
  Serial.println("=== HEAP INFO ===");

  float percent_bytes_allocated = ((float)info_value.total_free_bytes / (float)total_size)  * 100.0f;

  Serial.print("Bytes Free :");
  Serial.print(percent_bytes_allocated);
  Serial.println('%');
  Serial.println("==================");
}

void setup() {
  Serial.begin(115200);
  esp_log_level_set(SETTING_MANAGMENT_TAG, ESP_LOG_VERBOSE);
  print_reset_reason();
  TCode.setSettingManager(&settings);
  TCode.setOutputStream(&Serial);

  SettingsUsage usage;
  settings.getSystemUsage(usage);
  Serial.println("================");
  Serial.println("    File Sys    ");
  Serial.println("================");
  Serial.print("Space Available:");
  Serial.println(usage.spaceAvailable);
  Serial.print("File Size:");
  Serial.println(usage.sizeOfFile);
  Serial.print("Space Used by SPIFFS:");
  Serial.println(usage.spaceUsed);
  Serial.println("================");

  TCode.registerAxis("Stroke", {ChannelType::LINEAR, 0}, 0.5f);
  TCode.registerAxis("Surge", {ChannelType::LINEAR, 1}, 0.5f);
  TCode.registerAxis("Sway", {ChannelType::LINEAR, 2}, 0.5f);
  TCode.registerAxis("Twist", {ChannelType::ROTATION, 0}, 0.5f;
  TCode.registerAxis("Roll", {ChannelType::ROTATION, 1}, 0.5f);
  TCode.registerAxis("Pitch", {ChannelType::ROTATION, 2}, 0.5f);
  TCode.registerAxis("Vibe 1",{ChannelType::VIBRATION, 0}, 0);
  TCode.registerAxis("Vibe 2",{ChannelType::VIBRATION, 1}, 0);
  TCode.registerAxis("Valve", {ChannelType::AUXILIARY, 0}, 0);
  TCode.registerAxis("Suck", {ChannelType::AUXILIARY, 1}, 0);
  TCode.registerAxis("Lube", {ChannelType::AUXILIARY, 2}, 0);

  TCode.registerButton(12, "Test");

  Serial.println();
  Serial.println("================");
  delay(1000);
}

void loop() {
  bool received = false;
  while(Serial.available())
  {
    TCode.inputByte(Serial.read());
    if(received == false)
      received = true;
  }

  if(received)
  {
    print_heap();
  }
  
  /*
  Serial.print(">A0:");
  Serial.println(test_axis.getPosition());
  */
}

