p = SLIPSerial("COM5", 115200);
~numLED=120;
120*4
p.writeSLIP(255!1);
p.writeSLIP(0!480);
p.writeSLIP(([0,0,255,0]!10).flat);
p.writeSLIP(([0,0,0,255]!10).flat);

p.writeSLIP([0,0,0,255]);

(
Event.addEventType(\led, {
    var array;
    ~id = ~id.asArray;
    ~red = ~red ? 0;
    ~green = ~green ? 0;
    ~blue = ~blue ? 0;
    ~white = ~white ? 0;
    array = ~numLED.collect({
        arg i;
        ( ~id.isEmpty || ~id.includes(i) ).if({
            [~red, ~green, ~blue, ~white].asInteger;
        }, {
            [0,0,0,0];
        });
    }).flat;
    // array.postln;
    p.writeSLIP(array);
});
)

(
x = Pbind(
    \type, \led,
    \numLED, ~numLED,
    \dur, 0.05,
    \red, Pseg([0,255,0],[1,1], \lin, inf),
    \blue, Pseg([0,255,0],[0.5, 0.5], \lin, inf),
    \green, Pseg([0,255,0],[0.3, 0.3], \lin, inf),
).play;
)
x.stop;

(
y = Pbind(
    \numLED, ~numLED,
    \type, \led,
    \id,Pseq((0..9).mirror, inf),
    \dur, 0.1,
    \red, 0,
    \green, 255,
    \blue, 0,
    \white, 0
).play;
)
y.stop;

(
z = Pbind(
    \type, \led,
    \numLED, ~numLED,
    \dur, Pseq([0.55,0.5,0.5, 0.5, 0.125, 0.125, 0.125,0.125,0.125,0.125, 0.125, 0.125],inf),
    \white, Pseq([255,0], inf),
    \tempo, 120/60
).play;
)
z.stop;

p.close;

