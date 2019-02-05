#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import serial
import time
# import slip.SerialComm as SerialComm
# import slip.ProtoSLIP as ProtoSLIP
import threading
import wx

END=192
ESC=219
ESC_END=220
ESC_ESC=221
portName = "/dev/ttyUSB0"
baudRate = 115200
go = 1

port = serial.Serial(portName, baudRate)

def getSlipPacket():
    packet = []
    while go:
        byte = ord(port.read())
        if (byte == ESC):
            byte = ord(port.read())
            if (byte == ESC_END):
                packet.append(END)
            elif (byte == ESC_ESC):
                packet.append(ESC)
            else:
                print( "SLIP packet error.")
        elif (byte == END):
            if packet: # si le paquet n'est pas vide,
                # on fait quelque chose avec.
                # Ici, on présume que ce sont de données de capteurs à 10 bits et on imprime.
                values = []
                for i in range(0, len(packet), 2):
                    values.append(packet[i]<<8 | packet[i+1])
                print( values ) # On imprime, mais on pourrait appeler une autre fonction.
                packet = []
        else:
            packet.append(byte)


thread = threading.Thread(target=getSlipPacket())
thread.daemon = True
thread.start()

