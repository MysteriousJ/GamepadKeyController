Gamepad Key Controller allows you to map usb game controller inputs to keyboard and mouse inputs if a game you want to play does not have native support for your controller. I designed it to be very fast to set up. It supports up to 16 buttons and six axes of analog input, and can save and load configuration files.

I wrote this program along with reading Programming Windows (Petzold 98) for practice. Though other controller-to-keyboard mapping programs work better and have many more features, you may find this program interesting if you're learning how to use the win32 api.

##Dependencies
SDL2 for joystick input

##Building
Open the visual studio sln file in the vc directory (This guide is written for Visual Studio Community Edition, 2015).
Go to settings in the menu and select GamepadKeyController Properties.
Select General under Configuration Properties > C/C++. Add the path to your SDL include files to Additional Include Directories.
In General, under Configuration Properties > Linker, add the directory with your SLD .lib files.
Click OK to Save and close the Properties dialog.
Select Build > Build Solution (or just press F7).
The .exe will be in the Debug folder in the vc directory. Copy SDL2.dll to that folder (or wherever you put the exe), and you should be able to run the program.

##License
MIT
