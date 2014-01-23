df3120-fbclock
==============

Simple framebuffer clock application for Linux on the Parrot DF3120 digital photo frame.

First you'll need to get Linux installed on the DF3120. There are instructions at https://sites.google.com/site/repurposelinux/df3120. For the filesystem image, I used the version downloadable from http://www.martinhubacek.cz/arm/df3120-linux-picture-frame, which has a working chmod command.

Next step is to get a working toolchain for cross-compiling. You can do this by following the minifs build instructions here: https://sites.google.com/site/repurposelinux/df3120/install_minifs/

To get files onto the DF3120, you can connect it to your machine using a USB cable and use wget or tftp. Alternatively, you could stick the SD card into a card reader and drop files directly onto it that way.
