TEST_STRING = "JUST Monika, Hello moNIka"
GOLD_STRING = "JUST MONIKA, HELLO MONIKA"
import lab31_vpi as V

def Write():
    '''V.WriteBus(1, ord(c))
    yield
    V.WriteBus(0, 0)
    yield'''
    for i in range (len(TEST_STRING)):
        V.WriteBus(1, ord(TEST_STRING[i]))
        yield
        

def Check():
    #global check_idx
    #for check_idx in range(len(TEST_STRING)):
    if check_idx >= len(GOLD_STRING):
        print("We don't expect any character")
        return

    valid, data = V.ReadBus()
    if valid:
        if data == ord(GOLD_STRING[i]):
            print("Expect '%c' == Get '%c'"% (GOLD_STRING[check_idx],chr(o_char_value)))
        else:
            print("Expect '%c' == Get '%c'"% (GOLD_STRING[check_idx],chr(o_char_value)))
    check_idx += 1
    yield
    
def CycleGenerator():
    from itertools import zip_longest, repeat
    yield from zip_longest(Write(), Check())
    yield from repeat(None)

CycleObject = CycleGenerator()

def Cycle():
    #print("TODO")
    next(CycleObject)
    pass


