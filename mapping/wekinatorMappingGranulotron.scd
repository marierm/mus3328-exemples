~numWekOutputs = 5;
~buses = Array.fill(~numWekOutputs, { Bus.control; });

(
OSCdef(\wek, {
    arg msg;
    ~buses.do({ arg bus, i;
        bus.set( msg[i+1]);
    });
}, "/wek/outputs", recvPort:12000);
)


~buf = Buffer.read(Server.default, "02-kraftMoyen.wav".resolveRelative);
~buf.play;

(
SynthDef(\granulotron, {
	arg out=0, buf=0, pan=0.5, rate=0.5, amp=1, centerPos=0.5,
	granFreq=1, granDur=0.2, gate=1, att=0.2, rel=0.01;
	var trig;
    // mise en échelle (scaling)
    granFreq = granFreq.linexp(0.0, 1.0, 0.5, 100);
    granDur = granDur.lincurve(0.0, 1.0, 0.001, 1.0, 4);
    centerPos = centerPos;
    rate = rate.linexp(0.0, 1.0, 0.125, 8.0);
    amp = amp.squared;
    pan = pan.linlin(0.0, 1.0, -1.0, 1.0);
	trig = Impulse.ar(granFreq);
	Out.ar(out,
		GrainBuf.ar(
			numChannels: 2,
			trigger: trig,
			sndbuf: buf,
			rate: rate,
			pos: centerPos,
			dur: granDur,
			pan: pan,
			mul: amp,
			interp: 4
		) * EnvGen.kr(Env.asr(att, 1, rel), gate, doneAction:2);
	)
}).add;
)

x = Synth(\granulotron, [\buf, ~buf]);
x.set(\centerPos, 0.5);
x.set(\amp, 0.5);

x.map(\amp, ~buses[0]);
x.map(\granFreq, ~buses[1]);
x.map(\granDur, ~buses[2]);
x.map(\rate, ~buses[3]);
x.map(\centerPos, ~buses[4]);









