# -*- coding: utf-8 -*-
# Code volé à  Asanka P. Sayakkara ici:
# http://recolog.blogspot.ca/2012/10/serial-line-internet-protocol-slip.html

import termios  
import serial  
from collections import deque  
SLIP_END = 0300          # Ces nombres sont en base 8 (octal)
SLIP_ESC = 0333          # En décimal, ils correspondent à: 192, 219, 220 et
                         # 221
SLIP_ESC_END = 0334  
SLIP_ESC_ESC = 0335  
DEBUG_MAKER = 0015  
MAX_MTU = 200  
readBufferQueue = deque([])  
#-------------------------------------------------------------------------------  
# Cette fonction reçoit une liste d'octets (bytes) et les encode en SLIP.
def encodeToSLIP(byteList):  
     tempSLIPBuffer = []  
     tempSLIPBuffer.append(SLIP_END)  
     for i in byteList:  
          if i == SLIP_END:  
               tempSLIPBuffer.append(SLIP_ESC)  
               tempSLIPBuffer.append(SLIP_ESC_END)  
          elif i == SLIP_ESC:  
               tempSLIPBuffer.append(SLIP_ESC)  
               tempSLIPBuffer.append(SLIP_ESC_ESC)  
          else:  
               tempSLIPBuffer.append(i)  
     tempSLIPBuffer.append(SLIP_END)  
     return tempSLIPBuffer  
#-------------------------------------------------------------------------------  
#-------------------------------------------------------------------------------  
# Cette fonction utilise getSerialByte() pour obtenir des octets encodés en SLIP du port série.
def decodeFromSLIP(serialFD):  
     dataBuffer = []  
     while 1:  
          serialByte = getSerialByte(serialFD)  
          if serialByte is None:  
               return -1  
          elif serialByte == SLIP_END:  
               if len(dataBuffer) > 0:  
                    return dataBuffer  
          elif serialByte == SLIP_ESC:  
               serialByte = getSerialByte(serialFD)  
               if serialByte is None:  
                    return -1  
               elif serialByte == SLIP_ESC_END:  
                    dataBuffer.append(SLIP_END)  
               elif serialByte == SLIP_ESC_ESC:  
                    dataBuffer.append(SLIP_ESC)  
               elif serialByte == DEBUG_MAKER:  
                    dataBuffer.append(DEBUG_MAKER)  
               else:  
                    print("Protocol Error")  
          else:  
               dataBuffer.append(serialByte)  
     return            
#-------------------------------------------------------------------------------  
#-------------------------------------------------------------------------------  
# Cette fonction lit les données sur le port série et retourne un octet à la fois.
def getSerialByte(serialFD):       
     if len(readBufferQueue) == 0:  
          #fetch a new data chunk from the serial port       
          i = 0  
          while len(readBufferQueue) < MAX_MTU:  
               newByte = ord(serialFD.read())  
               readBufferQueue.append(newByte)  
          newByte = readBufferQueue.popleft()  
          return newByte  
     else:  
          newByte = readBufferQueue.popleft()  
          return newByte  
#-------------------------------------------------------------------------------  
