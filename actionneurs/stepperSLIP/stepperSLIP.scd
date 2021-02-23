p = SLIPSerial("COM5", 115200);

p.writeSLIP([244, 1]);


p.close;