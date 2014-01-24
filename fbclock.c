/* fbclock: framebuffer clock for Linux on the Parrot DF3120 digital
 * photo frame.
 *
 * Requires: libpng
 */

#include <stdio.h>

#include <unistd.h>
#include <time.h>
#include <png.h>

/* Seconds to sleep between updates */
#define SLEEP	5

int main() {
    time_t t;
    struct tm *tp;

    while (1) {
        /* Get current time */
        t = time(NULL);
        tp = localtime(&t);

        /* Display it */
        printf("Time: %s\n", asctime(tp));
        fflush(NULL);

        /* Wait for next update */
        sleep(SLEEP);
    }
    return 0;
}
