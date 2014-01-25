/* fbclock: framebuffer clock for Linux on the Parrot DF3120 digital
 * photo frame.
 *
 * Requires: libpng
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <png.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "fbclock.h"

/* File descriptor for framebuffer device */
int fb_descriptor;

/* Framebuffer memory */
char *fb = NULL;


int main(int argc, char *argv[]) {
    time_t t;
    struct tm *tp;

    /* Display offset from top left of screen, in pixels */
    /* TODO: turn into command-line option */
/*    int x_offset = 0;
    int y_offset = 0; */

    fb = get_framebuffer();

    while (1) {
        /* Get current time */
        t = time(NULL);
        tp = localtime(&t);

        /* Display it */
        display_time(tp);

        /* Wait for next update */
        sleep(SLEEP);
    }

    /* Clean up */
    close_framebuffer();

    return 0;
}


/* display_time: Display time on framebuffer.
 * tp: pointer to tm struct such as that returned by localtime().
 * fb: pointer to framebuffer memory.
 */
void display_time(struct tm *tp) {
    int x_pos = 0;
    int y_pos = 0;

    int hours = tp->tm_hour;
    int minutes = tp->tm_min;
    int mday = tp->tm_mday;
    int month_num = tp->tm_mon;
    int year = tp->tm_year + 1900;

    /* TODO: allow user-specified display formats */
    display_png(digit_filenames[mday/10], x_pos, y_pos);
    display_png(digit_filenames[mday%10], x_pos, y_pos);

    display_png(short_month_filenames[month_num], x_pos, y_pos);

    display_png(digit_filenames[year/1000], x_pos, y_pos);
    display_png(digit_filenames[(year%1000)/100], x_pos, y_pos);
    display_png(digit_filenames[(year%100)/10], x_pos, y_pos);
    display_png(digit_filenames[year%10], x_pos, y_pos);

    display_png(digit_filenames[hours/10], x_pos, y_pos);
    display_png(digit_filenames[hours%10], x_pos, y_pos);
    display_png(digit_filenames[minutes/10], x_pos, y_pos);
    display_png(digit_filenames[minutes%10], x_pos, y_pos);

    printf("Time: %s\n", asctime(tp));
    fflush(NULL);

    return;
}


/* display_png: display a PNG file at the given location on the framebuffer.
 * filename: name of the file to display.
 * x_pos, y_pos: x and y coordinates at which to display it on the framebuffer.
 */
void display_png(char *filename, int x_pos, int y_pos) {
    printf("Displaying: %s\n", filename);
    fflush(NULL);
    return;
}


/* Set up framebuffer for use.
 * Returns: pointer to framebuffer memory.
 */
char *get_framebuffer() {
    size_t bytes;

    fb_descriptor = open("/dev/fb0", O_RDWR);
    if (fb_descriptor == -1) {
        fprintf(stderr, "Could not open /dev/fb0\n");
        exit(EXIT_FAILURE);
    }

    bytes = screen_size_in_bytes(fb_descriptor);

    fb = (char *)mmap(NULL, bytes, PROT_READ|PROT_WRITE, MAP_SHARED,
                      fb_descriptor, 0);

    if ((int)*fb == -1) {
        fprintf(stderr, "Failed to mmap /dev/fb0\n");
        exit(EXIT_FAILURE);
    }

    return fb;
}


/* Clean up after we're finished with the framebuffer. Unmap memory
 * and close file descriptor.
 */
void close_framebuffer() {
    munmap(fb, screen_size_in_bytes(fb_descriptor));
    close(fb_descriptor);
    return;
}


/* Get the size of the screen in bytes.
 * fb_descriptor: file descriptor of the framebuffer device.
 */
size_t screen_size_in_bytes() {
    size_t bytes;
    struct fb_var_screeninfo var_screeninfo;

    if (ioctl(fb_descriptor, FBIOGET_VSCREENINFO, &var_screeninfo)) {
        fprintf(stderr, "Unable to get var screen info\n");
        exit(EXIT_FAILURE);
    }

    bytes = var_screeninfo.xres
            * var_screeninfo.yres
            * var_screeninfo.bits_per_pixel/8;

    /* XXX debug output */
    printf("x resolution: %d\n", var_screeninfo.xres);
    printf("y resolution: %d\n", var_screeninfo.yres);
    printf("bpp: %d\n", var_screeninfo.bits_per_pixel);
    printf("Screen size: %d\n", (int)bytes);

    return bytes;
}

