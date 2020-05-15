#include <systemc.h>
#include "Transactor.h"

enum operation {WRITE=false, READ=true};

void Transactor::word_read(unsigned x, unsigned y, int& d)
{
    static const auto Z = sc_lv<32>(SC_LOGIC_Z);
    cout << "INFO: " << name() << "::read starting @ " << sc_time_stamp() << " Addr (" << x << "," << y << ")" << endl;
        wait(iclk -> posedge_event());
    if(x == 0 && y == 0) {
        wait(iclk -> posedge_event());
        LD -> write(true);
        RW -> write(READ);
        X -> write(x);
        Y -> write(y);
    }
    wait(iclk->posedge_event());
    d = D->read().to_long();
    
    if (y < 8) {
        if (x < 8) {
		    LD -> write(true);
		    RW -> write(READ);
		    X -> write(x);
		    Y -> write(y);
        }
        else
            LD -> write(true);
        
    }
	if (y == 8) {
		x = x + 1;
		y = 0;
	}
    y = y + 1;
    
}

void Transactor::word_write(unsigned x, unsigned y, int d)
{
    cout << "INFO: " << name() << "::write starting @ " << sc_time_stamp() << " Addr (" << x << "," << y << ")" << endl;
    wait(iclk->posedge_event());
    if (x < 8) {
		wait(iclk -> posedge_event());
		LD -> write(true);
		RW -> write(WRITE);
		X -> write(y);
		Y -> write(x);
		D -> write(d);
        x = x + 1;
	}
    
	if (x == 7 ){
		wait(iclk -> posedge_event());
		LD -> write(false); 
	}
 
}
