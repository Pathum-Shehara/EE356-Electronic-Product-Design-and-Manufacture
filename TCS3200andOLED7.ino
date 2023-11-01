//oled display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//On arduino UNO:       A4(SDA), A5(SCL)

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D //< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define s0 7       //TCS3200 pins wiring
#define s1 8
#define s2 4
#define s3 6
#define out 5

#define motorPin 3 //motor transistor is connected to pin 3

int data=0, Red=0, Green=0, Blue=0;
String strColor;
String strRed;
String strGreen;
String strBlue;
String str1;
String str2;

void setup(){
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  delay(2000);         // wait for initializing
  oled.clearDisplay(); // clear display

  oled.setTextColor(WHITE);     // text color

  strColor=String("White");
  
  pinMode(s0,OUTPUT);    //pin modes
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);

  //Serial.begin(9600);   //intialize the serial monitor baud rate
   
  digitalWrite(s0,HIGH); //Putting S0/S1 on HIGH/HIGH levels means the output frequency scalling is at 100% (recommended)
  digitalWrite(s1,HIGH); //LOW/LOW is off HIGH/LOW is 20% and LOW/HIGH is  2%
}

void loop() {
  oled.clearDisplay(); // clear display
  digitalWrite(s2,LOW);        //S2/S3 levels define which set of photodiodes we are using LOW/LOW is for RED LOW/HIGH is for Blue and HIGH/HIGH is for green
  digitalWrite(s3,LOW);

  //Serial.print("Red value= "); 
  GetData();                 //Executing GetData function to get the value
  data = map(data, 195, 0, 0, 255);
  percentageRGB();
  Red=data;
  strRed=String(Red);

  digitalWrite(s2,HIGH);
  digitalWrite(s3,HIGH);

  //Serial.print("Green value= ");
  GetData();
  data = map(data, 200, 0, 0, 255);
  percentageRGB();
  Green=data;  
  strGreen=String(Green);

  digitalWrite(s2,LOW);
  digitalWrite(s3,HIGH);

  //Serial.print("Blue value= ");
  GetData();
  data = map(data, 150, 0, 0, 255);
  percentageRGB();
  Blue=data;
  strBlue=String(Blue);

  //Serial.println();
  delay(2000);

  getColor();

  ToDisplay();  
  delay(2000);
}

void displayColor(){
  oled.setTextSize(2);          // text size
  oled.setCursor(0, 0);         // position to display
  oled.print(strColor);   // text to display
  oled.display();               // show on OLED
}

void displayRed(){
  oled.setTextSize(1);          // text size
  oled.setCursor(0, 25);        // position to display
  str1=String("Red: ");       // text to display
  str2=String(str1+strRed);
  oled.print(str2);
  oled.display();               // show on OLED
}

void displayGreen(){
  oled.setTextSize(1);          // text size
  oled.setCursor(0, 40);        // position to display
  str1=String("Green: ");       // text to display
  str2=String(str1+strGreen);
  oled.print(str2);
  oled.display();               // show on OLED
}

void displayBlue(){
  oled.setTextSize(1);          // text size
  oled.setCursor(0, 55);        // position to display
  str1=String("Blue: ");       // text to display
  str2=String(str1+strBlue);
  oled.print(str2);
  oled.display(); 
}

void getColor(){
  if (Red > 85 && Green > 85 && Blue > 85){
    strColor=String("White");
    color1();
  }
  else if (Red < 20 && Green < 20 && Blue <20){
    strColor=String("Black");
    color2();
  }
  else if (Red < 80 && Red>70 && Green < 80 && Green>70 && Blue <80 && Blue >70){
    strColor=String("Silver");
    color2();
  }  
  else if (Red < 55  && Red>45 && Green < 60 && Green> 50 && Blue < 60 && Blue > 50){
    strColor=String("Gray");
    color2();
  } 
  else if (Red > Blue && Red > Green){
    RedSub();
  }
  else if (Green > Blue && Green > Red){
    GreenSub();
  }
  else if (Blue > Red && Blue > Green){
    BlueSub();
  }
  else{
    strColor=String("Unknown");
  }
}

void GetData(){
  data=pulseIn(out,LOW);       //here we wait until "out" go LOW, we start measuring the duration and stops when "out" is HIGH again
  Serial.print(data);          //it's a time duration measured, which is related to frequency as the sensor gives a frequency depending on the color
  Serial.print("\t");          //The higher the frequency the lower the duration
  delay(20);
}

void ToDisplay(){
  displayColor();
  displayRed();
  displayGreen();
  displayBlue();
}

void percentageRGB(){
  if (data<0){
    data=0;
  }
  else{
    data=data*100/255;
  }
}

void RedSub(){
  if (Red>80 && Green>70 && Green<80 && Blue>80){
    strColor=String("Pink");
    //color3();
  }
  else if (Red > 75 && Green > 75){
    strColor=String("Yellow");
    color3();
  }
  else if (Red > 60 && Green < 75 && Green > 50 && Blue < 65){
    strColor=String("Orange");
    color4();
  }
  else if (Red > 70 && Blue > 65){
    strColor=String("Purple");
    color11();
  }
  else if (Red < 55){
    strColor=String("Maroon");
    color5();
  }
  else{
    strColor=String("Red");
    color6();
  }  
}

void GreenSub(){
  if (Green > 80 && Blue > 80){
    strColor=String("Aqua");
    color7();
  }
  if (Red > 60 && Green > 60 && Green < 80){
    strColor=String("Olive");
    color8();
  }
  else if (Green > 80){
    strColor=String("Lime");
    color9();
  }
  else if (Green < 40){
    strColor=String("Dark Green");
    color9();
  }
  else{
    strColor=String("Green");
    color10();
  }  
}

void BlueSub(){
 if (Green > 80 && Blue > 80){
    strColor=String("Aqua");
    color7();
  }  
  else if (Red > 75 && Blue > 75){
    strColor=String("Purple");
    color11();
  }
  else if (Blue < 40){
    strColor=String("Navy");
    color12();
  }
  else{
    strColor=String("Blue");
    color13();
  }  
}

void color1()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(2000);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(1000); //wait 50 seconds.
}


void color2()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 50 seconds.  
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second  
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(1000); //wait 50 seconds.
}

void color3()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(250);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(250); //wait 50 seconds.
}

void color4()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(600);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(200); //wait 50 seconds.
  digitalWrite(motorPin, HIGH);  //stop vibrating
  delay(200); //wait 50 seconds.
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(200); //wait 50 seconds.
  digitalWrite(motorPin, HIGH);  //stop vibrating
  delay(200); //wait 50 seconds.
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(200); //wait 50 seconds.
}

void color5()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(250);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(250); //wait 50 seconds.
  digitalWrite(motorPin, HIGH);  //stop vibrating
  delay(250); //wait 50 seconds.
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(250); //wait 50 seconds.
  digitalWrite(motorPin, HIGH);  //stop vibrating
  delay(250); //wait 50 seconds.
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(500); //wait 50 seconds  
}

void color6()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, HIGH);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(500); //wait 1 seconds.
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(2000); //wait 50 seconds.
}

void color7()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, HIGH);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 1 seconds.
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, HIGH);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 1 seconds.
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //vibratE
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, HIGH);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, LOW);  //vibratE
  delay(500); //wait 50 seconds.
}

void color8()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, HIGH);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 1 seconds.
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, HIGH); //vibrate
  delay(300);  // delay one second
  digitalWrite(motorPin, LOW); //vibrate
  delay(300);  // delay one second
}

void color9()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(150);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, HIGH);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 1 seconds.
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, HIGH); //vibrate
  delay(175);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100); //wait 50 seconds.
  digitalWrite(motorPin, HIGH); //vibrate
  delay(175);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(1000); //wait 50 seconds. 
}

void color10()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(700);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(10000);
}

void color11()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(150);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(150);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(150);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(150);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(300);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(150);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(150);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(150);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(150);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(2000);
}

void color12()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(250);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(50);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(250);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(50);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(2000);
}

void color13()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(250);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(50);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(250);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(50);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(50);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(50);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(50);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(50);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(50);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(50);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(250);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(2000);
}

void color14()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(75);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(75);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(200);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(2000);
}

void color15()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(200);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(200);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(2000);
}

void color16()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(1000);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(500);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(1000);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(500);
}

void color17()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(75);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(75);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(2000);
}

void color18()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(500);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(1000);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(500);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(1000);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(2000);
}

void color19()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(100);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(2000);
}

void color20()
{
  digitalWrite(motorPin, HIGH); //vibrate
  delay(150);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(150);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(100);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(75);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(75);
  digitalWrite(motorPin, HIGH); //vibrate
  delay(75);  // delay one second
  digitalWrite(motorPin, LOW);  //stop vibrating
  delay(500);
}