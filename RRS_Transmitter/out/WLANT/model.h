# ifndef   MODEL_H
#   define MODEL_H

//#define LOGGING
//#define TEST_SEQ
//#define AQ_LOG
//#define ABL_TEST
//#define LDPC_TEST
//#define AGC_TEST

#include <gsl/gsl_randist.h>

//[ Random number generator for GSL
extern gsl_rng* RNG;
//]

#ifdef LOGGING
  FILE* debug_log;
#endif

#ifdef LOGGING
  FILE* debug_log;
#endif

#ifdef AQ_LOG
  FILE* aq_log;
#endif

#ifdef AQ_LOG
  FILE* agc_log;
#endif

static char overflow_comment[200];
//char overflow_comment2[200];  // Было так


void initialize_model();
void destroy_model();

void logging( char* log_string, char block_begin );

#endif // MODEL_H
