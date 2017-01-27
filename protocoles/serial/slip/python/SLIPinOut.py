#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import serial
import time
import slip.SerialComm as SerialComm
import slip.ProtoSLIP as ProtoSLIP
import threading
import wx

portName = "/dev/ttyACM0"
baudRate = 115200
go = 1

serialPort = SerialComm.connectToSerialPort(portName, baudRate)

def readAndUpdate():
    while (go):
        values = []
        # Lire un paquet SLIP
        bytes = ProtoSLIP.decodeFromSLIP(serialPort)
        # Faire quelque chose avec ce paquet:
        for i in xrange(0, len(bytes), 2):
            values.append(bytes[i]<<8 | bytes[i+1])
        print values
        time.sleep(0.001)
    serialPort.flush()
    SerialComm.disconnectFromSerialPort(serialPort)
    print "Port closed."
    return

thread = threading.Thread(target=readAndUpdate())
thread.daemon = True
thread.start()


class SlipView(wx.Frame):
    def __init__(self, parent, title):
        super(SpongeView, self).__init__(parent, title=title, size=(500,400))
        
        self.InitUI()
        self.Centre()
        self.Show() 

    def InitUI(self):
        panel = wx.Panel(self)
        panel.SetBackgroundColour("#DDDDDD")

        # main box
        vbox0 = wx.BoxSizer(wx.VERTICAL)
        # la premiere ligne de widgets
        hbox0 = wx.BoxSizer(wx.HORIZONTAL)

        # grid avec les textes et popups
        flexbox = wx.FlexGridSizer(5,2,2,2)
        flexbox.AddGrowableCol(1)
       
        ports = []
        for i in serialPorts:
            ports.append(i[0])
        textPort = wx.StaticText(panel, id=-1, label="Serial Port: ")
        self.choicePort = wx.Choice(panel, id=-1, choices=ports)

        rates = [];
        for i in range(len(baudRates)):
            rates.append(str(baudRates[i]))
        textBaud = wx.StaticText(panel, id=-1, label="Baudrate: ")
        self.choiceBaud = wx.Choice(panel, id=-1, choices=rates)
        textHost = wx.StaticText(panel, id=-1, label="OSC Host Name: ")
        self.oscHost = wx.TextCtrl(panel, id=-1, style=wx.TE_PROCESS_ENTER)
        textPath = wx.StaticText(panel, id=-1, label="OSC Base Path: ")
        self.oscPath = wx.TextCtrl(panel, id=-1, style=wx.TE_PROCESS_ENTER)
        textOscPort = wx.StaticText(panel, id=-1, label="OSC Port: ")
        self.oscPort = wx.TextCtrl(panel, id=-1, style=wx.TE_PROCESS_ENTER)

        flexbox.AddMany([
                (textPort, 0, wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL, 0),
                (self.choicePort, 1, wx.EXPAND, 0),
                (textBaud, 0, wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL, 0),
                (self.choiceBaud, 1, wx.EXPAND, 0),
                (textHost, 0, wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL, 0),
                (self.oscHost, 1, wx.EXPAND, 0),
                (textPath, 0, wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL, 0),
                (self.oscPath, 1, wx.EXPAND, 0),
                (textOscPort, 0, wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL, 0),
                (self.oscPort, 1, wx.EXPAND, 0)
        ])

        hbox0.Add(flexbox, 1, flag=wx.EXPAND|wx.ALL, border=2)


        self.openButton = wx.ToggleButton(panel, label='Open Port')
        hbox0.Add(self.openButton, 1, flag=wx.EXPAND|wx.ALL, border=2)

        vbox0.Add(hbox0, 0, flag=wx.EXPAND|wx.ALL, border=2)
        # tu rajoutes dans vbox0 pour aller en dessous...
        
        self.checkList = wx.CheckListBox(panel)
        
        vbox0.Add(self.checkList, 1, wx.EXPAND, border=5)
        panel.SetSizer(vbox0)


if __name__ == '__main__':
    app = wx.App()
    SpongeController(app)
    app.MainLoop()
