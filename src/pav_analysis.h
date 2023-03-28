#ifndef PAV_ANALYSIS_H
#define PAV_ANALYSIS_H

float compute_power(const float *x, unsigned int N);
float compute_am(const float *x, unsigned int N);
float compute_zcr(const float *x, unsigned int N, float fm);
float hamming(unsigned int i, unsigned int N);
float compute_power_ham(const float *x, unsigned int N,const float *w );

#endif	/* PAV_ANALYSIS_H	*/
