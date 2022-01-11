# trmxClient
Video and image player made with OpenFrameworks for Raspberry Pi 2. All functions of trmxClient are addressable by OSC. 

The code was written by Angela Gabereau (Montréal) in 2015 for the interactive audiovisual installation "Transmestizx" by Daniela Moreno Wray (http://dmw.hotglue.me/?transmestiza) in Quito, Ecuador. Updated for full GLES2 support by Olivia Jackson (Bogotá) in 2016 while on show in Mapa Teatro http://www.midbo.co/movie/transmestizx/ . Pure Data control patches and implementation in RPi2 by Pedro Soler.

The application runs on Raspberry Pi 2 with Archlinux (Gstreamer 1.8.3-1 and gst-omx 1.2.0-1) and OpenFrameworks 0.9.4. Thanks to Arturo Castro from OpenFrameworks for support. Instructions for installation > http://openframeworks.cc/setup/raspberrypi/raspberry-pi-getting-started-archlinux/

Improvements needed in the code:
1. The media file system needs to recognise alphabetic or numerical order and not change all its media numbers when new media is added at a later letter or number.
2. Currently the internal alpha fade system does not permit rapid (less than 2 or 3 seconds) x-fades.
3. It would be good if Pause, instead of being a toggle, could be distinguished by something like pause 1 pause 0
4. Rename the layers (mediaframes) in the order that they are on top of each other - from 1 (bottom) to 4 (top) - or add a command permitting the assignation of a stack number to a mediaframe.

Other tasks pending: Improve documentation, upload the precompiled disk image for Rpi.

Please feel free to fork, use, adapt.
