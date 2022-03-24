// Reçoit les messages OSC envoyés du patch wacom.pd

// Démarrer le serveur SC.
Server.default.boot;	// ou s.boot;

// Exécuter toute la parenthèse qui suit.
(
// Les noms des messages OSC qu'on recevra.
~oscBasePath = "/";
~oscMsgNames = [\x, \y, \p, \tx, \ty, \prox, \touch];
// Les maximums de chacunes des valeurs reçues.
// C'est pour normaliser les valeurs entre 0 et 1.
// ~maxVals = [25400, 20320, 127, 127, 1023, 55, 1, 1, 1];
~maxVals = [1, 1, 1, 1, 1, 1, 1];
~minVals = [-1, -1, 0, -1, -1, 0, 0];
// On crée des bus de control.
~ctlBuses = ();

~oscMsgNames.do({|name|
	~ctlBuses.put(name, Bus.control(Server.default, 1));
});
)


(
// Créer un array de OSCdefs (pour répondre aux messages OSC)
~oscMsgNames.collect({|name, index|
	// On crée un OSCdef pour répondre à chaque message OSC.
	OSCdef(
		name,
		{|msg|	// Fonction exécutée à chaque réception du message OSC.
			var normVal;
			// msg[1] contient la valeur.
			// .linlin normalise entre 0 et 1.
            normVal = msg[1].linlin(~minVals[index], ~maxVals[index], 0.0, 1.0);
            ~ctlBuses[name].set(normVal);	// Pour l'instant, on attribue
											// simplement la valeur au bus de
											// contrôle.

			// Pour imprimer les valeurs (déboguer):
            // name.post; ": ".post;normVal.postln;
		},
		~oscBasePath ++ name, // Créer le path OSC en concaténant.
		recvPort: 9050
	)
});
)

// Visualiser.
(
var w, scopes, updateFunction;
w = Window.new("Valeurs de la tablette", Rect(20, 20, 450, 400));
w.view.decorator = FlowLayout(w.view.bounds);
scopes = ~oscMsgNames.collect({|name, index|
	StaticText(w.view,100@20).string_(name).align_(\right);
	Slider(w.view, 300@20);
});
updateFunction = {
	loop{
		~oscMsgNames.do({|name, i|
			~ctlBuses[name].get({ |val|
				{scopes[i].value_(val)}.defer;
			});
		});
		0.04.wait;
	}
};
updateFunction.fork;
w.onClose({updateFunction.stop});
w.front;
)

// Charger des sons dans des buffers.
(
~fileNames = List[];
~buffers = "sons/*.wav".resolveRelative.pathMatch.collect({ |path|
	// Extraire le nom du fichier.
	~fileNames.add(path.basename);
	// Lire un canal à la fois.
	// On assume que ce sont des fichiers 2 canaux (stéréo).
	[
		Buffer.readChannel(Server.default, path, channels: [0]),
		Buffer.readChannel(Server.default, path, channels: [1])
	]
});
)

(
// Un (autre) granulateur:
SynthDef(\granulotron, {
	arg out=0, bufL=0, bufR=1, pan=0, rate=1, amp=1, centerPos=0.5,
	granFreq=15, granDur=0.2, gate=1, att=0.2, rel=0.01;

	var trig;
	trig = Impulse.ar(granFreq);
	Out.ar(out,
		TGrains.ar(
			numChannels: 2,
			trigger: trig,
			bufnum: [bufL, bufR],		// multichannel expansion!
			rate: rate,
			centerPos: centerPos,
			dur: granDur,
			pan: pan,
			amp: amp,
			interp: 4
		) * EnvGen.kr(Env.asr(att, 1, rel), gate, doneAction:2);
	)
}).add;
)

// On détermine le registre des paramètres et le mappage (un pour un).  Dans
// le Event (Dictionary) ci-dessous, chaque clé porte le nom d'un paramètre de
// granulotron.  L'élément de cette clé est un array des deux éléments: le nom
// de l'OSCdef qui affectera ce paramètre et un ControlSpec qui détermine le
// "scaling".
(
~map = (
	\pan: [nil, ControlSpec(-1, 1, \linear, 0.0, 0, "")], // pas de pan
	\rate: [\y, ControlSpec(0.125, 8, \exp, 0, 1, "")],
	\amp: [\p, ControlSpec(0, 4, \amp, 0.0, 0, "")],
	\centerPos: [\x, ControlSpec()],
	\granFreq: [\tx, ControlSpec(0.5, 800, \exp, 0.0, 0.2)],
	\granDur: [\ty, ControlSpec(0.001, 1.0, 4, 0.0, 0.2)]
);


// On va utiliser des synth pour faire du "scaling".  (On pourrait opter pour
// d'autres stratégies comme faire le scaling du côté du client.)
// On crée les SynthDefs à la chaîne:
~map.keysValuesDo({|key, value|
	SynthDef(key, {
		arg in, out;
		Out.kr(out, value[1].map(in))
	}).add;
});
)

(
~scaleSynths = ~map.collect({|value, name|
	// Un nouveau bus pour chacun des paramètres.
	~ctlBuses.put( name, Bus.control() );
	// Un synth pour chacun des paramètres
	Synth.head(
		Server.default,
		name,
		// Le synth met des valeurs sur le nouveau bus.
		[ \out, ~ctlBuses[name] ]
		// value[0] contient le nom du bus (qui est le même que le nom du
		// message OSC) qui va affecter ce paramètre.
	).map(\in, ~ctlBuses[value[0]]);
});
)

(
~isTouching = 0;
// Toucher la tablette démarre un granulotron.
OSCdef(
	\pressureBinary,
	{|msg|
        (~isTouching !=   (msg[1] > 0.0).asInteger).if({
            ~isTouching = (msg[1] > 0.0).asInteger;
            ~isTouching.switch(
                1,
                {
                    ~granulotron.isPlaying.not.if({
                        ~granulotron = Synth.tail(
                            Server.default,
                            \granulotron, [
                                \bufL, ~buffers[4][0],
                                \bufR, ~buffers[4][1]
                            ]
                        );
                        ~granulotron.map(
                            \rate, ~ctlBuses[\rate],
                            \amp, ~ctlBuses[\amp],
                            \centerPos, ~ctlBuses[\centerPos],
                            \granFreq, ~ctlBuses[\granFreq],
                            \granDur, ~ctlBuses[\granDur],
                        );
                        NodeWatcher.register(~granulotron, true);
                    })
                },
                0, {
                    ~granulotron.isPlaying.if({
                        ~granulotron.set(\gate, 0)
                    });
                }
            )

        });

    }, "/p", recvPort:9050
);
)

// Un peu de ménage.
(
OSCdef.freeAll;
~ctlBuses.do(_.free);
~buffers.do({|i| i.do(_.free)});
~scaleSynths.do(_.free);
~granulotron.free;
)

