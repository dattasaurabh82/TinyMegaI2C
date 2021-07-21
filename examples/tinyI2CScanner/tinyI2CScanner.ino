#include <TinyMegaI2C.h>

void setup() {
  TinyMegaI2C.init();

  Serial.begin(115200);
  while (!Serial) {
  }

  Serial.println(F("\nI2C Scanner"));
  delay(1000);
}

void loop() {

  byte address;
  //  byte error, address;  //variable for error and I2C address
  int nDevices;

  Serial.println(F("Scanning..."));
  nDevices = 0;

  for (address = 1; address < 127; address++ ) {
    if  (TinyMegaI2C.start(address, 0)) {
      nDevices++;
      Serial.print("I2C device found at address 0x");
      Serial.print(address, HEX);
      Serial.println("  !");
    }
  }
  if (nDevices == 0)
    Serial.println(F("No I2C devices found\n"));
  else
    Serial.println(F("done\n"));
  delay(5000);
}
