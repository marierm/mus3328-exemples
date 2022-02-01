p = SLIPSerial("COM8", 115200);

p.writeSLIP([90]);
p.writeSLIP([0]);



(
b = Bus.control;
a = {Out.kr(b, SinOsc.kr(1).range(0,180))}.play;
)

(
fork{
    loop {
        b.get({
            arg value;
            p.writeSLIP([ value.asInteger ]);
        });
        0.1.wait;
    }
}
)

p.close;