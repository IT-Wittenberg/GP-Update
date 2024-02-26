#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(void) {
    int gp_result = system("echo N | gpupdate /force");    
    return gp_result;
}
