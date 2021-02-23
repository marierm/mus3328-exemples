p = SLIPSerial("COM5", 115200);

p.writeSLIP([1]);p.writeSLIP([0]);


(
Event.addEventType(\solenoid, {
    p.writeSLIP([1]);
    p.writeSLIP([0]);
});
)

(
x = Pbind(
    \type, \solenoid,
    \dur, 0.1,
    \red, Pseg([0,255,0], [3,3],\lin, inf),
    \green, 0,
    \blue, 0,
    \white, 0
).play;
)

x.stop;



p.close;
