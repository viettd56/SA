CFLAGS ?= -O2 -Wall -D_REENTRANT
LDFLAGS+= -lm -lpthread -lssl -lcrypto -lpulse-simple -lpulse -lavahi-common -lavahi-client  


SRCS := mdns.c common.c alac.c audio.c audio_pulse.c daemon.c metadata.c rtp.c rtsp.c player.c main.c
DEPS := common.h mdns.h alac.h audio.h daemon.h metadata.h rtp.h rtsp.h player.h


# default target
all: Test


%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) $<

Test.o: main.c
	$(CC) -c $(CFLAGS) main.c

OBJS := $(SRCS:.c=.o)
Test: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o Test

clean:
	rm -f Test
	rm -f $(OBJS)
