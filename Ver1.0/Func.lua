function max(a,b) 
     if (a>b) then
          return a
     else
          return b
     end
end

function min(a,b) 
     if (a>b) then
          return b
     else
          return a
     end
end

function floor(a) 
     return (a - a%1)
end

function Wheel (WheelPos) 
     if (WheelPos < 85) then
        return {WheelPos * 3, 255 - WheelPos * 3, 0}
     elseif (WheelPos < 170) then
        WheelPos = WheelPos - 85;
        return {255 - WheelPos * 3, 0, WheelPos * 3};
     else 
        WheelPos = WheelPos - 170;
        return {0, WheelPos * 3, 255 - WheelPos * 3};
     end
end

function TwoColorWheel(rgb1,rgb2,octave)
     local rgbOut={}
     for i = 1,3 do
          rgbOut[i] = rgb1[i] + ( (octave-1) * (rgb2[i] - rgb1[i]))/6
     end
     return rgbOut
end

function LedsReadWrite ()
      local minVal = {150,170,200,200,200,200,200}
      local maxLeds = 36

     if typeMenu==5 then
          for i = 1,7 do
              local rgb = Wheel( (movingIdx + (255*i) / 7)%255 + 1 ) 
              RGB.r[i] = rgb[1]
              RGB.g[i] = rgb[2]
              RGB.b[i] = rgb[3]
          end
          movingIdx = movingIdx%255 + 4  
     end 
     gpio.write(resetPin, gpio.HIGH)
     gpio.write(resetPin, gpio.LOW)
     local lastString=nil
     
     for i = 1,7 do 
          gpio.write(strobePin, gpio.LOW)
          local spectrumValue = max(minVal[i],min(1023,tonumber(adc.read(0))))    
          gpio.write(strobePin, gpio.HIGH)
          local numLeds = (spectrumValue - minVal[i]) / ( (1023 - minVal[i] ) / maxLeds )
          numLeds = max(1,min(maxLeds-1,numLeds))
          lastVal[i] = max(numLeds,lastVal[i] - 1)
          if i%2==0 then
              triString=string.char(0,0,0):rep(maxLeds -  lastVal[i] )..string.char(255,0,0)..string.char(0,0,0):rep( lastVal[i] - numLeds )..string.char(RGB.r[i],RGB.g[i],RGB.b[i]):rep(numLeds-1)      
          else
              triString=string.char(RGB.r[i],RGB.g[i],RGB.b[i]):rep(numLeds-1)..string.char(0,0,0):rep( lastVal[i] - numLeds )..string.char(255,0,0)..string.char(0,0,0):rep(maxLeds - lastVal[i])
          end
        if i==1 then
               lastString = triString
         else
               lastString = lastString..triString
         end
     end
     ws2812.writergb(stripPin, lastString)
end 


stripPin = 1
resetPin = 4
strobePin = 6
RGB={r={},g={},b={}}
for i = 1,7 do
     RGB.r[i] = 0
     RGB.g[i] = 255
     RGB.b[i] = 0
end
lastVal = {0,0,0,0,0,0,0}
gpio.mode(resetPin, gpio.OUTPUT)  
gpio.mode(strobePin, gpio.OUTPUT) 
gpio.mode(stripPin,gpio.OUTPUT)
gpio.write(resetPin, gpio.LOW)
gpio.write(strobePin, gpio.HIGH)
movingIdx=1
tmr.alarm(0, 150, 1, LedsReadWrite)
