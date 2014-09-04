#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include "common.h"
#include "audio.h"

static pa_simple *pa_dev = NULL;
static int pa_error;
static int init() {
    char *pa_server = NULL;
    char *pa_sink = NULL;
    char *pa_appname = config.apname;
    static const pa_sample_spec ss = {
            .format = PA_SAMPLE_S16LE,
            .rate = 44100,
            .channels = 2
    };

    pa_dev = pa_simple_new(pa_server,
            pa_appname,
            PA_STREAM_PLAYBACK,
            pa_sink,
            "OpenAir Stream",
            &ss, NULL, NULL,
            &pa_error);

    if (!pa_dev)
        die("Could not connect to pulseaudio server: %s", pa_strerror(pa_error));

    return 0;
}

static void deinit(void) {
    if (pa_dev)
        pa_simple_free(pa_dev);
    pa_dev = NULL;
}

static void start(int sample_rate) {
    if (sample_rate != 44100)
        die("unexpected sample rate!");
}

static void play(short buf[], int samples) {
    if( pa_simple_write(pa_dev, (char *)buf, (size_t)samples * 4, &pa_error) < 0 )
        fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(pa_error));
}

static void stop(void) {
    if (pa_simple_drain(pa_dev, &pa_error) < 0)
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(pa_error));
}

audio_output audio_pulse = {
    .init = &init,
    .deinit = &deinit,
    .start = &start,
    .stop = &stop,
    .play = &play,
    .volume = NULL
};
