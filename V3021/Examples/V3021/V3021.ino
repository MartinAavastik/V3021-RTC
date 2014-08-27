#include <V3021.h>

// strPin, clkPin, ioPin
V3021 v3021(4, 3, 2);

void setup() {
  v3021.begin();
  Serial.begin(9600);
  
  v3021.allowClockChange();
  
  v3021.setSeconds(30);
  v3021.setMinutes(34); //
  v3021.setHours(14); // 
  v3021.setDays(19); // 
  v3021.setMonths(8); // 
  v3021.setYears(2014); // 
  
  v3021.disallowClockChange();
  
  Serial.println("Clock is set");
  
}

void loop() {
  
  Serial.print(v3021.readHours() );
  Serial.print(':');
  Serial.print(v3021.readMinutes() );
  Serial.print(':');
  Serial.print(v3021.readSeconds() );
  Serial.print(' ');
  
  Serial.print(v3021.readDays() );
  Serial.print('.');
  Serial.print(v3021.readMonths() );
  Serial.print('.');
  Serial.print(v3021.readYears() );
  Serial.println();
  
  delay(1000);
  
}
