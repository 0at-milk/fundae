/***
  code adapted from:
  http://processing.flosscience.com/processing-for-android/macul-2012/simple-interaction
  
  edited to function with esp32 and joystick
***/
int xPos=50;
int yPos=50;
int xDir=1;
int yDir=1;

int speed=10;
int lives=5;
boolean lost=false;
int score; 

int duration = 100;
int pressTime;

import processing.serial.*;
import java.util.Arrays;
Serial myPort;  // Create object from Serial class
String val;      // Data received from the serial port
int xyzPins[] = {13, 12, 14};   //x,y,z pins

void setup()
{
  size(500, 500);
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[4];
  System.out.println(portName);
  myPort = new Serial(this, portName, 9600);

  
  size (400,400);
  smooth();
  background(0);
  noStroke();
  fill(0,255,0);

}

void draw()
{
  
  if ( myPort.available() > 0) {  // If data is available,
    val = myPort.readStringUntil('\n');         // read it and store it in val
  }
  val = trim(val);
  System.out.println("VALUES ARE " + val);
  if (val != null && val.equals("0")) {              // If the button is clicked
    speed=speed+1;    // increase the speed
    
  }
  background(0);
  ellipse(xPos, 200, 40, 40);
  textSize(15);
  if( duration <1000 ) {
    fill(3, 252, 240);
    text("\nMove the ball as soon as ", 75,100);
    text("it touches the edges!", 150,150);
    duration+=5;
  }
  
  text("score = "+score,10,10);
  
  
  
  
  //this checks if the user has moved the joystick
  //if (val.length()>1) {              // If the serial value is 0,    
  //  System.out.println("uncleaned..." + val );
  //  String xycoo = val.substring(0, val.length());
  //  String[] words = xycoo.split(",");
  //  //System.out.println("words[0] and words[1]" + words[0] + words[1]);
  //  //System.out.println("The INT VALUE  array size is: " + Integer.parseInt(words[0].strip()));
  //  xPos = Integer.parseInt(words[0].strip());
  //  }
    
  
  xPos=xPos+xDir*speed;
  if (xPos>width-20 || xPos<20)
  {
    xDir=-xDir;
  }
  if (yPos>width-20 || yPos<20)
  {
    xDir=-xDir;
  }
  
  if (lives<=0)
  {
    noLoop();
  }
  
  if (lives<=0)
  {
    lost=true;
    noLoop();
    textSize(20);
    text("Click to Restart", 125,100);
    textSize(13);
  }
}
//this code doesn't adds extra functionallity
void mousePressed(){
  if (dist(mouseX, mouseY, xPos, 200)<=20)
  {
    speed=speed+1;
    score=score+speed;
  }
  else
  {
    lives=lives-1;
  }
  if (lost==true)
  {
    speed=1;
    lives=5;
    score=0;
    xPos=width/2;
    yPos=width/2;
    xDir=1;
    yDir=1;
    lost=false;
    loop();
  }
}

void keyReleased() {
  pressTime = millis();
}
