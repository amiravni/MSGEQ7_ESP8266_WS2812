

// definition of RGB class
class RGB {
  public:
    RGB() {
      red = 0;
      green = 0;
      blue = 0;
    }
    RGB(int r, int g, int b) {
      red = r;
      green = g;
      blue = b;
    }
    RGB operator= (RGB color1) {
      this->red = color1.red;
      this->green = color1.green;
      this->blue = color1.blue;
    }
    void initRGB() {
      red = 0;
      green = 0;
      blue = 0;
    }
    void wheel(int WheelPos) {
  WheelPos = WheelPos % 256;

  if (WheelPos < 85) {
    red = WheelPos * 3;
    green = 255 - WheelPos * 3;
    blue = 0;
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    blue = WheelPos * 3;
    red = 255 - WheelPos * 3;
    green = 0;
  }
  else if (WheelPos < 255) {
    WheelPos -= 170;
    green = WheelPos * 3;
    blue = 255 - WheelPos * 3;
    red = 0;

  }
  else
  {
    WheelPos -= 255;
    red = WheelPos * 3;
    green = 255 - WheelPos * 3;
    blue = 0;
  }
      
    }

    void getColorFromString(String str) {
      String strSub = (str.substring(0, 3));
      red = strSub.toInt();
      strSub = (str.substring(3, 6));
      green = strSub.toInt();
      strSub = (str.substring(6, 9));
      blue = strSub.toInt();
    }


    void getColorWheel(int WheelPos) {
      WheelPos = WheelPos % 256;

      if (WheelPos < 85) {
        red = WheelPos * 3;
        green = 255 - WheelPos * 3;
        blue = 0;
      }
      else if (WheelPos < 170) {
        WheelPos -= 85;
        blue = WheelPos * 3;
        red = 255 - WheelPos * 3;
        green = 0;
      }
      else if (WheelPos < 255) {
        WheelPos -= 170;
        green = WheelPos * 3;
        blue = 255 - WheelPos * 3;
        red = 0;

      }
      else
      {
        WheelPos -= 255;
        red = WheelPos * 3;
        green = 255 - WheelPos * 3;
        blue = 0;
      }
    }
    
    int red;
    int green;
    int blue;
};



// definition of a Pixel
class pixel {
  public:
    void initPixel(int iPosInp) {
      iPos = iPosInp;
      iOctave = iPos / numOfLedsSingle;
      iOctavePos = iPos % numOfLedsSingle;
      clr.initRGB();
      clr.green = 255;
    }

    void changeColor(RGB color) {
      clr = color;
    }

    int iPos;
    int iOctave;
    int iOctavePos;
    RGB clr;
};




// definition of an Octave
class octave {
    RGB topColor;
    int iOctPos;
    int iPosStart;
    int iPosEnd;
    int analogVal[bufferSize];
    int pixelsVal;
    int dirOfFlow;
    int minVal;
    int maxVal;
    int lastTopVal;
    bool lastTopValDelay;
    pixel octPixels[numOfLedsSingle];
    int rainbowIdx;

  public:
    void octaveInit(int iOctInp) {
      iOctPos = iOctInp;
      topColor.red = 255;
      topColor.green = 0;
      topColor.blue = 0;
      iPosStart = iOctInp * numOfLedsSingle;
      iPosEnd = (iOctInp + 1) * numOfLedsSingle - 1;
      dirOfFlow = 1;
      if (iOctInp % 2) {
        int tmp = iPosStart;
        iPosStart = iPosEnd;
        iPosEnd = tmp;
        dirOfFlow = -1;
      }
      for (int ppp = 0; ppp < numOfLedsSingle ; ppp++) {
        octPixels[ppp].initPixel(iPosStart + dirOfFlow * ppp);
      }
      analogVal[bufferSize - 1] = 0;
      pixelsVal = 0;
      minVal = minVals[iOctInp];
      maxVal = 1023;
      lastTopVal = 1;
      lastTopValDelay = 1;
      rainbowIdx = iOctPos*36;
    }

    void refreshBuffer() {
      for (int vvv = 0; vvv < bufferSize - 1 ; vvv++) {
        analogVal[vvv] = analogVal[vvv + 1];
      }
      analogVal[bufferSize - 1] = 0;
    }

    void readVal() {
      analogVal[bufferSize - 1] = constrain(analogRead(A0), minVal, maxVal);
     /*      DEBUGGING_L(iPosStart);
            DEBUGGING_L(":  ");
            DEBUGGING_L(analogVal[bufferSize - 1]);
            DEBUGGING_L("   ");*/
      }

    void CalcLedsVal() {
      pixelsVal = 0;
      double weightSum = 0;
      double pixelSum = 0;
      for (int iii = bufferSize - numOfHistory; iii < bufferSize; iii++) {
        double weight = (double)(1 + iii - (bufferSize - numOfHistory)) / ( (double)(numOfHistory) );
        weight = weight * weight; //*weight;
        pixelSum = pixelSum + weight * (double)analogVal[iii];
        weightSum = weightSum + weight;
        delay(0);
      }
      //  DEBUGGING_L(pixelSum);
      //  DEBUGGING_L("  ");
      //  DEBUGGING_L(weightSum);
      //  DEBUGGING_L("-->");
      //  DEBUGGING(pixelSum/weightSum);

      pixelsVal =  map((int)(pixelSum / weightSum), minVal, maxVal, 0, numOfLedsSingle);
    }

    void drawLeds() {
      if (lastTopVal>0) {
        if (!lastTopValDelay) lastTopVal--;
        lastTopValDelay = !lastTopValDelay;
        }
      if (lastTopVal < pixelsVal) lastTopVal = pixelsVal;
      
      for (int ppp = 0; ppp < numOfLedsSingle ; ppp++) {
        if (ppp == lastTopVal) {
          strip.setPixelColor(iPosStart + dirOfFlow * ppp , strip.Color( topColor.red, topColor.green, topColor.blue) ); 
        //  DEBUGGING(ppp);
        }
        else if (ppp > pixelsVal) {
          strip.setPixelColor(iPosStart + dirOfFlow * ppp , strip.Color(  0,   0, 0) );
        }
        else if (ppp <= pixelsVal) {
          strip.setPixelColor(iPosStart + dirOfFlow * ppp , strip.Color(  octPixels[ppp].clr.red, octPixels[ppp].clr.green, octPixels[ppp].clr.blue) );
        }
      }
    }



    void changeColorScheme(int value, RGB color1, RGB color2, RGB color3, double var1, double var2) {
      if (value == 1) {
        RGB colorTmp(0, 0, 0);
        for (int ppp = 0; ppp < ceil(var1 * numOfLedsSingle) ; ppp++) {
          double weight = (((double)ppp / (double)ceil(var1 * numOfLedsSingle) ));
          colorTmp.red = color1.red + (double)( color2.red - color1.red ) * weight;
          colorTmp.green = color1.green + (double)( color2.green - color1.green ) * weight;
          colorTmp.blue = color1.blue + (double)( color2.blue - color1.blue ) * weight;
          octPixels[ppp].changeColor(colorTmp);
          delay(0);
        }
        for (int ppp = ceil(var1 * numOfLedsSingle) ; ppp < (numOfLedsSingle) ; ppp++) {
          double weight = (   (((double)ppp) - (double)(ceil(var1 * numOfLedsSingle))) / (double)(numOfLedsSingle - ceil(var1 * numOfLedsSingle) ));
          colorTmp.red = color2.red + (double)( color3.red - color2.red ) * weight;
          colorTmp.green = color2.green + (double)( color3.green - color2.green ) * weight;
          colorTmp.blue = color2.blue + (double)( color3.blue - color2.blue ) * weight;
          octPixels[ppp].changeColor(colorTmp);
          delay(0);
        }
      }

      if (value == 2) {
        for (int ppp = 0; ppp < (int)ceil(var1 * numOfLedsSingle) ; ppp++) {
          octPixels[ppp].changeColor(color1);
          delay(0);
        }
        for (int ppp = (int)ceil(var1 * numOfLedsSingle); ppp < (int)ceil(var2 * numOfLedsSingle) ; ppp++) {
          octPixels[ppp].changeColor(color2);
          delay(0);
        }
        for (int ppp = (int)ceil(var2 * numOfLedsSingle); ppp < numOfLedsSingle ; ppp++) {
          octPixels[ppp].changeColor(color3);
          delay(0);
        }
      }

      if (value == 3) {
        topColor = color1;
      }
      if (value == 4) { //rainbow
        RGB colorTmp(0, 0, 0);
        colorTmp.wheel(rainbowIdx);
        for (int ppp = 0; ppp < numOfLedsSingle ; ppp++) {
          octPixels[ppp].changeColor( colorTmp );
          delay(0);
        }   
        rainbowIdx = rainbowIdx + 2;     
      }      
      if (value == 6) {
        minVal = var1;
      }
    }


};
