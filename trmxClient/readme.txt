 
_Transmestizx OSC Communicator Client_

Angela Gabereau // 2015
for the installation Transmestizx (http://dmw.hotglue.me/?transmestiza)


__Summary

	This application runs on the RPi2 and receives commands from the Server computer running Pure Data or other OSC enabled control app.
	
__OSC Commands

see full list in https://github.com/pedruski/trmxclient/documentation


__Installation

	This is an Open Frameworks app (http://openframeworks.cc/).  
	
	1.Install Open Frameworks in the RPi2
	
	http://forum.openframeworks.cc/t/raspberry­pi­2­setup­guide/18690 
	add this to /home/pi/.config for correct makes: export MAKEFLAGS=­j4 PLATFORM_VARIANT=rpi2 
	
	2. Navigate to the trmxClient directory. 
	
	In config.make, set the path to your installation of OpenFrameworks, on line number 11, eg:
		OF_ROOT=/home/pi/openFrameworks

	3. Set the configuration variables in /bin/data/settings.xml:

	3.1. Set the IP address of the server, which will recieve the messages sent from this client.  On the server, open a terminal and execute the following command:
		hostname -i
	Write down the ip address, and add it:
		<serverIpAddress>192.168.2.2</serverIpAddress>
	You'll have to add the client destination ip address to the server app too.

	3.2. Set the send and recieve ports.
	    <sendPort>9000</sendPort>
    	<recvPort>9001</recvPort>

	3.3. Set the directory that contains the media files.  Could be on HD or USB.

		<mediaDirPath>/home/media<mediaDirPath>

	The remaining settings are optional.

	Now the configuration is complete.  Time to make and run the app.

	4. Open a terminal and navigate to this app's directory.
	
	por ejemplo: cd /git/transmestizx/Sketches/trmxClient 

	Type in the following command, and press enter:
		make
	If there are no errors, type the following command and press enter:
		make run

	Now the app should be running!

