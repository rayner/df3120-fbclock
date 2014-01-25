
/* Seconds to sleep between updates */
#define SLEEP	2

#define FILENAME_MAXLEN 10

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

void display_time(struct tm *tp);
void display_png(char *filename, int x_pos, int y_pos);

