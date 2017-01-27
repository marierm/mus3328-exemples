
(
p = SerialPort(
	"/dev/ttyUSB0",    //edit to match your port. SerialPort.listDevices
	baudrate: 9600    //check that baudrate is the same as in arduino sketch
);
)


//read
(
r = Routine({
	var byte, str, res;
	inf.do{|i|
		if(byte = p.read; byte != 13 , {
			str = str ++ byte.asAscii;
		},{
			res = str.split($ ).asInt;
			// do something 
			res.postln;
			str = "";
		});
	};
}).play;
)

r.stop;
p.close;

// write
p.putAll("123 456 789" ++ 13.asAscii);
