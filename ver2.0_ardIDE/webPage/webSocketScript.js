
var ipValue;
var connection;

var generalVal = "1";
var octaveVal = "A";
var color1 = {red:"000", green:"255", blue:"000"};
var color2 = {red:"255", green:"255", blue:"000"};
var color3 = {red:"255", green:"000", blue:"000"};
var var1 = "050";
var var2 = "050";

function WSConnect(){
var text = document.getElementById('ip').value;
ipValue = text;	
connection = new WebSocket(ipValue, ['arduino']); 	
connection.onopen = function() {
	document.getElementById("webSocketCell").style.backgroundColor = "lightgreen"; 
};
connection.onclose = function() {
	document.getElementById("webSocketCell").style.backgroundColor = "red"; 
};
connection.onmessage = function(evt) {
		console.log((evt.data));
		bigString2Vars(evt.data);
		updateHTML();
};

console.log(text)
console.log("IP value changed to:"+ipValue);
updateHTML();

}	


function updateHTML() {
document.getElementById("Action").value = parseInt(generalVal);
if (octaveVal=="A") document.getElementById("Octave-range").value = 0;
else document.getElementById("Octave-range").value = parseInt(octaveVal);
document.getElementById("color1").value = rgbToHex(parseInt(color1.red), parseInt(color1.green), parseInt(color1.blue));
document.getElementById("color2").value = rgbToHex(parseInt(color2.red), parseInt(color2.green), parseInt(color2.blue));
document.getElementById("color3").value = rgbToHex(parseInt(color3.red), parseInt(color3.green), parseInt(color3.blue));
document.getElementById("var1-range").value = parseInt(var1);
document.getElementById("var2-range").value = parseInt(var2);

$(document.getElementById('Action')).change();
$(document.getElementById('Octave-range')).slider( "refresh" );
$(document.getElementById('var1-range')).slider( "refresh" );
$(document.getElementById('var2-range')).slider( "refresh" );
}

function componentToHex(c) {
    var hex = c.toString(16);
    return hex.length == 1 ? "0" + hex : hex;
}

function rgbToHex(r, g, b) {
    return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}

function hexToRgb(hex) {
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
        red: ZeroPadding(parseInt(result[1], 16)),
        green: ZeroPadding(parseInt(result[2], 16)),
        blue: ZeroPadding(parseInt(result[3], 16))
    } : null;
}			

function bigString2Vars(bigString) {
      generalVal = bigString.substring(0,1);
      octaveVal =  bigString.substring(1,2);
      color1.red = bigString.substring(2,5);
	  color1.green = bigString.substring(5,8);
	  color1.blue = bigString.substring(8,11);
      color2.red = bigString.substring(11,14);
	  color2.green = bigString.substring(14,17);
	  color2.blue = bigString.substring(17,20);
      color3.red = bigString.substring(20,23);
	  color3.green = bigString.substring(23,26);
	  color3.blue = bigString.substring(26,29);	  
      var1 = bigString.substring(29,32);	
      var2 = bigString.substring(32,35);	
}		

function presets(value) {
	switch (value) {
		case 1:
			bigString2Vars("3A128249255255255000255000000050050");
			sendManual();
			bigString2Vars("4A000255000255255000255000000050050");
			break;
		case 2:
			bigString2Vars("3A000000000058001058250187255028068");
			sendManual();
			bigString2Vars("2A004000004058001058250187255028068");
			break;
		case 3:
			bigString2Vars("3A255000000255172000255000000033050");
			sendManual();
			bigString2Vars("1A000255000255172000255000000033050");
			break;
		case 4:
			bigString2Vars("3A111244255000000000000240000004050");
			sendManual();
			bigString2Vars("1A141255117000000000000240000004050");
			break;
		case 5:
			bigString2Vars("3A000000000249000000240000000072050");
			sendManual();
			bigString2Vars("1A000000230249000000240000000072050");
			break;			
		
	}
	sendManual();
	updateHTML();
}
		
function sendPreSet(preSetVal)
			{
				bigString2Send="";
				bigString2Send.concat(generalVal,octaveVal,color1.red,color1.green,color1.blue,color2.red,color2.green,color2.blue,color3.red,color3.green,color3.blue,var1,var2);
				console.log(bigString2Send);
					//connection.send("t"+newValue);
			}	

function sendManual()
			{
			var bigString2Send="";
			var octaveValTmp = octaveVal;
			if (octaveValTmp!='A') {
				octaveValTmp = 7 - octaveValTmp;
			}
			bigString2Send = bigString2Send.concat(generalVal,octaveVal,color1.red,color1.green,color1.blue,color2.red,color2.green,color2.blue,color3.red,color3.green,color3.blue,var1,var2);
			console.log(bigString2Send);
			connection.send(bigString2Send);
			}			

function ZeroPadding(num) {
	var n = num.toString();
	if (num < 10) {
		return ("00" + n);
	}
	else if (num < 100) {
		return ("0" + n);
	}
	else {
	return n;
	}
}	

function ChangeGeneralVal(value) {
	generalVal = value.toString();Octave
	document.getElementById("Octave").style.display  = "block";		
	document.getElementById("color1All").style.display  = "block";	
	document.getElementById("color2All").style.display  = "block";
	document.getElementById("color3All").style.display  = "block";		
	document.getElementById("var1").style.display  = "block";		
	document.getElementById("var2").style.display  = "block";	
	document.getElementById("rainbowSpeed").style.display  = "block";	
	document.getElementById("avgHistory").style.display  = "block";	
	document.getElementById("delay").style.display  = "block";	
	document.getElementById("noiseLevel").style.display  = "block";		
	
	switch(value) {
			case "1":
				document.getElementById("var2").style.display  = "none";		
				document.getElementById("rainbowSpeed").style.display  = "none";	
				document.getElementById("avgHistory").style.display  = "none";	
				document.getElementById("delay").style.display  = "none";	
				document.getElementById("noiseLevel").style.display  = "none";						
				break;
			case "2":
				document.getElementById("rainbowSpeed").style.display  = "none";	
				document.getElementById("avgHistory").style.display  = "none";	
				document.getElementById("delay").style.display  = "none";	
				document.getElementById("noiseLevel").style.display  = "none";				
				break;
			case "3":
				document.getElementById("color2All").style.display  = "none";
				document.getElementById("color3All").style.display  = "none";
				document.getElementById("var1").style.display  = "none";					
				document.getElementById("var2").style.display  = "none";	
				document.getElementById("rainbowSpeed").style.display  = "none";	
				document.getElementById("avgHistory").style.display  = "none";	
				document.getElementById("delay").style.display  = "none";	
				document.getElementById("noiseLevel").style.display  = "none";					
				break;
			case "4":
				document.getElementById("Octave").style.display  = "none";		
				document.getElementById("color1All").style.display  = "none";	
				document.getElementById("color2All").style.display  = "none";
				document.getElementById("color3All").style.display  = "none";		
				document.getElementById("var1").style.display  = "none";		
				document.getElementById("var2").style.display  = "none";	
				document.getElementById("avgHistory").style.display  = "none";	
				document.getElementById("delay").style.display  = "none";	
				document.getElementById("noiseLevel").style.display  = "none";				
				break;
			case "5":
				document.getElementById("Octave").style.display  = "none";		
				document.getElementById("color1All").style.display  = "none";	
				document.getElementById("color2All").style.display  = "none";
				document.getElementById("color3All").style.display  = "none";		
				document.getElementById("var1").style.display  = "none";		
				document.getElementById("var2").style.display  = "none";	
				document.getElementById("rainbowSpeed").style.display  = "none";	
				document.getElementById("noiseLevel").style.display  = "none";				
				break;
			case "6":
				document.getElementById("color1All").style.display  = "none";	
				document.getElementById("color2All").style.display  = "none";
				document.getElementById("color3All").style.display  = "none";		
				document.getElementById("var1").style.display  = "none";		
				document.getElementById("var2").style.display  = "none";	
				document.getElementById("rainbowSpeed").style.display  = "none";	
				document.getElementById("avgHistory").style.display  = "none";	
				document.getElementById("delay").style.display  = "none";			
				break;

		}
	}
		
function ChangeOctave(value) {
	if (value==0) octaveVal="A";
	else octaveVal = value.toString();
	}

			
function ChangeColor(colorNum,hexValue) {

	switch(colorNum) {
	case 1:
		color1 = hexToRgb(hexValue);
		break;
	case 2:
		color2 = hexToRgb(hexValue);
		break;
	case 3:
		color3 = hexToRgb(hexValue);
		break;		
	}
}	

function ChangeVars(varNum,value) {
	switch(varNum) {
	case 1:
		var1 = ZeroPadding(value);
		break;
	case 2:
		var2 = ZeroPadding(value);
		break;

	}
}			

function ChangeRainbowSpeed(value) {
		var1 = ZeroPadding(value);
}

function ChangeAvgHistory(value) {
		var1 = ZeroPadding(value);
}

function ChangeDelay(value) {
		var2 = ZeroPadding(value);
}

function ChangeNoiseLevel(value) {
		var1 = ZeroPadding(value);
}

