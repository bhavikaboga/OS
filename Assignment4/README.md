Commands to run the project: <br>
`make` <br>
`sudo insmod devInput.ko` <br>
`sudo insmod devOutput.ko` <br>
`sudo ./test` <br>
`sudo rmmod devOutput.ko` <br>
`sudo rmmod devInput.ko` <br>

The file `testDriver.c` is being used to test if the input and output device drivers are working properly. If you want to create your own testfile, name it as `testDriver.c` so it can work with the makefile and give it access to `/dev/devInput` and `/dev/devOutput`.
