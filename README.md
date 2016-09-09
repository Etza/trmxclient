# trmxclient
Video and image player made with OpenFrameworks for RPi2. All functions of trmxClient are addressable by OSC. 

The code was written by Angela Gabereau (Montréal) in 2015 for the interactive audiovisual installation "Transmestizx" by Daniela Moreno Wray (http://dmw.hotglue.me/?transmestiza) in Quito, Ecuador. 

The application runs on Raspberry Pi 2 with Archlinux and OpenFrameworks 0.9.3. Thanks to Arturo Castro from OpenFrameworks for support. Instructions for installation > http://openframeworks.cc/setup/raspberrypi/raspberry-pi-getting-started-archlinux/

We recommend using Berryboot to install the latest Archlinux on your SSD card http://www.berryterminal.com/doku.php/berryboot

Improvements needed in the code:
1. The media file system needs to recognise alphabetic or numerical order and not change all its media numbers when new media is added at a later letter or number.
2. Currently the internal alpha fade system does not permit rapid (less than 2 or 3 seconds) x-fades.
3. It would be good if Pause, instead of being a toggle, could be distinguished by something like pause 1 pause 0
4. Rename the layers in the order that they are on top of each other - from 1 (bottom) to 4 (top)



