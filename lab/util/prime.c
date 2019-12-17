#include "kernel/types.h"
#include "user/user.h"

union buffer{
    int i;
    char buf[4];
};

int main(int argc, char **argv) {
    int fds[2];
    int ret = pipe(fds);
    if (ret != 0 ) {
        exit(ret);
    }

    int pid = fork();
    if (pid == 0) {
        //father
        close(fds[0]);
        int i;
        for ( i= 2; i <=35 ; i++) {
            union buffer buf;
            buf.i = i;
            if (write(fds[1], buf.buf, sizeof(char) * 4)!=4) {
                printf("write error\n");
                exit(0);
            }

        }
        close(fds[1]);
    } else if (pid > 0) {
        //child
        close(fds[1]);
        int first_in = 1;
        int first_out = 1;
        int prime = 0;
        int sieve_fds[2];
        while(1) {
            union buffer buf;

            int retNum = read(fds[0], buf.buf, sizeof(char)* 4);
            if (retNum == 0) {
                close(fds[0]);
                exit(0);
            }
            if (retNum != 4) {
                printf("read pipe error, read num char :%d\n", retNum);
                close(fds[0]);
                exit(0);
            }
            if (first_in) {
                prime = buf.i;
                printf("prime %d", prime);
                first_in =0;
                continue;
            }
            if (first_out) {
                int ret = pipe(sieve_fds);
                if (ret != 0 ) {
                    close(fds[0]);
                    exit(ret);
                }
                close(sieve_fds[0]);
                int new_pid = fork();
                if (new_pid > 0) {
                    //child
                    close(fds[0]);
                    fds[0] = sieve_fds[0];
                    fds[1] = sieve_fds[1];
                    first_in = 0;
                    continue;
                } else if (new_pid <0 ) {
                    close(fds[0]);
                    close(sieve_fds[1]);
                    printf("fork error\n");
                    exit(0);
                }
                first_out= 0;
            }
            if (buf.i % prime != 0) {
                if(write(sieve_fds[1], buf.buf, sizeof(char)*  4) != 4) {
                    printf("write error\n");
                    close(fds[0]);
                    close(sieve_fds[1]);
                    exit(0);
                };
            }
        }
    } else {
        printf("fork error\n");
        exit(0);
    }

    exit(0);
}