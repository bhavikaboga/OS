Commands to run project: <br>
`make` <br>
`sudo insmod charDriver.ko` <br>
`sudo ./test` <br>
`sudo rmmod charDriver.ko` <br>

The file `testDriver.c` is being used to test if the device driver is working properly. If you want to create your own testfile, name it as `testDriver.c` so it can work with the makefile and give it access to `/dev/devChar`. 


To test the echoed input:
`make`<br>
`sudo insmod charDriver.ko`<br>
`sudo -i`<br>
`echo "test input" > /dev/charDriver`<br>
`cat /dev/charDriver`
