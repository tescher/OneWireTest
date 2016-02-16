#include <OneWire.h>

// DS18S20 Temperature chip i/o
OneWire ds(10);  // on pin 10

float getTemp() {

  byte data[12];
  byte addr[8];
  int i;

  ds.reset_search();
  ds.search(addr);

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     

  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( int i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  byte LowByte = data[0];
  byte HighByte = data[1];
  int TReading = (HighByte << 8) + LowByte;
  byte SignBit = TReading & 0x8000;  // test most sig bit
  if (SignBit) // negative
  {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  int Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25

  Tc_100 = Tc_100 * 9 / 5 + 3200;       // Convert to Fahrenheit

  return (float) Tc_100 / 100.0;
}

void setup(void) {
  // initialize inputs/outputs
  // start serial port
  Serial.begin(9600);
}

void loop(void) {

  Serial.println(getTemp());
  delay(2000);

}
