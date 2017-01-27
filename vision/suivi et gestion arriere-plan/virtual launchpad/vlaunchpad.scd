s.boot;

(
//se déclarer un synth
SynthDef(\square, { arg freq, pulseWidth, amp, upTime, downTime;
	var sound, e;

	e = LagUD.kr(amp, upTime, downTime);
	sound = LFPulse.ar([freq, freq + 1], 0, pulseWidth, e);

	Out.ar(0, sound);
}).send(s);
)

(
//partir 25 instances du synth
~square = List.new;
25.do{ arg i; ~square.add(Synth.new(\square, [freq: 250 + (37*i), pulseWidth: i.linlin(0, 24, 0, 0.5), amp: 0, upTime: 0.1, downTime: 1.5]))};

//modifier les paramètres des synth avec ce qui arrive par osc
n = OSCdef.new(\trig,{ arg msg, time, addr="127.0.0.1", recvPort=13000;
	//msg.postln;
	case{msg[3] == 1} { ~square[(msg[1]*5)+ msg[2]].set(\amp, 0.1); }
	{msg[3] == 0} { ~square[(msg[1]*5)+ msg[2]].set(\amp, 0.0); };
	},
	'/trig', recvPort: 13000);
)

s.freeAll;


~square[0].set(\amp, 0.1);
~square[0].set(\amp, 0.0);
n.free;