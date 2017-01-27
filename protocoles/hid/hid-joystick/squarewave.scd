s.boot;

//le synth
(
SynthDef(\joySquareWave, { arg pitch, attacktime = 0.001, dur = 0.3, gate, gate2, vol, pulseWidth, pan = 1.0, outBus;
	var sound, sound2, e, trigRate, trig, e2, e3, tailVol, freq;

	//l'enveloppe et le son de tête(asr)
	e = EnvGen.kr(Env.asr(attacktime, 1, 0.001, -4), gate);
	freq = Lag.kr(pitch, 0.1);
	sound = LFPulse.ar([freq, freq + 1], 0, pulseWidth, 0.3) + LFPulse.ar([freq - 40.midicps, freq - 40.midicps + 1], 0, pulseWidth, 0.6) * e;

	//les enveloppes et le son du release trigger
	trigRate = TRand.kr(7, 18, gate2);
	tailVol = TRand.kr(2.51, 3.6, gate2);
	trig = Impulse.kr(trigRate);
	e2 = 	EnvGen.kr(Env.perc(0.001, 1/trigRate, 1, -1), trig);
	e3 = 	EnvGen.kr(Env.perc(0.31, dur, 1, -4), gate2);
	sound2 = LFPulse.ar(freq + TRand.kr(4, 25, trig), 0, pulseWidth, 0.2) + LFPulse.ar((freq-40.midicps) + TRand.kr(4, 25, trig), 0, pulseWidth, 0.4) * e2 * e3 * tailVol;

	Out.ar(outBus, Pan2.ar((sound + sound2) * vol, pan));
}).send(s);
)


(
//les responders OSC (old school)
~squareWave1 = Synth.new(\joySquareWave); ~squareWave1.set(\outBus, 0);
~joystickPitchFloor = 31;
~joyResp1.remove;
~joyResp1 = OSCresponderNode(nil,"/joyButton",{ arg time, addr, msg;
	//msg.postln;
	case{msg[1] == 0} {
		~squareWave1.set(\gate, msg[2], \gate2, 1-msg[2]);};

}).add;

~joyResp2.remove;
~joyResp2 = OSCresponderNode(nil,"/joyAxis",{ arg time, addr, msg;
	//msg.postln;
	case{msg[1] == 0} {
	~squareWave1.set(\pulseWidth, msg[2].linlin(-1, 1, 0.01, 0.5),
		\pan, msg[2].linlin(0, 255, -1.0, 1.0)); }
	{msg[1] == 1} {
	~squareWave1.set(\pitch,msg[2].linlin(-1, 1, 86.midicps, ~joystickPitchFloor.midicps)); }

	{msg[1] == 2} {
		~squareWave1.set(\vol, msg[2].linlin(-1, 1, 0.4, 0));};
}).add;
)
