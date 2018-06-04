


class RotaryGauge {
//  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
  ILI9341_t3 *tft;
  String label = "Freq";
  int centerX = 60;
  int centerY = 60;
  int angleOffset=90;
  int prevAngle = 0;
  int angle = 90;
  int minAngle = 70;
  int maxAngle = 290;
  int outerRadius = 39;
  int innerRadius = 25;
  int color = ILI9341_WHITE;
  int borderColor = ILI9341_WHITE;
  int bkgColor = ILI9341_BLACK;

  public:
    RotaryGauge(ILI9341_t3 *ptft): tft(ptft){}

    void drawFrame(int cX, int cY){
        centerX=cX;
        centerY=cY;
    for (int currRadius = innerRadius; currRadius <= outerRadius; currRadius++) {
      for (int currAngle = minAngle; currAngle <= maxAngle; currAngle++) {
        
  //      int x2 = centerX + round(cos(angle*DEG_TO_RAD)*40);
  //      int y2 = centerY + round(sin(angle*DEG_TO_RAD)*40);
    
    //    tft.drawLine(x1, y1, x2, y2, ILI9341_GREEN);
        int tempAngle = currAngle + angleOffset;
        int tempColor = bkgColor;
        if(currRadius == innerRadius or currRadius == outerRadius or
           currAngle == minAngle or currAngle == maxAngle ){
          int tempAngle = currAngle + angleOffset;
          int x = centerX + round(cos(tempAngle*DEG_TO_RAD)*currRadius);
          int y = centerY + round(sin(tempAngle*DEG_TO_RAD)*currRadius); 
          tempColor = borderColor;
          tft->drawPixel( x,  y, tempColor);
        }
        
      }
    }
  }

  void setPosition(int cX, int cY){ 
      centerX=cX;
      centerY=cY;
  }

  void drawValue(int val, int minVal, int maxVal){
      if(val < minVal) val = minVal;
      if(val > maxVal) val = maxVal;
      angle = 1+(maxAngle-minAngle)*val/maxVal;
      int angleStart=minAngle+prevAngle;
      int angleStop=minAngle+angle;
      int tempColor = color;
      int modulo = 1;
      if(prevAngle > angle){
          //      Serial.println("rever");
          tempColor =  bkgColor;
          angleStart = minAngle+angle;
          angleStop = minAngle+prevAngle;
          modulo = 1;
      }
      prevAngle=angle;
//      Serial.println(val);
//    Serial.println(angle);
//
//   Serial.println(angleStart); 
//    Serial.println(angleStop);
//    delay(3000);
    
//    for (int currRadius = innerRadius; currRadius <= outerRadius; currRadius++) {
    for (int currAngle = angleStart; currAngle < angleStop; currAngle++) {
      int tempAngle = currAngle + angleOffset;
      int x1 = centerX + round(cos(tempAngle*DEG_TO_RAD)*(innerRadius+2));
      int y1 = centerY + round(sin(tempAngle*DEG_TO_RAD)*(innerRadius+2)); 
      int x2 = centerX + round(cos(tempAngle*DEG_TO_RAD)*(outerRadius-2)); 
      int y2 = centerY + round(sin(tempAngle*DEG_TO_RAD)*(outerRadius-2)); 
      if( (currAngle % modulo == 0)){ //currAngle < (minAngle+angle) &&
        tft->drawLine(x1, y1, x2, y2, tempColor);
      }  
    }
    drawLabel(val);
//    }
  }

  void drawLabel(int val){
    tft->fillRect( centerX-20,  centerY-10,  40, 20,  bkgColor);
    tft->setCursor(centerX-18,centerY-10);
    tft->setTextColor(color);
    tft->setTextSize(2);
    tft->println(val);
  }
  

//  Serial.println((String) round(cos(180*DEG_TO_RAD)*40) ); 
//  Serial.println((String) round(sin(180*DEG_TO_RAD)*40) ); 


//  tft.drawCircle( 60, 60, 41, ILI9341_GREEN); 
//  tft.fillCircle(60, 60, 40, ILI9341_BLACK);

//    tft.setCursor(0,150);
//    tft.setTextColor(ILI9341_WHITE);
//    tft.setTextSize(2);
//     tft.println((String) x);
//     tft.println((String) y);

};

