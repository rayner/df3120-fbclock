
/* Seconds to sleep between updates */
#define SLEEP	2

/* Filenames of clock graphics */
char *digit_filenames[] = {
    "graphics/0.png",
    "graphics/1.png",
    "graphics/2.png",
    "graphics/3.png",
    "graphics/4.png",
    "graphics/5.png",
    "graphics/6.png",
    "graphics/7.png",
    "graphics/8.png",
    "graphics/9.png"
};

char *short_month_filenames[] = {
    "graphics/jan.png",
    "graphics/feb.png",
    "graphics/mar.png",
    "graphics/apr.png",
    "graphics/may.png",
    "graphics/jun.png",
    "graphics/jul.png",
    "graphics/aug.png",
    "graphics/sep.png",
    "graphics/oct.png",
    "graphics/nov.png",
    "graphics/dec.png"
};


/* Function prototypes */
void display_time(struct tm *tp, struct framebuffer *fb, int x_offset, 
                  int y_offset);
