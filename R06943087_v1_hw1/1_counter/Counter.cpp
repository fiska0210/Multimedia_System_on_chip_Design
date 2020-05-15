#include "Counter.h"

void  Counter::ProcessMethod() {
	if(rst_.read() == true) {
	    v_ = 0;
	} 
    else if(enable_.read() == true) {
        if (up_down_.read() == true) {
            if (v_ < 15)
                v_ = v_+1;
        }
        else {
            if (v_ > 0)
                v_ = v_-1;
        }
    }
    value_.write(v_);
}


