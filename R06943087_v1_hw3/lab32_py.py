from nicotb import *
from nicotb.protocol import OneWire
import numpy as np

TEST_STRING = "just Monika, Hello moNIka"
GOLD_STRING = "JUST MONIKA, HELLO MONIKA"

def main():
    yield rs_ev
    master = OneWire.Master(iv, ic, ck_ev) 
    slave = OneWire.Slave(ov, oc, ck_ev, callbacks=[Check]) 
    values = master.values 

    for i in range(len(TEST_STRING)):    
        values.i_char[0] = ord(TEST_STRING[i])
        yield ck_ev
        yield ck_ev
        yield from master.Send(values) 
        yield ck_ev
        yield ck_ev 

check_idx = 0

def Check(data): 
    global check_idx
    if check_idx >= len(GOLD_STRING):
        print("We don't expect any character")
        return   

    o_char_value = data.values.o_char[0]
	#print(chr(o_char_value))
    if o_char_value == ord(GOLD_STRING[check_idx]):
        print("Expect '%c' == Get '%c'"% (GOLD_STRING[check_idx],chr(o_char_value)))
    else:
        print("Expect '%c' == Get '%c'"% (GOLD_STRING[check_idx],chr(o_char_value)))
    check_idx += 1

ck_ev, rs_ev = CreateEvents(["ck_ev", "rst_out",])
iv, ic, ov, oc = CreateBuses([   
	(("u_to_upper", "i_valid"),), 
	(("u_to_upper", "i_char"),), 
	(("u_to_upper", "o_valid"),),
	(("u_to_upper", "o_char"),), 
])
RegisterCoroutines([
    main(),
])
