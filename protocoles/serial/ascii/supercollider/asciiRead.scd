(
p = SerialPort("COM7", 115200);

r = Routine({
    var string, val;
    loop {
        var char;
        char = p.read;
        (char == 13).if({
            val = string.split($ ).asInteger;
            val.postln;
            // faire quelque chose avec val!
            // x.set(\freq, val[0].linexp(0, 1023, 110, 880));
            // y.set(\freq, val[1].linexp(0, 1023, 110, 880));
            string = "";
        }, {
            string = string ++ char.asAscii;
        });
    }
});

r.play;
)


r.stop;p.close;


