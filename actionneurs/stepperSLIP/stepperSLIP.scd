p = SLIPSerial("COM5", 115200);



p.writeSLIP([15, 1]);


p.close;