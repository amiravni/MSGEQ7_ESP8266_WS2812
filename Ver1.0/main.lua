

octave = 1
typeMenu = 1 
twoColor = 1

file.open("info.txt", "r")
str = file.readline()
file.close()
SSID = string.sub(str,1,string.find(str, ":")-1)
PWD = string.sub(str,string.find(str, ":")+1,string.len(str))     
wifi.setmode(wifi.STATION)
wifi.sta.config(SSID,PWD)
SSID = nil
PWD = nil
str = nil
if srv then
  srv:close()
end


srv = net.createServer(net.TCP)
srv:listen(80, function(conn)

conn:on("receive", function(conn, payload)
   -- print(payload)
   tmr.stop(0)
    local isOpen = false
    local fileName = 'index.html'
    
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
      conn:send("HTTP/1.1 200 OK\r\n")
      conn:send("Content-type: text/html\r\n")
      conn:send("Connection: close\r\n\r\n")
    elseif string.sub(payload, 1, 8) == 'GET /oct' then
      conn:close()
      conn = nil
      octave = 8 - tonumber(string.sub(payload, 9, 9))
    elseif string.sub(payload, 1, 8) == 'GET /two' then
      conn:close()
      conn = nil
      twoColor = tonumber(string.sub(payload, 9, 9))      
    elseif string.sub(payload, 1, 8) == 'GET /typ' then
      conn:close()
      conn = nil
      typeMenu = tonumber(string.sub(payload, 9, 9))
    elseif string.sub(payload, 1, 6) == 'GET /~' then
      conn:close()
      conn = nil
      if  typeMenu==1 then
          startidx = octave
          stepidx = octave
          endidx = octave
      elseif (typeMenu==2 or typeMenu==5 or typeMenu==6) then
          startidx = 1
          stepidx = 1
          endidx = 7       
     elseif typeMenu==3 then
          startidx = 1
          stepidx = 2
          endidx = 7    
      elseif typeMenu==4 then
          startidx = 2
          stepidx = 2
          endidx = 6  
      else
          startidx = octave
          stepidx = octave
          endidx = octave
      end
     if typeMenu == 6 then
          RGB.r[twoColor] = tonumber(string.sub(payload, 7, 8), 16)
          RGB.g[twoColor] = tonumber(string.sub(payload, 9, 10), 16)
          RGB.b[twoColor] = tonumber(string.sub(payload, 11, 12), 16)
     end
     for i = startidx,endidx,stepidx do
          if typeMenu==5 then
               rgb = Wheel( (255*i) / 7 ) 
               RGB.r[i] = rgb[1]
               RGB.g[i] = rgb[2]
               RGB.b[i] = rgb[3] 
          elseif typeMenu==6 then 
               rgb = TwoColorWheel({RGB.r[1],RGB.g[1],RGB.b[1]},{RGB.r[7],RGB.g[7],RGB.b[7]},i) 
               RGB.r[i] = rgb[1]
               RGB.g[i] = rgb[2]
               RGB.b[i] = rgb[3]                
          else
               RGB.r[i] = tonumber(string.sub(payload, 7, 8), 16)
               RGB.g[i] = tonumber(string.sub(payload, 9, 10), 16)
               RGB.b[i] = tonumber(string.sub(payload, 11, 12), 16)
          end
     end   
    else
      conn:close()
    end
     if typeMenu == 5 then 
           tmr.alarm(0, 85, 1, LedsReadWrite)
     else
          tmr.alarm(0, 75, 1, LedsReadWrite)
     end
  end)

end)


