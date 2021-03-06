#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import wx
from wx.lib.pubsub import pub
import serial
from serial.tools import list_ports
import slip.SerialComm as SerialComm
import slip.ProtoSLIP as ProtoSLIP
import threading
import math

serialPorts = list_ports.comports()
baudRates = [115200, 57600, 19200, 9600]
numSensorSliders = 2
numLedSliders = 3

class SlipView(wx.Frame):
    def __init__(self, parent, title):
        super(SlipView, self).__init__(parent, title=title, size=(500,400))
        
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

        flexbox.AddMany([
                (textPort, 0, wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL, 0),
                (self.choicePort, 1, wx.EXPAND, 0),
                (textBaud, 0, wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL, 0),
                (self.choiceBaud, 1, wx.EXPAND, 0)
        ])

        hbox0.Add(flexbox, 1, flag=wx.EXPAND|wx.ALL, border=2)


        self.openButton = wx.ToggleButton(panel, label='Open Port')
        hbox0.Add(self.openButton, 1, flag=wx.EXPAND|wx.ALL, border=2)

        vbox0.Add(hbox0, 0, flag=wx.EXPAND|wx.ALL, border=2)
        # tu rajoutes dans vbox0 pour aller en dessous...

        self.sensorSliders = []
        self.ledSliders = []

        for i in range(numSensorSliders):
            vbox0.Add(wx.StaticText(panel, id=-1, label=("Sensor " + str(i))), 1, wx.ALIGN_BOTTOM, border=5)
            self.sensorSliders.append(wx.Slider(panel))
            vbox0.Add(self.sensorSliders[i], 1, wx.EXPAND, border=5)
            self.sensorSliders[i].SetRange(0,1023)

        for i in range(numLedSliders):
            vbox0.Add(wx.StaticText(panel, id=-1, label=("LED " + str(i))), 1, wx.ALIGN_BOTTOM, border=5)
            self.ledSliders.append(wx.Slider(panel))
            vbox0.Add(self.ledSliders[i], 1, wx.EXPAND, border=5)
            self.ledSliders[i].SetRange(0,255)


        # self.slider1 = wx.Slider(panel)
        # self.slider2 = wx.Slider(panel)
        
        # flexbox.AddMany([
        #     (textPort, 0, wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL, 0),
        #     (self.choicePort, 1, wx.EXPAND, 0),
        #     (textBaud, 0, wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL, 0),
        #     (self.choiceBaud, 1, wx.EXPAND, 0)
        # ])

        panel.SetSizer(vbox0)
        

class SlipController:
    def __init__(self, app):
        self.slip = Slip()  # Model
        self.view = SlipView(None, 'Slip') # View
        self.view.openButton.Bind(wx.EVT_TOGGLEBUTTON, self.openSlipPort)
        for i in range(numLedSliders):
            self.view.ledSliders[i].Bind(wx.EVT_SLIDER, self.sendSliderData)
        pub.subscribe(self.sensorDataChanged, "sensorData")

        # sliderList = []
        # for i in self.slip.sliderNames:
        #     sliderList.append(i)
        # self.view.checkList.Set(featureList)

        # for i in self.slip.features:
        #     i.activate()

        # self.oscTargetChanged()

    def openSlipPort(self, evt):
        if (evt.GetInt() == 1):
            self.slip.go = 1
            self.slip.openPort(
                port=self.view.choicePort.GetStringSelection(),
                baudrate=self.view.choiceBaud.GetStringSelection()
            )
        else:
            self.slip.go = 0

    def sensorDataChanged(self, data):
        for i in range(numSensorSliders):
            self.view.sensorSliders[i].SetValue(data[i])

    def sendSliderData(self, evt):
        data = []
        for i in range(numLedSliders):
            data.append(self.view.ledSliders[i].GetValue())
        self.slip.sendSliderData(data)
        
class Slip():
    def __init__(self):
        self.go = 1

        self.sliderNames = [
            'sensor A', 'sensor B', 'LED A', 'LED B'
        ]

    def readAndUpdate(self):
        while (self.go):
            self.values = []
            self.bytes = ProtoSLIP.decodeFromSLIP(self.ser)
            for i in xrange(0, len(self.bytes), 2):
                self.values.append(self.bytes[i]<<8 | self.bytes[i+1])
            self.updateSliders()
        self.ser.flush()
        SerialComm.disconnectFromSerialPort(self.ser)
        print "Port closed."
        return

    def openPort(self, port='/dev/ttyUSB0', baudrate=115200):
        self.ser = SerialComm.connectToSerialPort(port, baudrate)
        self.thread = threading.Thread(target=self.readAndUpdate)
        self.thread.daemon = True
        self.thread.start()

    def updateSliders(self):
        pub.sendMessage("sensorData", data = self.values)
        # print self.values

    def sendSliderData(self, data):
        # sliderData = []
        SerialComm.writeToSerialPort(self.ser, ProtoSLIP.encodeToSLIP(data))
        print data

if __name__ == '__main__':
    app = wx.App()
    SlipController(app)
    app.MainLoop()
