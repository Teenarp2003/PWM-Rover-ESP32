#define CH1 39
#define CH2 36
#define CH3 16
#define CH4 17
#define CH5 21
#define CH6 22

#define pwmA 25
#define pwmAR 32
#define pwmB 5
#define pwmBR 4
#define pfreq 5000

int MotorSpeedA = 0;
int MotorSpeedB = 0;
int MotorDirA = 0;
int MotorDirB = 0;
int rcCH1, rcCH2, rcCH3, rcCH4, rcCH5, rcCH6;

void mControlA(int mspeed, int mdir) {
  if (mdir == 0) {
    analogWriteFrequency(pwmA, pfreq);
    analogWrite(pwmA, mspeed);
    analogWriteFrequency(pwmAR, pfreq);
    analogWrite(pwmAR, 0 );
    
    Serial.print("|| A - forward ");
  } else if (mdir == 1) {
    // Motor forward
    analogWriteFrequency(pwmAR, pfreq);
    analogWrite(pwmAR, mspeed);
    analogWriteFrequency(pwmA, pfreq);
    analogWrite(pwmA, 0);
    Serial.print("|| A - backward ");
  }
  else{
    analogWriteFrequency(pwmAR, pfreq);
    analogWrite(pwmAR,0);
    analogWriteFrequency(pwmA, pfreq);
    analogWrite(pwmA, 0);
  }
}

void mControlB(int mspeed, int mdir) {
  if (mdir == 0) {
    // Motor backward
    Serial.print("|| B - forward || ");
    analogWriteFrequency(pwmB, pfreq);
    analogWrite(pwmB, mspeed);
    analogWriteFrequency(pwmBR, pfreq);
    analogWrite(pwmBR, 0);
  } else if (mdir == 1) {
    // Motor forward
    analogWriteFrequency(pwmBR, 2000);
    analogWrite(pwmBR, mspeed);
    analogWriteFrequency(pwmB, 2000);
    analogWrite(pwmB, 0);
    Serial.print("|| B - backward || ");
  } else{
    analogWriteFrequency(pwmBR, pfreq);
    analogWrite(pwmBR,0);
    analogWriteFrequency(pwmB, pfreq);
    analogWrite(pwmB, 0);
  }
}

int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 25000);
  if (ch < 10) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

bool readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void setup() {
  Serial.begin(115200);
  pinMode(CH1, INPUT);  
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
  pinMode(CH6, INPUT);

  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(pwmAR, OUTPUT);
  pinMode(pwmBR, OUTPUT);

}

void loop() {
  rcCH1 = readChannel(CH1, -100, 100, 0);
  rcCH2 = readChannel(CH2, -100, 100, 0);
  rcCH3 = readChannel(CH3, 0,155, 0); 
  rcCH4 = readChannel(CH4, -255, 255, 0);
  rcCH5 = readChannel(CH5, -255, 255, 0);
  rcCH6 = readChannel(CH6, -255, 255, 0);
  Serial.print("Ch1 = ");
  Serial.print(rcCH1);

  Serial.print(" Ch2 = ");
  Serial.print(rcCH2);

  Serial.print(" Ch3 = ");
  Serial.print(rcCH3);

  Serial.print(" Ch4 = ");
  Serial.print(rcCH4);
 
  Serial.print(" Ch5 = ");
  Serial.print(rcCH5);

  Serial.print(" Ch6 = ");
  Serial.print(rcCH6);
  if (rcCH2 > 0) {
    MotorDirA = 0;
    MotorDirB = 0;
  } else if ( rcCH2 < 0){
    MotorDirA = 1;
    MotorDirB = 1;
  }
  
  if (rcCH1 >= 10 || rcCH2 >= 10 || rcCH1 <= -10 || rcCH2 <= -10 ){
    if(rcCH5 > 10){
      MotorSpeedA = rcCH3 + abs(rcCH2);
      MotorSpeedB = rcCH3 + abs(rcCH2);
      // Set left/right offset with channel 1 value
      MotorSpeedA += rcCH1;
      MotorSpeedB -= rcCH1;
    }
  }
  else{
    MotorSpeedA = 0;
    MotorSpeedB = 0;
  }
  // Ensure that speeds are between 0 and 255
  MotorSpeedA = constrain(MotorSpeedA, 0, 253);
  MotorSpeedB = constrain(MotorSpeedB, 0, 253);

  mControlA(MotorSpeedA, MotorDirA);
  mControlB(MotorSpeedB, MotorDirB);

  Serial.print("Motor A Speed = ");
  Serial.print(MotorSpeedA);
  Serial.print(" | Motor B Speed = ");
  Serial.println(MotorSpeedB);

}
