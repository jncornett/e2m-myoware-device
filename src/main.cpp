/**
 * Sends sensor data over Bluetooth (classic) serial.
 */

#include <Arduino.h>
#define ENABLE_BLUETOOTH
#ifdef ENABLE_BLUETOOTH
#include <BluetoothSerial.h>
#endif

#include "filter.h"

#define SENSOR_PIN A3
#define SAMPLE_RATE_HZ 100
#define HAPTIC_THRESHOLD 300
#define HAPTIC_SCALE 1024
#define BLUETOOTH_DEVICE_NAME "E2MMyowareDevice"
#define IDLE_DELAY_MS 1000

const int sample_delay_ms = 1000 / SAMPLE_RATE_HZ;

short feedback(short input)
{
  if (input > HAPTIC_THRESHOLD)
    return HAPTIC_SCALE;
  else
    return 0;
}

#ifdef ENABLE_BLUETOOTH
void log_data(BluetoothSerial *device, short raw, short filtered, short feedback)
{
  device->printf("%d %d %d\n", raw, filtered, feedback);
}
#endif

#ifdef ENABLE_BLUETOOTH
bool prev_is_connected = false;
BluetoothSerial SerialBT;
#endif

#ifdef ENABLE_BLUETOOTH
bool update_bluetooth_connection_state()
{
  bool is_connected = SerialBT.hasClient();
  if (is_connected && !prev_is_connected)
  {
    // transitioning from disconnected to connected
    Serial.println("Connected!");
  }
  else if (!is_connected && prev_is_connected)
  {
    Serial.println("Disconnected :(");
  }
  prev_is_connected = is_connected;
  return is_connected;
}
#endif

void setup()
{
  Serial.begin(115200);
#ifdef ENABLE_BLUETOOTH
  SerialBT.begin(BLUETOOTH_DEVICE_NAME);
#endif
  Serial.println();
}

void loop()
{
#ifdef ENABLE_BLUETOOTH
  bool bluetooth_is_connected = update_bluetooth_connection_state();
#endif

  // compute sensor readings
  int raw_value = analogRead(SENSOR_PIN);
  short filtered = filter(raw_value);
  short haptic = feedback(filtered);

#ifdef ENABLE_BLUETOOTH
  // log sensor readings
  if (bluetooth_is_connected)
  {
    log_data(&SerialBT, raw_value, filtered, haptic);
  }
#endif

  Serial.printf("%d %d %d\n", raw_value, filtered, haptic);

  delay(sample_delay_ms);
}
