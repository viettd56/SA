#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <memory.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include "common.h"
#include "audio.h"

extern audio_output audio_pulse;

audio_output *audio_get_output() {
	return &audio_pulse;
}
