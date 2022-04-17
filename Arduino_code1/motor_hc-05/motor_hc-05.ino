#include <Servo.h>

const int servo1 = 9;       // first servo
const int servo2 = 10;       // second servo
const int joyH = A0;        // L/R Parallax Thumbstick
const int joyV = A1;        // U/D Parallax Thumbstick

int servoVal;           // variable to read the value from the analog pin

Servo myservo1;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo

//--------------------------
char incomingByte = ' ';   // for incoming serial data

#define LED 13
#define BAUDRATE 57600
#define DEBUGOUTPUT 0

// checksum variables
byte payloadChecksum = 0;
byte CalculatedChecksum;
byte checksum = 0;              //data type byte stores an 8-bit unsigned number, from 0 to 255
int payloadLength = 0;
byte payloadData[64] = {0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;
#define LED 13
#define BAUDRATE 57600
#define DEBUGOUTPUT 0
#define powercontrol 10
int Raspipin = 6;


long lastReceivedPacket = 0;
boolean bigPacket = false;
boolean brainwave = false;
byte generatedChecksum = 0;

void setup() {
  
  myservo1.attach(servo1);  // attaches the servo
  myservo2.attach(servo2);  // attaches the servo


  //--------------
  pinMode(Raspipin, OUTPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(BAUDRATE); 
  Serial.print("PoorQuality");
  Serial.print("\t");
  Serial.print("Time since last packet");
  Serial.print("\t");
  Serial.print("Attention");
  Serial.print("\t");
  Serial.print("Delta");
  Serial.print("\t");
  Serial.print("Low Alpha");
  Serial.print("\t");
  Serial.print("High Alpha");
  Serial.print("\t");
  Serial.print("Low Beta");
  Serial.print("\t");
  Serial.print("High Beta");
  Serial.print("\t");
  Serial.print("Low Gamma");
  Serial.print("\t");
  Serial.print("Mid Gamma");
  Serial.print("\n");

}


byte ReadOneByte() {
  int ByteRead;

  while(!Serial.available());
  ByteRead = Serial.read();

#if DEBUGOUTPUT  
  Serial.print((char)ByteRead);   // echo the same byte out the USB serial (for debug purposes)
#endif

  return ByteRead;
}

unsigned int delta_wave = 0;
unsigned int theta_wave = 0;
unsigned int low_alpha_wave = 0;
unsigned int high_alpha_wave = 0;
unsigned int low_beta_wave = 0;
unsigned int high_beta_wave = 0;
unsigned int low_gamma_wave = 0;
unsigned int mid_gamma_wave = 0;
void read_waves(int i) {

  delta_wave = read_3byte_int(i);
  i+=3;
  theta_wave = read_3byte_int(i);
  i+=3;
  low_alpha_wave = read_3byte_int(i);
  i+=3;
  high_alpha_wave = read_3byte_int(i);
  i+=3;
  low_beta_wave = read_3byte_int(i);
  i+=3;
  high_beta_wave = read_3byte_int(i);
  i+=3;
  low_gamma_wave = read_3byte_int(i);
  i+=3;
  mid_gamma_wave = read_3byte_int(i);
}

int read_3byte_int(int i) {
  return ((payloadData[i] << 16) + (payloadData[i+1] << 8) + payloadData[i+2]);
}


void loop() {

//------------------------------
    outputJoystick();
  // Read the horizontal joystick value  (value between 0 and 1023)
    servoVal = analogRead(joyH);          
    servoVal = map(servoVal, 0, 1023, 0, 180);     // scale it to use it with the servo (result  between 0 and 180)

    myservo2.write(servoVal);                         // sets the servo position according to the scaled value    

    // Read the horizontal joystick value  (value between 0 and 1023)
    servoVal = analogRead(joyV);           
    servoVal = map(servoVal, 0, 1023, 70, 180);     // scale it to use it with the servo (result between 70 and 180)

    myservo1.write(servoVal);                           // sets the servo position according to the scaled value

    delay(15);                                       // waits for the servo to get there

    //------------------------------

   
   
  // Look for sync bytes
  if(ReadOneByte() == 0xAA) {
if(ReadOneByte() == 0xAA) 

      payloadLength = ReadOneByte();
      if(payloadLength > 169)                      //Payload length can not be greater than 169
          return;

      generatedChecksum = 0;        
      for(int i = 0; i < payloadLength; i++) {  
        payloadData[i] = ReadOneByte();            //Read payload into memory
        generatedChecksum += payloadData[i];
      }   

      checksum = ReadOneByte();                      //Read checksum byte from stream      
      generatedChecksum = 255 - generatedChecksum;   //Take one's compliment of generated checksum

        if(checksum == generatedChecksum) {    

        poorQuality = 200;
        attention = 0;
        meditation = 0;
        
        brainwave = false;
        for(int i = 0; i < payloadLength; i++) {    // Parse the payload
          switch (payloadData[i]) {
          case 2:
            i++;            
            poorQuality = payloadData[i];
            bigPacket = true;            
            break;
          case 4:
            i++;
            attention = payloadData[i];                        
            break;
          case 5:
            i++;
            meditation = payloadData[i];
            break;
          case 0x80:
            i = i + 3;
            break;
          case 0x83:
            i++;      
             brainwave = true;
            byte vlen = payloadData[i];
           // Serial.print(vlen, DEC);
          ///  Serial.println();
            read_waves(i+1);
            i += vlen; // i = i + vlen
            break;
          } // switch
        } // for loop

#if !DEBUGOUTPUT

        // *** Add your code here ***

        if(bigPacket) {
          if(poorQuality == 0){
            digitalWrite(LED, HIGH);
            Serial.print(poorQuality, DEC);
            Serial.print("\t");
            Serial.print(millis() - lastReceivedPacket, DEC);
            lastReceivedPacket = millis();
            Serial.print("\t");
            Serial.print(attention, DEC);
            Serial.print("\t");
            Serial.print(delta_wave, DEC);
            Serial.print("\t");
            Serial.print(theta_wave, DEC);
            Serial.print("\t");
            Serial.print(low_alpha_wave, DEC);
            Serial.print("\t");
            Serial.print(high_alpha_wave, DEC);
            Serial.print("\t");
            Serial.print(low_beta_wave, DEC);
            Serial.print("\t");
            Serial.print(high_beta_wave, DEC);
            Serial.print("\t");
            Serial.print(low_gamma_wave, DEC);
            Serial.print("\t");
            Serial.print(mid_gamma_wave, DEC);
            Serial.print("\n");
          }
          else{
          digitalWrite(LED, LOW);
          Serial.print(poorQuality, DEC);
          Serial.print("\t");
          Serial.print(millis() - lastReceivedPacket, DEC);
          lastReceivedPacket = millis();
          Serial.print("\t");
          Serial.print("\n");
          }

          if ( attention >= 20)
          {
            digitalWrite(Raspipin, HIGH);   // sets the LED on
            delay(10);                  // waits for a second
            digitalWrite(Raspipin, LOW);    // sets the LED off
            delay(10);
          }

 }                     
        }
#endif        
        bigPacket = false;        
      }
      else {
        // Checksum Error
      }  // end if else for checksum

// end if read 0xAA byte




}

void outputJoystick(){

    Serial.print(analogRead(joyH));
    Serial.print ("---"); 
    Serial.print(analogRead(joyV));
    Serial.println ("----------------");
}
