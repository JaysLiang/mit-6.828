#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv) {
    int fds[2];
    char buf[2] = "c";
    int ret = pipe(fds);
    if ( ret != 0 ) {
        exit(ret);
    }
    ret = fork();
    if ( 0 == ret) {
        //child
        read(fds[1], buf, 1);
        int pid = getpid();
        printf("%d: received ping\n", pid);
        write(fds[0],buf, 1);
        close(fds[0]);
        close(fds[1]);
    } else if (ret >0) {
        //parent
        int pid = getpid();
        write(fds[1], buf, 1);
        sleep(1);
        read(fds[0], buf, 1);
        printf("%d: received pong\n", pid);
        close(fds[0]);
        close(fds[1]);
    } else {
        // panic fork failed;
        printf("panic fork failed\n");
    }

    exit(0);
}