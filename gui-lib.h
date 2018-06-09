


class RotaryGauge {
//  tft.drawRect(FRAME_X, FRAME_Y, FRAME_W, FRAME_H, ILI9341_BLACK);
  ILI9341_t3 *tft;
  //String label = "Freq";
  int centerX = 60;
  int centerY = 60;
  int angleOffset=90;
  int prevAngle = 0;
  int angle = 90;
  int minAngle = 92;
  int maxAngle = 268;
  int outerRadius = 38;
  int innerRadius = 25;
  int color = ILI9341_WHITE;
  int borderColor = ILI9341_WHITE;
  int bkgColor = ILI9341_BLACK;
  int levelColor = ILI9341_WHITE;


  public:
    RotaryGauge(ILI9341_t3 *ptft): tft(ptft){}

    void drawFrame(int cX, int cY){
        centerX=cX;
        centerY=cY;
    for (int currRadius = innerRadius-1; currRadius <= outerRadius; currRadius++) {
      for (int currAngle = minAngle; currAngle <= maxAngle; currAngle++) {
        
  //      int x2 = centerX + round(cos(angle*DEG_TO_RAD)*40);
  //      int y2 = centerY + round(sin(angle*DEG_TO_RAD)*40);
    
    //    tft.drawLine(x1, y1, x2, y2, ILI9341_GREEN);
        int tempColor = bkgColor;
        int tempAngle = currAngle + angleOffset;
        
        if(/*currRadius == innerRadius or*/ currRadius == outerRadius or
           currAngle == minAngle or currAngle == maxAngle ){
          int x = centerX + round(cos(tempAngle*DEG_TO_RAD)*currRadius);
          int y = centerY + round(sin(tempAngle*DEG_TO_RAD)*currRadius);
          tempColor = borderColor;
          tft->drawPixel( x,  y, tempColor);
        }
        if(currRadius == innerRadius-1){
          int x = centerX + round(cos(tempAngle*DEG_TO_RAD)*currRadius);
          int y = centerY + round(sin(tempAngle*DEG_TO_RAD)*currRadius);
          tempColor = borderColor;
          tft->drawLine(centerX, centerY, x, y, tempColor);
        }
      }
    }
    tft->drawRect(centerX-outerRadius, centerY, outerRadius*2+1, 23,  borderColor);
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
      int tempColor = levelColor;
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
    String valString = (String) val;
    tft->fillRect( centerX-18,  centerY-17,  36, 17,  borderColor);
    tft->setCursor(centerX-(12*valString.length()/2), centerY-15);
    tft->setTextColor(bkgColor);
    tft->setTextSize(2); //10px wide
    tft->println(val);
//    }
  }
    
  void drawLabel(String label){
    tft->setTextSize(2); //10px wide
      int labelWidth = 12*label.length();
    tft->fillRect( centerX-labelWidth/2,  centerY+2, labelWidth, 20,  bkgColor);
    tft->setTextColor(color);
    tft->setCursor(centerX-labelWidth/2,centerY+4);
    tft->println(label);
  }
};


class uiSwitch{
    ILI9341_t3 *tft;
    int centerX = 60;
    int centerY = 60;
    int width = 76;
    int color = ILI9341_WHITE;
    int invertColor = ILI9341_BLACK;
    int borderColor = ILI9341_WHITE;
    int bkgColor = ILI9341_WHITE;
    
    public:
    uiSwitch(ILI9341_t3 *ptft): tft(ptft){}
    
    void drawFrame(int cX, int cY){
        centerX=cX;
        centerY=cY;
        tft->fillRect(centerX-width/2, centerY-22, width, 44,  invertColor);
        tft->drawRect(centerX-width/2, centerY-22, width, 22,  borderColor);
        tft->fillRect(centerX-width/2, centerY, width, 22,  borderColor);
    }
    
    void draw(String label, String value,bool underline, bool showBorder, bool showBkg){
        drawFrame(centerX, centerY);
        tft->setTextColor(color);
        tft->setCursor(centerX-(12*(label.length())/2),centerY-19);
        tft->println(label);
        tft->setTextColor(invertColor);
        tft->setCursor(centerX-(12*(value.length())/2),centerY+3);
        tft->println(value);
    }
};




//class uiSwitch{
//    ILI9341_t3 *tft;
//    int centerX = 60;
//    int centerY = 60;
//    int width = 76;
//    int color = ILI9341_WHITE;
//    int invertColor = ILI9341_BLACK;
//    int borderColor = ILI9341_WHITE;
//    int bkgColor = ILI9341_WHITE;
//    
//public:
//    uiSwitch(ILI9341_t3 *ptft): tft(ptft){}
//    
//    void setPosition(int cX, int cY){
//        centerX=cX;
//        centerY=cY;
//    }
//    
//    void draw(String label, String value,bool underline, bool showBorder, bool showBkg){
//        tft->fillRect(centerX-(12*5), centerY-4, centerX+(12*7), 22, invertColor);
//        tft->setTextSize(2);
//        if(label != ""){
//            tft->setTextColor(color);
//            tft->setCursor(centerX-(12*(label.length()+value.length())/2),centerY);
//            tft->println(label);
//            if(underline){
//                tft->drawLine(centerX-(12*label.length()), centerY+17, centerX, centerY+17,  borderColor);
//            }
//            centerX = centerX -(12*(label.length()+value.length())/2)  ;
//            tft->setCursor(centerX,centerY);
//        }else{
//            centerX = centerX -12 -(12*(value.length()/2));
//        }
//        
//        if(showBorder){
//            tft->drawRect(centerX+8, centerY-4, 12*(value.length()+1)-6, 22,  borderColor);
//        }
//        if(showBkg){
//            tft->fillRect(centerX+8, centerY-4, 12*(value.length()+1)-6, 22,  bkgColor);
//            tft->setTextColor(invertColor);
//        }
//        tft->setCursor(centerX+12,centerY);
//        tft->println(value);
//    }
//};
