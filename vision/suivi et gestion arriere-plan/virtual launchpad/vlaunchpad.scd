
(
SynthDef(\square, {
    arg freq=220, pulseWidth=0.5, amp=0.1, gate=1;
	var sig, env;
    env = EnvGen.kr(Env.perc, gate, amp, doneAction: 2);
	sig = Pulse.ar([freq, freq*1.03], pulseWidth, env);
	Out.ar(0, sig);
}).add;
)

(
~nbCol = 5;
~nbRow = 5;
~synths = Array.fill(~nbCol * ~nbRow, nil);
// Créer un Synth quand un carré du 'laundpad' est occupé.
OSCdef(\trig, {
    arg msg, time;
    var synthNum;
    synthNum = msg[1]* ~nbCol + msg[2];
    synthNum.postln;
    msg.postln;
    (msg[3] == 1).if({
        ~synths[ synthNum ] = Synth(\square, [\freq, (synthNum + 40).midicps] );
    }, {
        ~synths[ synthNum ].release;
        ~synths[ synthNum ] = nil;
    });
}, "/trig", recvPort: 55555);
)
