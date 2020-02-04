# exemple d'utilisation de slip avec pyo en python3

from pyo_serial_tools import SLIPIO
from pyo import *

s = Server().boot()

# sig avec interpolation de pyo avec un buffer de 3 valeurs (à augmenter si vous avez plus de seunseurs)
sig = SigTo([0]*3)


def slipcallback(vals, write_to_serial):
    """
    fonction qui sera appelée à chaque paque slip reçu par le programme.
    vals: valeurs en entiers 16 bits reçues
    write_to_serial: est une fonction permettant d'écrire une liste de valeurs entre 0 et 255 sur le port série par le protocole slip
    """
    # change les valeurs du SigTo pour les nouvelles valeurs de senseur lues
    sig.value = vals
    # réécrit les valeurs reçues en les shiftant sur 8 bits
    write_to_serial([val>>2 for val in vals])

# création du receveur de packets slips en lui passant l'emplacement du port série, le baud rate et le callback
slip_receiver = SLIPIO(port_name="/dev/ttyACM0", baud_rate=115200, callback=slipcallback)

slip_receiver.start()

# controle de la fréquence d'oscillateurs avec nos données de senseur
sound = LFO(freq=sig,  mul=0.3)
sound.out()

s.start()
s.gui(locals)
