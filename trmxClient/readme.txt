 
_Transmestizx OSC Communicator Client_

Angela Gabereau // 2015




__Summary

This is the app that sites on the RPi2 and receives commands from the Server computer running Pure Data.

Application configuration is loaded via an xml file.  Configuration is specific to each installation.



__GUI Commands

	'f': Toggle Fullscreen
	's': Send Ping
	'p': Toggle pingFinity
	'd': Send directory list to  ...........

	___pingFinity - ping 4 ever.  

		It this option is enabled on both client and server, the two will exchange pings infinitly.
		Client sends ping, Server recieves ping, waits 3 seconds.  Server sends ping to client.  Client recieves ping, waits 3 seconds. Repeat.
		Note:  This mode with fuck up all other processing.  TO be used to troubleshoot OSC connectivity. 




__OSC Commands

	___Request Directory Listing

		address: /request_dir_list
		action: Send contents of directory to server
		response address: /receive_dir_list
		message format: [index number1]=[filename 1]$[index number2]=[filename 2]$...$[index numbern]=[filename n]$#
		example message: 1=c06h10f10.jpg$2=c06h30f10.jpg$3=c06h30f11.jpg$4=c06h35f02.jpg$5=c06h35f07.jpg$6=c06h35f11.jpg$7=c06h36f10.jpg$8=me_1621986_orange-cloudy (1).mov$9=Gao Bagua (1).mp4$10=fingers.mov$#


	___Ping

		address: /ping
		action: If pingFinity is enabled, send ping to server
		response address: /ping
		message format: ping




__Installation

	This is an Open Frameworks app (http://openframeworks.cc/).  Install Open Frameworks and run a couple examples to get warmed up.

	To run this app, some configuration is necessary

	~~ Compile vars - Set for compilation.

	1. Create a new file called config.make.  Use config.make.default as a template; copy and paste its contents into config.make

	2. In config.make, set the path to your installation of OpenFrameworks, on line number 11, eg:
		OF_ROOT=/home/pi/openFrameworks

	~~ Config vars - Set for runtime.

	3. Set the configuration variables.  Create a new file  in /bin/data called settings.xml.   Use settings.xml.default as a template; copy and paste its contents into settings.xml.

	4. Set the IP address of the server, which will recieve the messages sent from this client.  On the server, open a terminal and execute the following command:
		hostname -i
	Write down the ip address, and add it:
		<serverIpAddress>192.168.2.2</serverIpAddress>
	You'll have to add the client destination ip address to the server app too.

	5. Set the send and recieve ports.
	    <sendPort>9000</sendPort>
    	<recvPort>9001</recvPort>
	You'll have to add the client ports to the server app too. They will be the opposite. send->recv, recv->send.

	6. Set the directory that contains the media files.  Could be on HD or USB.

		<mediaDirPath>/run/media/angela/CHI_2014/transmestizx<mediaDirPath>

	The remaining settings are optional.

	Now the configuration is complete.  Time to make and run the app.

	Open a terminal and navigate to this app's directory.
	Type in the following command, and press enter:
		make
	If there are no errors, type the following command and press enter:
		make run

	Now the app should be running!

