/*
  Reading Encoder using PWM mode


  created 10 Sept 2023
  by Sainul IA


*/
String inputString = "";
bool stringComplete = false;  // whether the string is complete
bool dataRead = false;
bool offsetRead = false;
int index;
//int pin = A1;

int correctedValue = 0; //tared angle - based on the startup value
int startValue[8] = {303, 924, 446, 236, 669, 371, 462, 853}; //starting angle [PUTGripper3FV1]
// int startValue[8] = {303, 924, 446, 236, 669, 371, 462, 853}; //starting angle [PUTGripper3FV2]


void setup() {
  // initialize serial:
  Serial.begin(1000000);
  // reserve 200 bytes for the inputString:
  inputString.reserve(100);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(3, OUTPUT);
  // for(int pin=7;pin<14;pin++){
  //   pinMode(pin, INPUT);
  // }
  
  //pinMode(A7, INPUT);
  // for(int pin=A0; pin<=A7; pin++){
  //   index = pin - A0;
  //   startValue[index] = analogRead(pin);
  // }

  // startValue[0] = 303;
  // startValue[1] = 924;
  // startValue[2] = 446;
  // startValue[3] = 236;
  // startValue[4] = 669;
  // startValue[5] = 371;
  // startValue[6] = 462;
  // startValue[7] = 853;

}

void loop() {

  // analogWrite(3, 112);
  // analogWrite(5, 112);
  // analogWrite(6, 112);

  inputString = "";
  if (stringComplete && dataRead) {
    String response = "";
    stringComplete = false;
    dataRead = false;
    // for(int pin=7; pin<14; pin++){
    //   unsigned long duration = GetPWM(pin);
    //   response = response + String(duration) + " ";
    //   //Serial.println(duration);
    //   //Serial.print(" ");
    //   //delay(50);
    // }
    // Serial.println(response);
    //Serial.print("152 158 365 ");
    for(int pin=A0; pin<=A7; pin++){
      int value = analogRead(pin);
      value = correctEncoderValue(value, pin);
      response = response + String(value) + " ";
      
    }
    Serial.println(response);
  }
  if (stringComplete && offsetRead) {
    String response = "";
    stringComplete = false;
    offsetRead = false;

    for(int pin=A0; pin<=A7; pin++){
      int value = analogRead(pin);
      // value = correctEncoderValue(value, pin);
      response = response + String(value) + " ";
      
    }
    Serial.println(response);
  }
  //Serial.println(analogRead(A0));
  //delay(100);
}

int correctEncoderValue(int value, int pin){
  //recalculate angle
  index = pin - A0;
  value = value - startValue[index]; //this tares the position

  if(value < 0) //if the calculated angle is negative, we need to "normalize" it
  {
  value = value + 1024; //correction for negative numbers (i.e. -15 becomes +345)
  }
  else
  {
    //do nothing
  }
  //Serial.print("Corrected angle: ");
  //Serial.println(correctedAngle, 2); //print the corrected/tared angle  
  return value;
}

unsigned long GetPWM(int pin){
  unsigned long highTime = pulseIn(pin, HIGH, 50000UL);  // 50 millisecond timeout
  
  unsigned long lowTime = pulseIn(pin, LOW, 50000UL);  // 50 millisecond timeout

  // pulseIn() returns zero on timeout
  if (highTime == 0 || lowTime == 0)
    return 0;  // HIGH == 100%,  LOW = 0%

  return (highTime);  // highTime as percentage of total cycle time
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == 'r') {
      dataRead = true;
    }
    if (inChar == 'o') {
      offsetRead = true;
    }      
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}