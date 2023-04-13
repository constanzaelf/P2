#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "pav_analysis.h"

#include "vad.h"

const float FRAME_TIME = 10.0F; /* in ms. */

/* 
 * As the output state is only ST_VOICE, ST_SILENCE, or ST_UNDEF,
 * only this labels are needed. You need to add all labels, in case
 * you want to print the internal state in string format
 */

const char *state_str[] = {
  "UNDEF", "S", "V", "INIT","QV","QS"
};

const char *state2str(VAD_STATE st) {
  return state_str[st];
}

/* Define a datatype with interesting features */
typedef struct {
  float zcr;
  float p;
  float am;
} Features;

/* 
 * TODO: Delete and use your own features!
 */

Features compute_features(const float *x, int N) {
  /*
   * Input: x[i] : i=0 .... N-1 
   * Ouput: computed features
   */
  /* 
   * DELETE and include a call to your own functions
   *
   * For the moment, compute random value between 0 and 1 
   */
  Features feat;
  feat.p=compute_power(x,N);
  feat.zcr=compute_zcr(x,N,16000);
  feat.am=compute_am(x,N);
  // feat.zcr = feat.p = feat.am = (float) rand()/RAND_MAX;
  return feat;
}

/* 
 * TODO: Init the values of vad_data
 */

VAD_DATA * vad_open(float rate, float alfa0, float alfa1) {
  VAD_DATA *vad_data = malloc(sizeof(VAD_DATA));
  vad_data->state = ST_INIT;
  vad_data->sampling_rate = rate;
  vad_data->frame_length = rate * FRAME_TIME * 1e-3;

  vad_data->alfa0=alfa0;
  vad_data->alfa1=alfa1;
  vad_data->Min_S = 10;
  vad_data->Min_V = 30;
  vad_data->num_t=9;
  // vamos a ver más tarde si funciona con MIN y MAX
  return vad_data;
}

VAD_STATE vad_close(VAD_DATA *vad_data) {
  /* 
   * TODO: decide what to do with the last undecided frames
   */
  VAD_STATE state = vad_data->state;
  // if(state == ST_QV) state = ST_VOICE;
  // if(state == ST_QS) state = ST_SILENCE;

  free(vad_data);
  return state;
}

unsigned int vad_frame_size(VAD_DATA *vad_data) {
  return vad_data->frame_length;
}

/* 
 * TODO: Implement the Voice Activity Detection 
 * using a Finite State Automata
 */

VAD_STATE vad(VAD_DATA *vad_data, float *x) {

  /* 
   * TODO: You can change this, using your own features,
   * program finite state automaton, define conditions, etc.
   */

  Features f = compute_features(x, vad_data->frame_length);
  vad_data->last_feature = f.p; /* save feature, in case you want to show */

  switch (vad_data->state) {
  case ST_INIT:
    vad_data->state = ST_SILENCE;
    vad_data->P0=f.p + vad_data->alfa0 ;
    vad_data->c1= vad_data->P0 +vad_data->alfa1; // quizas silencio 
    break;

  case ST_SILENCE:
    if (f.p > vad_data->P0)
      vad_data->state = ST_QV;
      
    break;

  case ST_VOICE:
    if (f.p <vad_data->P0){
       // vad_data->num_t=3;
        // vad_data->state = ST_SILENCE;
    }
    else if(f.p <vad_data->c1){
      vad_data->state = ST_QS;
    }
    break;
  
  case ST_QV:
    vad_data->num_t--;
    if(f.p>vad_data->c1){
      vad_data->state=ST_VOICE;
      vad_data->num_t=9;
    }
    if(f.am<1e-6 ){
    vad_data->state = ST_SILENCE;
    vad_data->num_t=9;
    }
    else if((f.p<vad_data->P0)||(vad_data->num_t<=0)){
      vad_data->state=ST_SILENCE;
      vad_data->num_t=9;
    }
    break;

  case ST_QS:
    vad_data->num_t--;
    if((f.p>vad_data->c1)&& f.zcr){
      vad_data->state=ST_VOICE;
      vad_data->num_t=9;
    }else if(f.p<vad_data->P0 || vad_data->num_t<=0){
      vad_data->state=ST_SILENCE;
      vad_data->num_t=9;
    }else if(f.am > 2e-4 ){
      vad_data->state = ST_QV;
      vad_data->num_t=9;
    }
    break;


  case ST_UNDEF:
    break;
  }
  //if (vad_data->state == ST_SILENCE ||
   //   vad_data->state == ST_QV)
  //  return ST_SILENCE;
 // else if (vad_data->state == ST_VOICE ||
   //   vad_data->state == ST_QS)
  //  return ST_VOICE;
 
//if (vad_data->state == ST_SILENCE ||
      //vad_data->state == ST_VOICE// || //vad_data->state == ST_QS ||
//)//vad_data->state == ST_QV)
  //  return vad_data->state;
  //else
  //  return ST_UNDEF;
  if(vad_data->state == ST_INIT){
    return ST_SILENCE;
  }
  if (vad_data->state == ST_SILENCE ||
      vad_data->state == ST_VOICE)
    return vad_data->state;
  else
    return ST_UNDEF;

}

void vad_show_state(const VAD_DATA *vad_data, FILE *out) {
  fprintf(out, "%d\t%f\n", vad_data->state, vad_data->last_feature);
}
