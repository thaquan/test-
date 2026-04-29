#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    struct procinfo info;
    int target_pid;

    if (argc < 2) {
        target_pid = getpid(); // Mặc định lấy tiến trình hiện tại
    } else {
        target_pid = atoi(argv[1]);
    }

    if(procinfo(target_pid, &info) == 0){
        printf("Process: %s\n", info.name);
        printf("PID: %d, PPID: %d\n", info.pid, info.ppid);
        printf("State: %d\n", info.state);
        printf("Memory: %d bytes\n", (int)info.sz);
    } else {
        printf("Error: Process not found\n");
    }
    exit(0);
}
