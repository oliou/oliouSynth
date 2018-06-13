//This example implements a simple sliding On/Off button. The example
// demonstrates drawing and touch operations.
//
//Thanks to Adafruit forums member Asteroid for the original sketch!
//
#include <SPI.h>
#include <Wire.h>
#include <ILI9341_t3.h>
#include <Adafruit_STMPE610.h>
#include <gui-lib.h>


// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
#define TFT_DC      20
#define TFT_CS      21
#define TFT_RST    255  // 255 = unused, connect to 3.3V
#define TFT_MOSI     7
#define TFT_SCLK    14
#define TFT_MISO    12
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

boolean RecordOn = false;

#define FRAME_X 210
#define FRAME_Y 180
#define FRAME_W 100
#define FRAME_H 50

#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H

#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H

#define DEG_TO_RAD 0.017453292519943295769236907684886



 

void redBtn()
{ 
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ILI9341_RED);
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ILI9341_BLUE);
//  drawFrame();
  tft.setCursor(GREENBUTTON_X + 6 , GREENBUTTON_Y + (GREENBUTTON_H/2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("ON");
  RecordOn = false;
}

void greenBtn()
{
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, ILI9341_GREEN);
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, ILI9341_BLUE);
//  drawFrame();
  tft.setCursor(REDBUTTON_X + 6 , REDBUTTON_Y + (REDBUTTON_H/2));
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("OFF");
  RecordOn = true;
}

unsigned long last_time = millis();

RotaryGauge gauge1 = RotaryGauge(&tft); 
//uiSwitch switch1 = uiSwitch(&tft); 
horizontalGauge gauge =   horizontalGauge(&tft); 

void setup(void)
{
  Serial.begin(9600);
  tft.begin();
  if (!ts.begin()) { 
    Serial.println("Unable to start touchscreen.");
  } 
  else { 
    Serial.println("Touchscreen started."); 
  }

  tft.fillScreen(ILI9341_BLACK);
  // origin = left,top landscape (USB left upper)
  tft.setRotation(3); 
  redBtn();

//  switch1.draw(""," LP ",false,true,false);
   gauge.draw("Filter",64,0,127);
}

void loop()
{

//    for (int idx = 0; idx <= 230; idx++) {
//      Serial.println("--------");
//      int time = millis();
//      Serial.println();
//      drawFrame(idx);
//      time = millis()-time; 
//      Serial.println((String) time);
//      //Serial.println(idx);
//      delay(50);
//    }
//      tft.fillScreen(ILI9341_BLACK);

    
//    gauge1.drawFrame(80,80);

//    int time = millis();
//    Serial.println("--------");
//    gauge1.drawValue(50);
//    time = millis()-time; 
//    Serial.println((String) time);
//        delay(3000);
//
//    Serial.println("--------");
//    time = millis();
//    gauge1.drawValue(90);
//    time = millis()-time; 
//    Serial.println((String) time);
//        delay(3000);
//        
//    Serial.println("--------");
//    time = millis();
//    gauge1.drawValue(30);
//    time = millis()-time; 
//    Serial.println((String) time);
//
//    delay(3000);

//    for (int idx = 0; idx <127; idx+=5) {
//      Serial.println("--------");
////             Serial.println(idx);
//
//      int time = millis();
////      Serial.println();
//      gauge1.drawValue(idx,0,127);
//      time = millis()-time; 
////      Serial.println((String) time);
//      //Serial.println(idx);
//      delay(50);
//    }
//
//    for (int idx = 126; idx >= 0; idx-=5) {
//      Serial.println("--------");
////             Serial.println(idx);
//
//      int time = millis();
////      Serial.println();
//      gauge1.drawValue(idx,0,127);
//      time = millis()-time; 
////      Serial.println((String) time);
//      //Serial.println(idx);
//      delay(50);
//    }

//    for (int idx = 230; idx > 0; idx--) {
//      Serial.println("--------");
////             Serial.println(idx);
//
//      int time = millis();
////      Serial.println();
//      gauge1.drawValue(idx);
//      time = millis()-time; 
//      Serial.println((String) time);
//      //Serial.println(idx);
//      delay(50);
//    }
  
  // See if there's any  touch data for us
//  if (!ts.bufferEmpty())
//  {   
//    // Retrieve a point  
//    TS_Point p = ts.getPoint(); 
//    // Scale using the calibration #'s
//    // and rotate coordinate system
//    p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());
//    p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
//    int y = tft.height() - p.x;
//    int x = p.y
//    
//
//    if (RecordOn)
//    {
//      if((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
//        if ((y > REDBUTTON_Y) && (y <= (REDBUTTON_Y + REDBUTTON_H))) {
//          Serial.println("Red btn hit"); 
//          redBtn();
//        }
//      }
//    }
//    else //Record is off (RecordOn == false)
//    {
//      if((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
//        if ((y > GREENBUTTON_Y) && (y <= (GREENBUTTON_Y + GREENBUTTON_H))) {
//          Serial.println("Green btn hit"); 
//          greenBtn();
//        }
//      }
//    }
//
//    Serial.println(RecordOn);
//  }  
}


                                                                                                                                                                                                                                                                                                                                               
