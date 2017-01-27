(
//recevoir un message
o = OSCdef.new(\osc_test,{ arg msg;
	msg.postln;
}, nil, recvPort: 8000);

//spécifier une adresse et un port
a = NetAddr.new("127.0.0.1", 7777);
)

//envoyer un message
a.sendMsg("/foo", 15, "hello");

a.free;
o.free;