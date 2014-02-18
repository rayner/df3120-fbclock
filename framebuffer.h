

/* Struct of framebuffer attributes */
struct framebuffer {
    int descriptor; /* File descriptor for framebuffer device */
    unsigned char *mem_start; /* Pointer to start of framebuffer memory */
    struct fb_var_screeninfo screeninfo; /* Pointer to struct of screen info */
};


void new_framebuffer(struct framebuffer *fb, char *filename);
void close_framebuffer(struct framebuffer *fb);
size_t screen_size_in_bytes(struct framebuffer *fb);
struct image_size display_png(struct framebuffer *fb, char *filename, 
                              int x_pos, int y_pos);

