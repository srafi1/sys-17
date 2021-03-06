#include "pipe_networking.h"
#include <signal.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

int main() {
    signal(SIGINT, sighandler);
    while (1) {
        int well_known = server_setup();
        if (!fork()) {
            subserver(well_known);
        }
    }
}

void subserver(int from_client) {
    printf("I am the subserver\n");
    int to_client = server_connect(from_client);
    char buffer[BUFFER_SIZE];
    while (1) {
        //char buffer[HANDSHAKE_BUFFER_SIZE];
        read(from_client, buffer, BUFFER_SIZE);
        process(buffer);
        write(to_client, buffer, BUFFER_SIZE);
    }
}

void process(char * s) {
    int i;
    for (i = 0; i < strlen(s); i++) {
        if (s[i] != 0) {
            s[i] = toupper(s[i]);
        }
    }
}
