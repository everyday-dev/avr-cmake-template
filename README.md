# avr-cmake-template

## Introduction
Template project for building AVR based firmware using VScode, CMake and the AVR toolchain. This project assumes you have a couple different tools installed.

## Tools Setup
You will need the avr-gcc toolchain, Make and CMake to compile the source and generate executable artifacts.
-   [avrdude](https://github.com/avrdudes/avrdude)
-   gcc-avr / avr 8-bit toolchain
    - Windows can install via the download from [Microchip](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers)
    - If you are in Ubuntu, you can use ```apt```
        ```shell
        sudo apt-get install gcc-avr avr-libc
        ```
-   [dfu-programmer](https://dfu-programmer.github.io/)
-   Make -  Make is shipped with Linux and should already be present on the developers system. If the developer is running Windows they can either use WSL to run a Linux instance or install Make on Windows via [MinGW](https://sourceforge.net/projects/mingw/)
-   [CMake](https://cmake.org/download/)

To flash and debug the target there are a couple different options. The first being to use an AtmelICE or avrispmkII programmer via ```avrdude```. The second option is to use a USB DFU firmware provided by Microchip and the ```dfu-programmer``` utility to flash and erase the device. The CMake project has ```make``` commands for both.

To write and execute unit tests on the firmware source, you will need to install the Ceedling unit test framework and the Python package gcovr to generate Cobertura html reports. Ceedling is a Ruby Gem, so Ruby must first be installed on your system. GCC and Make are also needed. If the developer is on Windows, you can find install instructions above.
-   [Ruby](https://www.ruby-lang.org)
    - Windows - [RubyInstaller 2.7.3-1](https://github.com/oneclick/rubyinstaller2/releases/download/RubyInstaller-2.7.3-1/rubyinstaller-2.7.3-1-x64.exe)
    - Linux - ```sudo apt-get install ruby```
-   [Ceedling](http://www.throwtheswitch.org/ceedling)
-   [Python](https://www.python.org/downloads/)
-   [gcovr](https://gcovr.com/en/stable/)
    - ```python -m pip install gcovr```

## Adding new source files
To add a new source file to the ```src/``` folder: From the root directory, use the Ceedling command below to create a new source ```.c``` and ```.h``` file. This will also create a unit test file in the ```test/``` folder for you.
```c
// Don't include a .c or .h in the filename
$ ceedling module:create[FILENAME]
```

### Compiling, Flashing & Testing
After a fresh clone, the ```setup``` scripts can be used to initialize the project.
#### Windows
```bash
setup.ps1
```
#### Linux
```bash
./setup.sh
```

#### ```make``` commands
Once a project has been initialized, the following commands should be ran from the ```build/``` folder

To clean the working directory:
```bash
make clean
```

To compile the source:
```bash
make -j8
```

To erase the connected target via ```avrdude```:
```bash
make erase
```

To flash the connected target via ```avrdude```:
```bash
make flash
```

To flash the connected target via ```dfu-programmer```:
```bash
make dfu
```

To program the fuses via ```avrdude```
```bash
make write_fuses
```

To read the fuses via ```avrdude```
```bash
make read_fuses
```

To execute unit tests:
```bash
make test
```
