MIDIFunc.trace(true);

(
SynthDef(\mpeTest, {
    arg out=0, freq=440, pos=0, width=0.5, bend=0, amp=0.1, gate=1;
    var sig, env;
    freq = freq * bend.midiratio;
    env = EnvGen.kr(Env.adsr, gate, amp, doneAction: 2);
    sig = VarSaw.ar([freq, freq*1.03],0, Lag.kr(width,0.01), env);
    sig = Balance2.ar(sig[0], sig[1], pos);
    Out.ar(out, sig);
}).add;
)

(
~notes = Array.fill(16, nil);
MIDIdef.noteOn(\on, {
    arg vel, note, chan;
    ~notes[chan] = Synth(\mpeTest, [
        \amp, vel.linlin(0, 127, 0.0, 0.5),
        \freq, note.midicps
    ]);
});
MIDIdef.noteOff(\off, {
    arg vel, note, chan;
    ~notes[chan].release;
    ~notes[chan] = nil;
});
MIDIdef.bend(\bend, {
    arg bend, chan;
    ~notes[chan].set(\bend, bend.linlin(0,16383,-7.0, 7.0) );
});
MIDIdef.cc(\cc74, {
    arg val, cc, chan;
    ~notes[chan].set(\pos, val.linlin(0,127,-1.0, 1.0));
});
MIDIdef.touch(\touch, {
    arg val, chan;
    ~notes[chan].set(\width, val.linexp(0,127,0.5, 0.001));
});
)