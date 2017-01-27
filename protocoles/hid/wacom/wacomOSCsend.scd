// Code tiré de la documentation de SuperCollider.
// Adapté pour une tablette graphique Intuos 3.

GeneralHID.buildDeviceList;
GeneralHID.postDevices;
// On note que ma tablette Wacom Intuos est à l'indice 9 (cette fois-ci).
a = GeneralHID.open( GeneralHID.deviceList[9] )
// On imprime les infos sur cette interface.
a.info;
// On peut utiliser la méthode findArgs pour relocaliser la tablette Wacom
// plus tard.  Il est tout à fait possible que son indice change après un
// redémarrage.  Utiliser ses caractéristiques spécifiques est donc une
// solution à long terme beaucoup plus viable.
a.info.findArgs;
// Pour ma tablette Intuos 4: [ 1386, 176, , 258 ]
// On ferme.  On révouvrira à partir des infos précédentes.
a.close;
b = GeneralHID.findBy( 1386, 176 );
// On peut ensuite réouvrir comme ceci:
a = GeneralHID.open( b )
// Start eventloop:
GeneralHID.startEventLoop
// Les possiblités de l'interface:
a.caps;
// Il y a différents types de paramètres (slots):
// button (type 1), has only on/off (1/0) states
// relative (type 2), counts up or down (scrollwheel for example)
// absolute (type 3), continuous value between 0 and 1
// some other may show up on Linux ( Syn (type 0) and Miscellaneous (type 4), but these are generally not very useful).

// Regarder les données:
a.debug_( true );
// Arrêter:
a.debug_( false );

// Il est possible de faire un "grab" (sous Linux uniquement, pour l'instant)
// pour éviter que le périphérique soit pris en comptes par d'autres
// applications.  Cela nous assure que le pointeur n'est pas contrôlé par la
// tablette.
a.grab;

// une interface graphique peut être générée automatiquement:
a.makeGui;

// Des actions (fonctions) peuvent être assignées à chacun des paramètres de
// contrôle (slots).
// Ici, sur un bouton:
a.slots[1].at( 256 ).action_( { "Ce tu wla, gwassideurw".postln; } );

// Si on veut des fonctions différentes quand le bouton est enfoncé ou
// relâché:
a.slots[1].at( 256 ).action_( { |v|
	v.value.asInteger.switch(
		1, {"Ce tu wla, gwassideurw".postln;},
		0, {"Wayne Gretzky".postln;}
	);
} );


// Pour des contrôleurs continus:
a.slots[3].at( 0 ).action_( { |v| "Position en x: ".post; v.value.postln; } );

// Pour désassigner l'action, il faut assigner une action vide.
a.slots[1].at( 256 ).action_( {} );
a.slots[3].at( 0 ).action_( {} );


// Pour envoyer des messages OSC:
a.slots[3][24].action_( { |v|
	NetAddr.localAddr.sendMsg("/asynchrone/0/amp", v.value)
} );

a.slots[3][0].action_( { |v|
	NetAddr.localAddr.sendMsg("/asynchrone/0/posLo", v.value)
} );

a.slots[3][1].action_( { |v|
	NetAddr.localAddr.sendMsg("/asynchrone/0/posHi", v.value)
} );


///////////////////////////////////////////////
// Pour envoyer des messages OSC au granulotron:

// Créer l'adresse de destination:
(
~dest = NetAddr("localhost", 9050);
~oscBasePath = "/tablet/";
~oscMsgNames = [\posx, \posy, \tiltx, \tilty, \press, \dist, \touch, \btn1, \btn2];
~tabletSlots = [[3,0], [3,1,\flip], [3,26], [3,27,\flip], [3,24], [3,25], [1,330], [1,331], [1,332]];

~tabletSlots.do({|slot, index|
	a.slots[slot[0]][slot[1]].action_({|v|
		var value;
		value = v.value;
		slot[2].notNil.if({
			value = (value * -1) + 1;
		});
		~dest.sendMsg(~oscBasePath ++ ~oscMsgNames[index], value)
	})
});

)

a.close;
GeneralHID.stopEventLoop;
