p = SLIPSerial("COM5", 115200);

p.writeSLIP([1]);
p.writeSLIP([0]);


(
Event.addEventType(\solenoid, {
    p.writeSLIP([1]);
    0.03.wait;
    p.writeSLIP([0]);
});
)

(
x = Pbind(
    \type, \solenoid,
    \dur, Pseg([2.0, 0.05,0.05, 2.0], [30,5,30], \lin, inf),
).play;
)

x.stop;



p.close;
