
/* Seconds to sleep between updates */
#define SLEEP	2

/* Filenames of clock graphics */
char *digit_filenames[] = {
    "graphics/0.png",
    "graphics/1.png",
    "graphics/2.png",
    "3.png",
    "4.png",
    "5.png",
    "6.png",
    "7.png",
    "8.png",
    "9.png"
};

char *short_month_filenames[] = {
    "jan.png",
    "feb.png",
    "mar.png",
    "apr.png",
    "may.png",
    "jun.png",
    "jul.png",
    "aug.png",
    "sep.png",
    "oct.png",
    "nov.png",
    "dec.png"
};

/* Data types */
/* x,y dimensions of an image */
struct image_size {
    int x;
    int y;
};

/* Function prototypes */
void display_time(struct tm *tp, char *fb, int x_offset, int y_offset);
struct image_size display_png(char *filename, char *fb, int x_pos, int y_pos);
int open_fb();
char *get_framebuffer(int fb_descriptor);
size_t screen_size_in_bytes(int fb_descriptor);
void close_framebuffer(char *fb, int fb_descriptor);
