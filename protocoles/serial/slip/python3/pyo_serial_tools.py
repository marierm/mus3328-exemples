# code pris en grande partie de mus3328-exemples/protocoles/serial/slip/python/SLIPinOut.py et modifié pour fonctionner en python3
import SerialComm
import ProtoSLIP
import threading
import time

class SLIPIO:
    """
    Cette classe lit des packets SLIP contenant une quantité arbitraire de valeurs sur 16 bits et appelle une fonction callback à chaque packets lus.
    la fonction callback reçoit deux arguments : values et une fonction d'écriture sur le port série pouvant être appellée avec une liste (ou itérable) contenant des valeurs entières entre 0 et 255
    voir testpyoserial.py pour un apperçu de l'utilisation de cette classe
    """
    def __init__(self, port_name, baud_rate, callback, sleep_time=0.002):
        # 0.002 est un chiffre arbitraire, vous pouvez le modifier si vous avez des problèmes de performance
        self.sleep_time = 0.002
        self.port_name = port_name
        self.baud_rate = baud_rate
        self.serial_port = SerialComm.connectToSerialPort(port_name, baud_rate)
        self.callback = callback
        self.thread = None

    def _loop(self):
        while (True):
            bytes = ProtoSLIP.decodeFromSLIP(self.serial_port)
            values = []
            if len(bytes) >= 2:
                for i in range(0, len(bytes), 2):
                    values.append(bytes[i]<<8 | bytes[i+1])
                self.callback(values, lambda x: SerialComm.writeToSerialPort(self.serial_port, x))
            time.sleep(self.sleep_time)
        self.serial_port.flush()
        SerialComm.disconnectFromSerialPort(self.serial_port)
        return

    def start(self):
        self.thread = threading.Thread(target=lambda : self._loop())
        self.thread.daemon = True
        self.thread.start()
