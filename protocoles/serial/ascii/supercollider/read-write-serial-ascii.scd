
SerialPort.listDevices;
p = SerialPort("/dev/tty.usbmodem1421", baudrate: 115200);

//read
(
r = Routine({
	var byte, str, res;
	99999.do{|i|
		if(byte = p.read; byte != 13 , {
			str = str ++ byte.asAscii;
		},{
			res = str.split($ ).asInt;
			// do something
			res.postln;
			p.putAll(res[0].asString ++ "  " ++ abs(res[0] - 1023).asString ++ "  " ++  (res[1] / 4).asString ++ 13.asAscii);
			str = "";
		});
	};
}).play;
)

r.stop;
p.close;

// write
p.putAll("1023 1023 255" ++ 13.asAscii);
p.putAll("0 0 25" ++ 13.asAscii);
