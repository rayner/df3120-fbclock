
/* Seconds to sleep between updates */
#define SLEEP	2

/* Filenames of clock graphics */
char *digit_filenames[] = {
    "0.png",
    "1.png",
    "2.png",
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

/* Function prototypes */
void display_time(struct tm *tp, char *fb, int x_offset, int y_offset);
void display_png(char *filename, int x_pos, int y_pos);
int open_fb();
char *get_framebuffer(int fb_descriptor);
size_t screen_size_in_bytes(int fb_descriptor);
void close_framebuffer(char *fb, int fb_descriptor);
