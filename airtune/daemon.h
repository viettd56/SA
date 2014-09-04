#ifndef DAEMON_H
#define DAEMON_H

#include <stdarg.h>

void daemon_init();
void daemon_ready();
void daemon_fail(const char *format, va_list arg);
void daemon_exit();

#endif
