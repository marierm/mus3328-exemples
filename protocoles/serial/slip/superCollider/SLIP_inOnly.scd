
~buses = 2.collect({
    Bus.control;
});

(
r.stop;p.close;
p = SerialPort("COM7", 115200);

r = Routine({
    var byte, packet=[];
    loop {
        byte = p.read;
        byte.switch(
            192, {
                packet.pairsDo({
                    arg msb, lsb, i;
                    ~buses[ i.div(2) ].set(
                        ((msb << 8)  |  lsb).linlin(0,1023, 0.0, 1.0);
                    );
                });
                packet=[];
            },
            219, {
                byte = p.read;
                byte.switch(
                    220, { packet = packet ++ 192; },
                    221, { packet = packet ++ 219; },
                    { "SLIP error".postln; }
                );
            },
            { packet = packet ++ byte }
        );
    }
});

r.play;
)




