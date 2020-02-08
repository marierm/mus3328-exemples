# modifié pour fonctionner avec python3
# Code stolen from Asanka P. Sayakkara here:
# Code volé à Asanka P. Sayakkara ici:
# http://recolog.blogspot.ca/2012/10/serial-line-internet-protocol-slip.html

import ProtoSLIP
import serial
#-------------------------------------------------------------------------------
# Cette fonction ouvre le port série et retourne un objet FileDescriptor qui représente ce port.
def connectToSerialPort(port='/dev/ttyUSB0', baudrate=115200):
     serialFD = serial.Serial(
          port, baudrate,
          bytesize=8, parity='N', stopbits=1, xonxoff=False, rtscts=False
          )
     # port='/dev/ttyUSB0'- nom du port
     # baudrate=115200  - baud rate
     # bytesize=8           - size of a byte
     # parity='N'           - no parity
     # stopbits=1           - 1 stop bit
     # xonxoff=False           - no software handshakes
     # rtscts=False           - no hardware handshakes
     return serialFD
#-------------------------------------------------------------------------------
# -------------------------------------------------------------------------------
# Cette fonction prend un array d'octets (bytes) et les écrit sur le port
# série en utilisant le protocol SLIP.
def writeToSerialPort(serialFD, byteArray):
     encodedSLIPBytes = ProtoSLIP.encodeToSLIP(byteArray)
     byteString = bytes(encodedSLIPBytes) #convert byte list to a string
     serialFD.write(byteString)
     return
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
# Cette fonction lit le port série et retourne un array de bytes.
def readFromSerialPort(serialFD):
     i = 1
     byteArray = None
     byteArray = ProtoSLIP.decodeFromSLIP(serialFD)
     if byteArray is None:
          return -1
     else:
          return byteArray
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
# Cette fonction ferme le port série
def disconnectFromSerialPort(serialFD):
     serialFD.close()
     return
#-------------------------------------------------------------------------------
