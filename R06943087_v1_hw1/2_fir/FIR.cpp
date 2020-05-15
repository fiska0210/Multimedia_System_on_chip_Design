#include "FIR.h"
#include <numeric>
#include <algorithm>

FIR::FIR(sc_module_name mname, const std::vector<int> &&coeff):
	sc_module(mname),
	cur_(0),
	// fifo depth
	orig_in_(32),
	data_in_(32),
	data_out_(32),
	coeff_(coeff),
	pipe_(coeff.size(), 0)
{
	fputs("filter coefficients: [ ", stdout);
	for (int x: coeff_) {
		printf("%d ", x);
	}
	puts("]");

	// reverse(coeff_.begin(), coeff_.end());
	SC_THREAD(Driver);
	SC_THREAD(FirExecution);
	SC_THREAD(Monitor);
}

void FIR::Driver()
{
	const sc_time DELAY(1,SC_NS);
	const unsigned PTS = 20;
	for (unsigned t = 0; t < PTS; t++) {
		const int data = (t == 10 or t == 12) ? 1 : 0;
		orig_in_.write(data);
		data_in_.write(data);
		wait(DELAY);
	}
}

void FIR::FirExecution()
{	
	int dataread[20];
	while (true) {
		for (int r = 0; r < 20; r++) {
			dataread[r] = data_in_.read();
			//printf("data= %d\n", dataread[r]);
		}
		for (int i = 0; i < 20; i++) {
			int data_tmp = 0;
			int ix = i;
			for (int x_ : coeff_) {
				
				if(ix > 0) {
					data_tmp = data_tmp + x_*(dataread[ix]);
					//printf("x= %d %d %d\n", i, x_, dataread[ix]);
					ix = ix - 1;
				}
			}
			data_out_.write(data_tmp);
	}
	}
}





void FIR::Monitor(void)
{
	for (unsigned i = 0; ; i++) {
		const int data = orig_in_.read();
		const int result = data_out_.read();
		printf("[tap %u] %d %d\n", i, data, result);
		
	}
}


