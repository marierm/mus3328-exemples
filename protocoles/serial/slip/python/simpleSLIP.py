#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import serial
import time
import slip.SerialComm as SerialComm
import slip.ProtoSLIP as ProtoSLIP


portName = "/dev/ttyACM0"
baudRate = 115200
go = 1

serialPort = SerialComm.connectToSerialPort(portName, baudRate)

while (go):
    values = []
    # Lire un paquet SLIP
    bytes = ProtoSLIP.decodeFromSLIP(serialPort)
    # Faire quelque chose avec ce paquet:
    for i in xrange(0, len(bytes), 2):
        values.append(bytes[i]<<8 | bytes[i+1])
    print values
    time.sleep(0.001)

# Dans un contexte où on voudra lire le port série et utiliser les données
# pour contrôler une tâche (un traitement audionumérique avec Pyo, par
# exemple), on voudra utiliser un mécanisme de threading.
# 
# Pour cela, le module "threading" est recommandé:
# https://docs.python.org/2/library/threading.html
