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
#include <stdint.h>
#include <png.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "fbclock.h"



int main(int argc, char *argv[]) {
    time_t t;
    struct tm *tp;
    int fb_descriptor;
    char *fb;

    /* Display offset from top left of screen, in pixels */
    /* TODO: turn into command-line option */
    int x_offset = 0;
    int y_offset = 0;

    /* Set up framebuffer */
    fb_descriptor = open_fb();
    fb = get_framebuffer(fb_descriptor);

    while (1) {
        /* Get current time */
        t = time(NULL);
        tp = localtime(&t);

        /* Display it */
        display_time(tp, fb, fb_descriptor, x_offset, y_offset);

        /* Wait for next update */
        sleep(SLEEP);
    }

    /* Clean up */
    close_framebuffer(fb, fb_descriptor);

    return EXIT_SUCCESS;
}


/* display_time: Display time on framebuffer.
 * tp: pointer to tm struct such as that returned by localtime().
 * fb: pointer to framebuffer memory.
 * x_offset, y_offset: x and y position in pixels
 */
void display_time(struct tm *tp, char *fb, int fb_descriptor, int x_offset, int y_offset) {
    int x_pos = x_offset;
    int y_pos = y_offset;

    int hours = tp->tm_hour;
    int minutes = tp->tm_min;
    int mday = tp->tm_mday;
    int month_num = tp->tm_mon;
    int year = tp->tm_year + 1900;

    struct image_size png_size;

    /* TODO: allow user-specified display formats */
    png_size = display_png(digit_filenames[mday/10], fb, fb_descriptor, x_pos, y_pos);
    x_pos += png_size.x;
    png_size = display_png(digit_filenames[mday%10], fb, fb_descriptor, x_pos, y_pos);
    x_pos += png_size.x;

    png_size = display_png(short_month_filenames[month_num], fb, fb_descriptor, x_pos, y_pos);
    x_pos += png_size.x;

    png_size = display_png(digit_filenames[year/1000], fb, fb_descriptor, x_pos, y_pos);
    x_pos += png_size.x;
    png_size = display_png(digit_filenames[(year%1000)/100], fb, fb_descriptor, x_pos, y_pos);
    x_pos += png_size.x;
    png_size = display_png(digit_filenames[(year%100)/10], fb, fb_descriptor, x_pos, y_pos);
    x_pos += png_size.x;
    png_size = display_png(digit_filenames[year%10], fb, fb_descriptor, x_pos, y_pos);
    x_pos += png_size.x;

    png_size = display_png(digit_filenames[hours/10], fb, fb_descriptor, x_pos, y_pos);
    x_pos += png_size.x;
    png_size = display_png(digit_filenames[hours%10], fb, fb_descriptor, x_pos, y_pos);
    x_pos += png_size.x;
    png_size = display_png(digit_filenames[minutes/10], fb, fb_descriptor, x_pos, y_pos);
    x_pos += png_size.x;
    png_size = display_png(digit_filenames[minutes%10], fb, fb_descriptor, x_pos, y_pos);

    printf("Time: %s\n", asctime(tp));
    fflush(NULL);

    return;
}


/* display_png: display a PNG file at the given location on the framebuffer.
 * filename: name of the file to display.
 * fb: pointer to framebuffer memory.
 * x_pos, y_pos: x and y coordinates at which to display it on the framebuffer.
 */
struct image_size display_png(char *filename, char *fb, int fb_descriptor, int x_pos, int y_pos) {
    png_byte header[8];
    FILE *fp;
    png_structp png_ptr;
    png_infop info_ptr;
    png_byte color_type;
    png_byte bit_depth;
    struct image_size png_size;
    int x, y;
    png_bytepp row_pointers;
    struct fb_var_screeninfo var_screeninfo;
    

    printf("Displaying: %s\n", filename);
    fflush(NULL);

    fp = fopen(filename, "rb");
    if (!fp) {
        printf("Failed to open %s\n", filename);
        /* XXX */
    }

    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)) {
        printf("%s is not a PNG file.\n", filename);
        /* XXX */
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);
    if (!png_ptr || !info_ptr) {
        printf("failed to create png structs.\n");
        /* XXX */
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        printf("setjmp_failed.\n");
        /* XXX */
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    png_size.x = png_get_image_width(png_ptr, info_ptr);
    png_size.y = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    printf("%s: x: %d, y: %d, color type: %d, bit depth: %d\n", filename, png_size.x, png_size.y, color_type, bit_depth);

    row_pointers = png_get_rows(png_ptr, info_ptr);

    if (ioctl(fb_descriptor, FBIOGET_VSCREENINFO, &var_screeninfo)) {
        perror("Unable to get var screen info");
        exit(EXIT_FAILURE);
    }

    /******** Process and display the image data ********/
    /* XXX TODO: support fb colour depths other than 16-bit */
    /* Move to correct X/Y starting position */
    fb += x_pos * (var_screeninfo.bits_per_pixel / 8);
    fb += y_pos * (var_screeninfo.bits_per_pixel / 8) * var_screeninfo.xres;
    /* Loop through image data and write each pixel to the framebuffer */
    if (var_screeninfo.bits_per_pixel == 16) {
        for (y = 0; y < png_size.y; y++) {
            png_bytep row = row_pointers[y];
            for (x = 0; x < png_size.x; x++) {
                png_bytep png_pixel = &(row[x*4]);
                /*printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n", x, y, png_pixel[0], png_pixel[1], png_pixel[2], png_pixel[3]);*/
                /* Convert 8888 RGBA to 565 RGB */
                png_byte r = png_pixel[0];
                png_byte g = png_pixel[1];
                png_byte b = png_pixel[2];
                uint16_t fb_pixel = 0;

                r = r >> 3;
                g = g >> 2;
                b = b >> 3;

                fb_pixel = fb_pixel | r;
                fb_pixel = fb_pixel << 6;
                fb_pixel = fb_pixel | g;
                fb_pixel = fb_pixel << 5;
                fb_pixel = fb_pixel | b;

                /* Write pixel to framebuffer */
                /* XXX */
                *fb = fb_pixel;
                fb++;
                *fb = (fb_pixel >> 8);
                fb++;
            }
            fb -= png_size.x * (var_screeninfo.bits_per_pixel / 8); /* Move back to starting X coordinate */
            fb += (var_screeninfo.bits_per_pixel / 8) * var_screeninfo.xres; /* Move down to the next row of pixels */
        }
    }

    return png_size;
}


/* Open framebuffer device.
 * Returns: framebuffer file descriptor.
 */
int open_fb() {
    int fb_descriptor;
    fb_descriptor = open("/dev/fb0", O_RDWR);
    if (fb_descriptor == -1) {
        perror("Could not open /dev/fb0");
        exit(EXIT_FAILURE);
    }
    return fb_descriptor;
}


/* Set up framebuffer for use.
 * fb_descriptor: file descriptor of framebuffer device, from open_fb().
 * Returns: pointer to framebuffer memory.
 */
char *get_framebuffer(int fb_descriptor) {
    size_t bytes;
    char *fb;

    bytes = screen_size_in_bytes(fb_descriptor);

    fb = (char *)mmap(NULL, bytes, PROT_READ|PROT_WRITE, MAP_SHARED,
                      fb_descriptor, 0);

    if ((int)*fb == -1) {
        perror("Failed to mmap /dev/fb0");
        exit(EXIT_FAILURE);
    }

    return fb;
}


/* Clean up after we're finished with the framebuffer. Unmap memory
 * and close file descriptor.
 * fb: pointer to memory that was mmap'ed to the framebuffer device.
 * fb_descriptor: file descriptor for the framebuffer device.
 */
void close_framebuffer(char *fb, int fb_descriptor) {
    if (munmap(fb, screen_size_in_bytes(fb_descriptor)) == -1) {
        perror("Failed to munmap framebuffer memory");
        exit(EXIT_FAILURE);
    }
    if (close(fb_descriptor) == -1) {
        perror("Failed to close framebuffer");
        exit(EXIT_FAILURE);
    }
    return;
}


/* Get the size of the screen in bytes.
 * fb_descriptor: file descriptor of the framebuffer device.
 * Returns the number of bytes used for the screen.
 */
size_t screen_size_in_bytes(int fb_descriptor) {
    size_t bytes;
    struct fb_var_screeninfo var_screeninfo;

    if (ioctl(fb_descriptor, FBIOGET_VSCREENINFO, &var_screeninfo)) {
        perror("Unable to get var screen info");
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

