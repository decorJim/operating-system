// initlab
#include <stdlib.h>
#include <time.h>

#include "libprocesslab.h"
#include "q1/libq1.h"
#include "part2.h"

int main(int argc, char** argv) {
    srand(time(NULL));
    if (argc == 2) {
        int questionNum = atoi(argv[1]);
        switch (questionNum) {
            case 1:
                startQuestion(evaluateQuestion1, 1, 1);
                break;
            case 2:
                startQuestion(part2, 2, 1);
                break;
            default:
                break;
        }
        exit(0);
    }
    initLab();
    startQuestion(evaluateQuestion1, 1, 0);
    startQuestion(part2, 2, 0);
}