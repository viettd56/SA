#include <avahi-client/client.h>
#include <avahi-client/publish.h>
#include <avahi-common/thread-watch.h>
#include <avahi-common/error.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>

#include "common.h"
#include "mdns.h"

int mdns_pid=0;
static AvahiClient *client = NULL;
static AvahiEntryGroup *group = NULL;
static AvahiThreadedPoll *tpoll = NULL;

static char *name = NULL;
static int port = 0;

static void egroup_callback(AvahiEntryGroup *g,
                            AvahiEntryGroupState state,
                            AVAHI_GCC_UNUSED void *userdata) {
    if (state==AVAHI_ENTRY_GROUP_COLLISION)
        die("service name already exists on network!");
    if (state==AVAHI_ENTRY_GROUP_FAILURE)
        die("avahi entry group failure!");
}
static void register_service(AvahiClient *c) {
    debug(1, "avahi: register_service\n");
    if (!group)
        group = avahi_entry_group_new(c, egroup_callback, NULL);
    if (!group)
        die("avahi_entry_group_new failed");

    if (!avahi_entry_group_is_empty(group))
        return;

    int ret;
    ret = avahi_entry_group_add_service(group,
                                        AVAHI_IF_UNSPEC,
                                        AVAHI_PROTO_UNSPEC,
                                        0,
                                        name,
                                        "_raop._tcp",
                                        NULL,
                                        NULL,
                                        port,
                                        MDNS_RECORD,
                                        NULL);
    if (ret < 0)
        die("avahi_entry_group_add_service failed");

    ret = avahi_entry_group_commit(group);
    if (ret < 0)
        die("avahi_entry_group_commit failed");
}

static void client_callback(AvahiClient *c,
                            AvahiClientState state,
                            AVAHI_GCC_UNUSED void * userdata) {
    switch (state) {
        case AVAHI_CLIENT_S_REGISTERING:
            if (group)
                avahi_entry_group_reset(group);
            break;

        case AVAHI_CLIENT_S_RUNNING:
            register_service(c);
            break;

        case AVAHI_CLIENT_FAILURE:
        case AVAHI_CLIENT_S_COLLISION:
            die("avahi client failure");

        case AVAHI_CLIENT_CONNECTING:
            break;
    }
}

static int avahi_register(char *srvname, int srvport) {
    debug(1, "avahi: avahi_register\n");
    name = strdup(srvname);
    port = srvport;

    int err;
    if (!(tpoll = avahi_threaded_poll_new())) {
        warn("couldn't create avahi threaded tpoll!");
        return -1;
    }
    if (!(client = avahi_client_new(avahi_threaded_poll_get(tpoll), 
                                    0,
                                    client_callback,
                                    NULL,
                                    &err))) {
        warn("couldn't create avahi client: %s!", avahi_strerror(err));
        return -1;
    }

    if (avahi_threaded_poll_start(tpoll) < 0) {
        warn("couldn't start avahi tpoll thread");
        return -1;
    }

    return 0;
}

void mdns_unregister(void) {
    if (tpoll)
        avahi_threaded_poll_stop(tpoll);
    tpoll = NULL;

    if (name)
        free(name);
    name = NULL;
}

void mdns_register(void){
	char* mdns_apname=malloc(strlen(config.apname)+14);
	char* p = mdns_apname;
	int i;
	for(i=0;i<6;i++){
		sprintf(p,"%02X",config.hw_addr[i]);
		p+=2;
	}
	*p++='@';
	strcpy(p,config.apname);
	int err = avahi_register(mdns_apname,config.port);
	if(err <0){
		die("cannot register mDNS");
	}
}

