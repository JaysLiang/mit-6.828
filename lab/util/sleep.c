#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("sleep seconds. need 1 params\n");
        exit(0); //must call exit
    }

    int second = atoi(argv[1]);
    int ticks = second * 10; // 1 second equal 100 ticks;
    sleep(ticks);
    exit(0); // must call exit
}
