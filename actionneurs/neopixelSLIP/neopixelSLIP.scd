p = SLIPSerial("COM5", 115200);

p.writeSLIP(255!40);
p.writeSLIP(0!40);
p.writeSLIP(([0,0,0,255]!10).flat);
p.writeSLIP(([0,0,0,255]!10).flat);

p.writeSLIP([0,0,0,255]);


10.do({
    arg i;
    p.writeSLIP([i, 255,255,255,255]);
});

10.do({
    arg i;
    p.writeSLIP([i, 0,0,0,0]);
});


(
Event.addEventType(\led, {
    p.writeSLIP(([~red, ~green, ~blue, ~white]!10).flat);
});
)

(
x = Pbind(
    \type, \led,
    \dur, 0.03,
    \red, Pseq([0,255],inf),
    \green, Pseq([255,0], inf),
    \blue, 100,
    \white, 0
).play;
)

x.stop;



p.close;