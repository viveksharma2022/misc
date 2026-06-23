#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {

    size_t len;
    int msg_len = 0;
    char buf[100];

    strncpy(buf, "Hello from the child process! \n", sizeof(buf) - 1);
    msg_len = strlen(buf);

    len = write(1, buf, msg_len);

    return 0;
}