void setup() {
  Serial.begin(4800); // initialize serial communication at 9600 baud
  pinMode(13, OUTPUT);
}

void loop() {
  if (Serial.available()) { // check if data is available to read
    int value = Serial.read(); // read the integer value from the serial port
    
    if (value =='2') {
      digitalWrite(13, HIGH);
      delayMicroseconds(1000);
    }
    else if(value=='1')
      digitalWrite(13, LOW);
      delayMicroseconds(1000);
    }
  }


