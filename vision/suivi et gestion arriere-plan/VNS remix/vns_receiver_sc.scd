s.boot;

(
//se déclarer un synth
SynthDef(\SFlooper, { arg bufnum, amp, upTime, downTime;
	var sound, e;
	e = LagUD.kr(amp, upTime, downTime);
	sound = DiskIn.ar(2, bufnum, 1) * e;
	Out.ar(0, sound);
}).send(s);
)

(
//mettre des sons dans un buffer
a = Buffer.cueSoundFile(s, PathName.new(ScIDE.currentPath).pathOnly ++ "audiofiles/" ++ "bruit.aif", 0);
b = Buffer.cueSoundFile(s, PathName.new(ScIDE.currentPath).pathOnly ++ "audiofiles/" ++ "pad.aif", 0);

//partir 2 instances du synth
~bruit = Synth.new(\SFlooper, [bufnum: a, amp: 0, upTime: 0.1, downTime: 1.5]);
~pad = Synth.new(\SFlooper, [bufnum: b, amp: 0, upTime: 0.5, downTime: 0.5]);
~onset = 0;
//modifier les paramètres des synth avec ce qui arrive par osc
n = OSCdef.new(\rcv,{ arg msg, time;
	var in;

	//mapping
	in = msg[1].linlin(1000000, 11000000, 0, 1.0, true);

	//direct mapping
	~pad.set(\amp, in * 0.5);

	//thresholding - triggering
	case{(in > 0.5)  && (~onset == 0) } {~onset = 1;
		~bruit.set(\amp, 0.9); }
	{(in < 0.4) && (~onset == 1) } {~onset = 0;
		~bruit.set(\amp, 0.0); };
	},
	'/mvt', recvPort: 55554);
)

s.freeAll;


