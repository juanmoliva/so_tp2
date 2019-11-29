doit: 
	cd Toolchain; make all; cd ../; make all

all: bootloader kernel userland image

free-list: free-list bootloader kernel userland image

buddy: buddy-prepare bootloader kernel userland image

free-list:
	./prepare.sh

buddy-prepare:
	./prepare.sh buddy

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

.PHONY: bootloader image collections kernel userland all clean doit
