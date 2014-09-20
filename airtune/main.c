#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <memory.h>
#include <openssl/md5.h>
#include <sys/wait.h>
#include "common.h"
#include "daemon.h"
#include "rtsp.h"
#include "mdns.h"
#include "metadata.h"
#include <stdio.h>

static void log_setup();
static int shutting_down = 0;

void app_shutdown(int retval)
{
    if (shutting_down)
        return;
    shutting_down = 1;
    printf("Shutting down...\n");
    mdns_unregister();
    rtsp_shutdown_stream();
    if (config.output)
        config.output->deinit();
    daemon_exit(); // This does nothing if not in daemon mode

    exit(retval);
}

static void sig_ignore(int foo, siginfo_t *bar, void *baz)
{
}
static void sig_shutdown(int foo, siginfo_t *bar, void *baz)
{
    app_shutdown(0);
}

static void sig_child(int foo, siginfo_t *bar, void *baz)
{
    pid_t pid;
    while ((pid = waitpid((pid_t) - 1, 0, WNOHANG)) > 0)
    {
        if (pid == mdns_pid && !shutting_down)
        {
            die("MDNS child process died unexpectedly!");
        }
    }
}

static void sig_logrotate(int foo, siginfo_t *bar, void *baz)
{
    log_setup();
}

void signal_setup(void)
{
    // mask off all signals before creating threads.
    // this way we control which thread gets which signals.
    // for now, we don't care which thread gets the following.
    sigset_t set;
    sigfillset(&set);
    sigdelset(&set, SIGINT);
    sigdelset(&set, SIGTERM);
    sigdelset(&set, SIGHUP);
    sigdelset(&set, SIGSTOP);
    sigdelset(&set, SIGCHLD);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    // setting this to SIG_IGN would prevent signalling any threads.
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &sig_ignore;
    sigaction(SIGUSR1, &sa, NULL);

    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sa.sa_sigaction = &sig_shutdown;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    sa.sa_sigaction = &sig_logrotate;
    sigaction(SIGHUP, &sa, NULL);

    sa.sa_sigaction = &sig_child;
    sigaction(SIGCHLD, &sa, NULL);
}

void log_setup()
{
    if (config.logfile)
    {
        int log_fd = open(config.logfile,
                          O_WRONLY | O_CREAT | O_APPEND,
                          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (log_fd < 0)
            die("Could not open logfile");

        dup2(log_fd, STDOUT_FILENO);
        setvbuf (stdout, NULL, _IOLBF, BUFSIZ);

        if (!config.errfile)
        {
            dup2(log_fd, STDERR_FILENO);
            setvbuf (stderr, NULL, _IOLBF, BUFSIZ);
        }
    }

    if (config.errfile)
    {
        int err_fd = open(config.errfile,
                          O_WRONLY | O_CREAT | O_APPEND,
                          S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (err_fd < 0)
            die("Could not open logfile");

        dup2(err_fd, STDERR_FILENO);
        setvbuf (stderr, NULL, _IOLBF, BUFSIZ);
    }
}

void init(char *name, char *pass, int port)
{
    signal_setup();
    memset(&config, 0, sizeof(config));

    // set defaults
    config.buffer_start_fill = 220;
    config.port = port;
    config.password = pass;
    config.apname = name;
    if (config.daemonise)
    {
        daemon_init();
    }

    log_setup();
    config.output = audio_get_output(config.output_name);
    config.output->init();

    uint8_t ap_md5[16];
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, config.apname, strlen(config.apname));
    MD5_Final(ap_md5, &ctx);
    memcpy(config.hw_addr, ap_md5, sizeof(config.hw_addr));
    if (config.meta_dir)
        metadata_open();

    rtsp_listen_loop();
}

int main()
{
    init("AirPort", NULL, 5555);
    app_shutdown(1);
    return 1;
}
