#include <math.h>
#include "pav_analysis.h"

float compute_power(const float *x, unsigned int N) {
    // vamos a calcular la potencia media de una señal, suma de valores al cuadrado dividido por el 
    // numero de elementso 
    float pot=1e-12;
    for(unsigned int n=0; n<N; n++){
        pot+= x[n]*x[n];
    } 
    return 10*log10(pot/N);
}
// copiar funcion y hacerlo con el tocho
float hamming(unsigned int i, unsigned int N){
    float w = 1e-12;
    float ao=0.53836;
    float a1=0.46164;
    w=ao-a1*cos((2*3.1415*i)/N);
    return w;

}
float compute_power_ham(const float *x, unsigned int N,const float *w ) {
  
    float num=0;
    float den=0;
    for(unsigned int n=0; n<N; n++){
        num+= (x[n]*w[n])*(x[n]*w[n]);
        den+=w[n]*w[n];
    } 
    return 10*log10(num/den);
}
float compute_am(const float *x, unsigned int N) {
    float am=1e-12;
    for(unsigned int n=0; n<N; n++){
        am+= fabs(x[n]);
    } 
    return am/N;
}

float compute_zcr(const float *x, unsigned int N, float fm) {
    float zrc=0;

    for(unsigned int n=1; n<N; n++){
        if(x[n]*x[n-1]<0){
             zrc+= 1;
        }
    } 
    return zrc*fm/(2*(N-1));
}
