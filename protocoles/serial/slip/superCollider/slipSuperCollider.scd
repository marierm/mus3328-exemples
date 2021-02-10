// La classe SLIPSerial doit être installée dans le dossier Extensions de SC
// Vous pouvez demander à SuperCollider où se trouve le dossier Extension sur votre système en évaluant:
Platform.userExtensionDir;
// Redémarrez SuperCollider apès avoir ajouté le fichier au dossier

// Idem pour la classe BusPlot.

SLIPSerial.listDevices; // Lister les périphériques série.
s.boot;
~bus = Bus.control(s,2); // Bus deux canaux.

// à utiliser avec slipInOut.ino
(
~port = SLIPSerial("COM5",115200);

// paquet SLIP reçu.
~port.readSLIP({
    arg slipPacket;
    var sensors=[];
    slipPacket.pairsDo({ // [msbA, lsbA, msbB, lsbB]
        arg msb, lsb, i;
        sensors = sensors.add(msb<<8 | lsb);
    });
    sensors.postln;
    ~bus.set(*sensors); // l'astériques sort les items du tableau. équivalent à:
    // ~bus.set(sensors[0], sensors[1]);
});
)

BusPlot(~bus);
// Fermer le port avant d'essayer l'exemple suivant.
~port.close;

// Avec un petit GUI.
(
var window, sliders, port, sensors;
// port = SLIPSerial("/dev/ttyACM0",115200); // sous Linux, semblable sous Mac
port = SLIPSerial("COM5",115200); // sous Windows
window = Window("Exemple de communication en SLIP");
sliders	= 3.collect({|i| Slider(bounds:100@10) });
sliders.do({ |i|
	i.action_({ |sl|
		port.writeSLIP(
			sliders.collect({|i| (i.value * 255).asInteger; }).postln;
		);
	})
});
sensors = 2.collect({ |i| Slider(bounds:100@10); });

port.readSLIP({|i|
    var values=[];
    i.pairsDo({
        arg msb, lsb, i;
        values = values.add(msb<<8 | lsb);
        {sensors[i.div(2)].valueAction_( (msb<<8 | lsb) / 1023 ) }.defer;
    });

});


window.layout = VLayout( * (sliders ++ 20 ++ sensors) );
window.front;
window.onClose_({port.close});
);
