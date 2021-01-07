#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Abhi";
char pass[] = "abhiveda";
 
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "_bDm8icSFp0vLRuxoj_T7DxS0sLvMTLK";
Servo servo;
const int trigPin = D5; 
const int echoPin = D6;
const int buzzerPin = D1; 
const int button = D2;
const int Gas = D4;
const int Gas_analog = A0;
long duration;
int distance;
int count;
int cutoff;
int off=0;
int gassensorAnalog;
int gassensorDigital;
int off_for_gas=0;
int sound=0;
int manual=1;
int value;
BLYNK_WRITE(V1)
{
  value = param.asInt(); // Get value as integer
}
int sound_off;
BLYNK_WRITE(V3)
{
  sound_off = param.asInt(); // Get value as integer
}
void setup() 
{
servo.attach(D0); //D0
servo.write(0);
delay(200);      
pinMode(Gas, INPUT);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(buzzerPin,OUTPUT);
pinMode(button,INPUT);
Serial.begin(9600);// Starts the serial communication
Blynk.begin(auth, ssid, pass);
}


void loop() 
{
  Blynk.run();
//write here
  if (value==1)
  {
    off=0;
    (servo.write(off));
  Blynk.virtualWrite(V0, "System On" );
  }
if(off==0 && value==1)
{
Blynk.virtualWrite(V0, "System On");
Blynk.virtualWrite(V1, 1);
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.0343/2;
delay(1000);


if (distance > 245)
{
  if (sound_off==0 && manual==0)
  {
    sound=1;
    manual=1;
    Blynk.virtualWrite(V2, "Sound Off" );    
  Blynk.virtualWrite(V3, 0);
  count=0;
  cutoff=0;
    }
  if (count<10)
  {
  Blynk.virtualWrite(V8, "Waiting for time expiry " );
  Blynk.virtualWrite(V4, distance );
  Blynk.virtualWrite(V2, "Sound Off" );    
  Blynk.virtualWrite(V3, 0);
  Serial.println("Waiting for time expiry ");
  Serial.print("Current Distance: ");
  Serial.println(distance);
  digitalWrite(buzzerPin, LOW);
count += 1;
sound_off=0;
sound=1;
manual=1;
}
if(sound_off==0){delay(1000);Blynk.virtualWrite(V2, "Sound Off" );    
  Blynk.virtualWrite(V3, 0);}
if (sound_off==1)
{
  sound=1;
  Blynk.virtualWrite(V3, 1 );
  Blynk.virtualWrite(V2, "Sound On" ); 
  }
  if (count>9 && sound ==1)
{ 
  sound_off=1;
  Blynk.virtualWrite(V3, 1 );
  Blynk.virtualWrite(V2, "Sound On" ); 
  manual=0;   
  //make change here
  cutoff +=1;
  Serial.println("Playing Sound");
  tone(buzzerPin,2000);
  if(sound_off==0){
  delay(1000);
  Blynk.virtualWrite(V2, "Sound Off" );    
  Blynk.virtualWrite(V3, 0);}
  else{
    delay(1000);}
  //sound code
  noTone(buzzerPin);
  if(cutoff > 9)
  {
    off=180;
    (servo.write(off));
    delay(1000);
    value=0;
    Blynk.virtualWrite(V1, value );
  }
}
}
else
{
  Serial.print("Person present at distance:");
  Blynk.virtualWrite(V8, "Person Present" );
  Blynk.virtualWrite(V4, distance );
  Serial.println(distance);
digitalWrite(buzzerPin, LOW);
Blynk.virtualWrite(V2, "Sound Off" );    
  Blynk.virtualWrite(V3, 0);
count=0;
cutoff=0;
off=0;
(servo.write(0));
sound=0;
}

gassensorAnalog = analogRead(Gas_analog);
gassensorDigital = digitalRead(Gas);
Blynk.virtualWrite(V6, gassensorAnalog );
Blynk.virtualWrite(V5, gassensorDigital );
  Serial.print("Gas Sensor: ");
  Serial.print(gassensorAnalog);
  Serial.print("\t");
  Serial.print("Gas Class: ");
  Serial.print(gassensorDigital);
  Serial.print("\t");
  Serial.print("\t");
  
  if (gassensorAnalog > 1000) 
  {
    Blynk.virtualWrite(V7, "Gas is Leaking" );
    Serial.println("Gas");
    digitalWrite (buzzerPin, HIGH) ; //send tone
    delay(500);
    digitalWrite (buzzerPin, LOW) ;
    off_for_gas+=1;//no tone
    if(off_for_gas>9)
    {
    off=180;
    (servo.write(off));
    delay(1000);
    }
  }
  else
  {
    Serial.println("No Gas");
    Blynk.virtualWrite(V7, "No Leak" );
    off_for_gas=0;
    (servo.write(0));
  }
  delay(100);
}
else 
{
 sound_off=0;
 duration=0;
 distance=0;
 count=0;
 cutoff=0;
 off=180;
 sound=1;
 manual=1;
 gassensorAnalog=0;
 gassensorDigital=0;
 off_for_gas=180;
(servo.write(off));
     digitalWrite (buzzerPin, LOW) ;
      Blynk.virtualWrite(V0, "System Off" );
      Blynk.virtualWrite(V1, value );
      Blynk.virtualWrite(V2, "Sound Off" );
      Blynk.virtualWrite(V3, 0 );
      Blynk.virtualWrite(V4, 0 );
      Blynk.virtualWrite(V8, "System Off" );
      Blynk.virtualWrite(V6, 0 );
      Blynk.virtualWrite(V7, "System Off" );
      Blynk.virtualWrite(V5, "---" );
      }
}
