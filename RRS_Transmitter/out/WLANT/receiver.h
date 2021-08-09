# ifndef   RECEIVER_H
#   define RECEIVER_H

#include <gsl/gsl_complex.h>
#include "sim_consts.h"//transm
#include "sim_params.h"//transm
//#include "out/WLANT/MyTransmitter.cpp"
#include <complex>
#include <vector>


//void receiver(gsl_complex* time_signal,  //Было так
void receiver(fxp_complex* fxp_time_signal,
               unsigned num_of_samples,
               unsigned char* out_bits,
               unsigned* ABL_bits_per_QAM_symbol,
               unsigned* bits_per_OFDM_symbol,
               char* syncError,
               unsigned* feedBackNumOfOFDMSyms
             );

#endif // RECEIVER_H