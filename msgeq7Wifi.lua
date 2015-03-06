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

stripPin = 3
resetPin = 4
strobePin = 6
adPin = 0
spectrumValue={}
RGB={r=255,g=0,b=0}
minVal = 350
maxVal = 1023
dly = 1000

gpio.mode(resetPin, gpio.OUTPUT)  
gpio.mode(strobePin, gpio.OUTPUT) 
gpio.write(resetPin, gpio.LOW)
gpio.write(strobePin, gpio.HIGH)

wifi.setmode(wifi.STATIONAP)
wifi.ap.config({ssid='WiFi Light 192.168.4.1'})
print(wifi.ap.getip())
if srv then
  srv:close()
end
srv = net.createServer(net.TCP)
srv:listen(80, function(conn)

conn:on("receive", function(conn, payload)
   -- print(payload)
    local isOpen = false

    conn:on("sent", function(conn)
      if not isOpen then
       -- print('open')
        isOpen = true
        file.open(fileName, 'r')
      end
      local data = file.read(1024) -- 1024 max
      if data then
     --   print('send ' .. #data)
        conn:send(data)
      else
      --  print('close')
        file.close()
        conn:close()
        conn = nil
      end
    end)

    if string.sub(payload, 1, 6) == 'GET / ' then
      fileName = 'index.html'
      conn:send("HTTP/1.1 200 OK\r\n")
      conn:send("Content-type: text/html\r\n")
      conn:send("Connection: close\r\n\r\n")
    elseif string.sub(payload, 1, 9) == 'GET /off ' then
      conn:close()
      conn = nil
      RGB.r = 0
      RGB.g = 0
      RGB.b = 0 
    elseif string.sub(payload, 1, 8) == 'GET /on ' then
      conn:close()
      conn = nil
      RGB.r = 255
      RGB.g = 255
      RGB.b = 255    
    elseif string.sub(payload, 1, 6) == 'GET /~' then
      conn:close()
      conn = nil
      RGB.r = tonumber(string.sub(payload, 7, 8), 16)
      RGB.g = tonumber(string.sub(payload, 9, 10), 16)
      RGB.b = tonumber(string.sub(payload, 11, 12), 16)
    else
      conn:close()
    end

  end)

end)

tmr.alarm(0, 75, 1, function() 

     gpio.write(resetPin, gpio.HIGH)
     gpio.write(resetPin, gpio.LOW)
     local lastString=nil
     
     for i = 1,8 do 
          gpio.write(strobePin, gpio.LOW)
          tmr.delay(dly)
          local spectrumValue = adc.read(adPin)
          gpio.write(strobePin, gpio.HIGH)
          spectrumValue = max(minVal,min(maxVal,spectrumValue))
          spectrumValue = (1 * (spectrumValue - minVal) / (maxVal - minVal ))
        -- print("Oct["..i.."]: Val = ",tostring(spectrumValue[i]))
    --     local red = 1*min(85,max((spectrumValue[i]-85),0));
    --     local green =1*min(85,max((spectrumValue[i]-170),0));
    --     local blue = 1*min(85,max((spectrumValue[i]-0),0));
          local r = floor(min(255,max((spectrumValue*RGB.r),0)))
          local g = floor(min(255,max((spectrumValue*RGB.g),0)))
          local b = floor(min(255,max((spectrumValue*RGB.b),0)))
         local triString = string.char(0, 0, 0)..string.char(g, r, b):rep(2)
         if i==1 then
               lastString = triString
         else
               lastString = lastString..triString
         end
     end
   -- print("                      ")
     ws2812.write(stripPin, lastString)

end )