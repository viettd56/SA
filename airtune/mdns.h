#ifndef MDNS_H
#define MDNS_H

#define MDNS_RECORD "tp=UDP", "sm=false", "ek=1", "et=0,1", "cn=0,1", "ch=2", \
                "ss=16", "sr=44100", "vn=3", "txtvers=1", "da=true", "md=0,1,2", \
                config.password ? "pw=true" : "pw=false"

extern int mdns_pid;

void mdns_register(void);
void mdns_unregister(void);

#endif
