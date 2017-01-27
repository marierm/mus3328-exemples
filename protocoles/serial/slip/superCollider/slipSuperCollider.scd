// La classe SLIPSerial doit être installée dans le dossier Extensions de SC
SLIPSerial.listDevices; // Lister les périphériques série.


// à utiliser avec slipInOut.ino
(
~port = SLIPSerial("/dev/ttyACM0",115200); // Changer le nom du port
										   // et le baudrate au besoin.
~inc = 0; // Un compteur

// La fonction passée en paramètre à readSLIP est executée à chaque
// paquet SLIP reçu.
~port.readSLIP({|i| 
	var val;
	val = ((i[0] << 8) | i[1])  >> 2;
	(val.asInteger ! 3).postln;
	((~inc % 10) == 0).if({ // On renvoie un message SLIP une fois sur 10.
		~port.writeSLIP( val.asInteger ! 3 );
		// writeSLIP écrit les octets sur le port série.
	});
	~inc = ~inc + 1;
});
)

// Fermer le port avant d'essayer l'exemple suivant.
~port.close;


// Avec un petit GUI.
(
var window, sliders, port, sensor;
port = SLIPSerial("/dev/ttyACM0",115200);
window = Window("Exemple de communication en SLIP");
sliders	= 3.collect({|i| Slider(bounds:100@10) });
sliders.do({ |i|
	i.action_({ |sl|
		port.writeSLIP(
			sliders.collect({|i| (i.value * 255).asInteger; }).postln;
		);
	})
});
sensor = Slider(bounds:100@10);

port.readSLIP({|i| {sensor.valueAction_( ((i[0] << 8) | i[1]) / 1023)  }.defer; });

window.layout = VLayout( * sliders ++ 20 ++ sensor); 
window.front;
window.onClose_({port.close});
);
