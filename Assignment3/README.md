Commands to run the project:
`make`
`sudo insmod devInput.ko`
`sudo insmod devOutput.ko`
`sudo ./test`
`sudo rmmod devOutput.ko`
`sudo rmmod devInput.ko`

The file `testDriver.c` is being used to test if the input and output device drivers are working properly. If you want to create your own testfile, name it as `testDriver.c` so it can work with the makefile and give it access to `/dev/devInput` and `/dev/devOutput`.