#include "signal.h"
#include "time.h"

#include "config.h"
#include "vec3.h"
#include "mat3.h"
#include "renderer.h"

/*
        box_offset.x = sin(iTime * 0.0000025);
        box_offset.z = 3 + cos(iTime * 0.0000025);
        box_dimensions.x = cos(iTime * 0.000005) * .5 + 1;
*/

void interrupt_handler(int parm){
    // Reset terminal
    printf(ANSI_CLEAR_SCREEN);
    printf(ANSI_SHOW_CURSOR);
    exit(0);
}

int main(int argc, char *argv[]) {
    // Handle Interrupt Signal
    void (*int_handler)(int);
    int_handler = signal (SIGINT, interrupt_handler);

    // Prepare the terminal for drawing
    printf(ANSI_HIDE_CURSOR);
    printf(ANSI_CLEAR_SCREEN);


    double start_stamp = (double)clock();
    int c = 0;
    while (1){
        iTime = (double)clock() - start_stamp;


        /* To control the global sdf function see config.h */
        render(&global_sdf_function);


        printf(ANSI_RESET_COLOR);
        printf("Counter - %d;\n", c);
        printf("Running for - %fsec;\n", iTime/CLOCKS_PER_SEC);
        printf(fmod(iTime, 2.0) > 1 ? "Press Ctrl + C to stop..." : "");  // Blinking instruction

        c++;
        printf(ANSI_RETURN); // Move cursor back to start
    }

    return 0;
}

