#ifndef _AUDIO_H
#define _AUDIO_H

typedef struct {

    // start of program
    int (*init)(void);
    // at end of program
    void (*deinit)(void);

    void (*start)(int sample_rate);
    // block of samples
    void (*play)(short buf[], int samples);
    void (*stop)(void);

    // may be NULL, in which case soft volume is applied
    void (*volume)(double vol);
} audio_output;

audio_output *audio_get_output();

#endif //_AUDIO_H
