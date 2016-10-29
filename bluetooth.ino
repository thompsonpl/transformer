#define CE 2
#define TX 11
#define RX 10
#define STATE 12

#define UL 6
#define UR 9
#define DL 3
#define DR 5

#define MEMSIZE 3

#include <SoftwareSerial.h>

SoftwareSerial Bluetooth(RX,TX);

byte LInputMemory[MEMSIZE];
byte RInputMemory[MEMSIZE];

float LAverage = 0;
float RAverage = 0;

void moveMemory(byte LInput, byte RInput){
  for(byte i=0; i<MEMSIZE-1; i++){
    LInputMemory[i] = LInputMemory[i+1];
    RInputMemory[i] = RInputMemory[i+1];
  }
  LInputMemory[MEMSIZE-1] = LInput;
  RInputMemory[MEMSIZE-1] = RInput;
}

void calcAverage(){
  LAverage = 0;
  RAverage = 0;
  for(byte i=0; i<MEMSIZE; i++){
    LAverage += float(LInputMemory[i])/float(MEMSIZE);
    RAverage += float(RInputMemory[i])/float(MEMSIZE);  
  }
  Serial.print("LAverage: ");
  Serial.println(LAverage);
  Serial.print("RAverage: ");
  Serial.println(RAverage);
}

void printMemory(){
  Serial.print("LMemory: [");
  for(byte i =0; i<MEMSIZE; i++){
    Serial.print(LInputMemory[i]);
    Serial.print(',');
  }
  Serial.println(']');
  
  Serial.print("RMemory: [");
  for(byte i =0; i<MEMSIZE; i++){
    Serial.print(RInputMemory[i]);
    Serial.print(',');
  }
  Serial.println(']');
}

void setup(){
  pinMode(CE, OUTPUT);
  pinMode(UL, OUTPUT);
  pinMode(UR, OUTPUT);
  pinMode(DL, OUTPUT);
  pinMode(DR, OUTPUT);
  digitalWrite(CE, HIGH);
  Serial.begin(115200);
  Bluetooth.begin(9600);
  Serial.println("Module started");
  
  for(byte i=0; i<MEMSIZE; i++){
    LInputMemory[i] = 0;
    RInputMemory[i] = 0;
  }
}

void loop(){
  if(Bluetooth.available()){
    String command = Bluetooth.readStringUntil('\n');
    command.trim();
    int division = command.indexOf(' ');
    int lPow = constrain(command.substring(0,division).toInt(), 0, 255);
    int rPow = constrain(command.substring(division,command.length()).toInt(), 0, 255);
    /*
    Serial.print("LPOWER: ");
    Serial.println(lPow);
    Serial.print("RPOWER: ");
    Serial.println(rPow);
    */
    
    moveMemory(lPow, rPow);
    calcAverage();
    
    analogWrite(UL, LAverage);
    analogWrite(DL, LAverage);
    analogWrite(UR, RAverage);
    analogWrite(DR, RAverage);
    
    printMemory(); 
  }
}
