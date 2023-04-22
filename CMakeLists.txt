# Set min req version of Cmake
cmake_minimum_required(VERSION 3.13)

#####################################################################################
# Set our project and CPU specific flags, options, definitions and Linker Settings
#####################################################################################
# Set project name w/ no spaces
set(PRODUCT_NAME "NEW_PROJECT")

# Target MCU
set(MCU atmega32u4)

# Flash Progammer type
set(PROG_TYPE atmelice_isp)

# Add our MCU compiler options
set(COMPILE_OPTIONS
    -mmcu=${MCU} # MCU
    -std=gnu99 # C99 standard
    -Os # optimize
    -Wall # enable warnings
    -Wno-main
    -Wundef
    -Wstrict-prototypes
    -Werror
    -Wfatal-errors
    -Wl,--relax,--gc-sections
    -g
    -gdwarf-2
    -funsigned-char # a few optimizations
    -funsigned-bitfields
    -fpack-struct
    -fshort-enums
    -ffunction-sections
    -fdata-sections
    -fno-split-wide-types
    -fno-tree-scev-cprop
    -DF_CPU=16000000UL
    -DF_USB=8000000UL
)

# Set the target linker flags
set(LINKER_FLAGS "-mmcu=${MCU} -Wl,--gc-sections")

# Set any libraries you would need to link against (.a libs, gcc, c, m, nosys as examples)
# NOT TO BE CONFUSED WITH LINKER FLAGS. FLAGS BELONG IN the flags.cmake file
set(LINKER_STATIC_LIBRARIES
)

# Set your desired fuse values for writing
set(LOW_FUSE_VAL 0xCE)
set(HIGH_FUSE_VAL 0x99)
set(EXTENDED_FUSE_VAL 0xC3)

# Create a list of our APP source
set(APP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)

# Set all of our application and SDK include paths
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

# Add any subdirectories with CMake projects that should be added
set(CMAKE_SUBDIRS
    # ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glasslabs/DAC757x
)

# If you are going to compile Vendor source directly into yoru application
# add it here.
# Create a list of our APP source
set(VENDOR_SRC
    # ${CMAKE_CURRENT_SOURCE_DIR}/vendor/example/ex.c
)

#####################################################################################
# End of project and CPU specific items - DO NOT EDIT ANYTING BELOW THIS POINT
#####################################################################################

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

# Set the project and executable name
project(${PRODUCT_NAME} VERSION 1.0 LANGUAGES C)
set(EXECUTABLE_NAME ${PRODUCT_NAME})
# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_EXE_LINKER_FLAGS ${LINKER_FLAGS})

# Include the directories into CMake
include_directories(${INCLUDES})

# Link against our static libraries
link_libraries(${LINKER_STATIC_LIBRARIES})

add_compile_options(${COMPILE_OPTIONS})

# Create our executable
add_executable(${PRODUCT_NAME} ${APP_SRC} ${VENDOR_SRC}
)

# Include all of the vendor CMake subdirs
foreach(subdir ${CMAKE_SUBDIRS})
    add_subdirectory(${subdir})
endforeach()

# Extract the current firmware version
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h ver)
# Major
string(REGEX MATCH "VERSION_MAJOR([ \t]+[0-9]*)" _ ${ver})
string(REPLACE " " "" ver_major ${CMAKE_MATCH_1})
# Minor
string(REGEX MATCH "VERSION_MINOR([ \t]+[0-9]*)" _ ${ver})
string(REPLACE " " "" ver_minor ${CMAKE_MATCH_1})
# Patch
string(REGEX MATCH "VERSION_PATCH([ \t]+[0-9]*)" _ ${ver})
string(REPLACE " " "" ver_patch ${CMAKE_MATCH_1})
# Set the final firmware version string
set(firmware_version "v${ver_major}_${ver_minor}_${ver_patch}")

# Rename the output to .elf as we will create multiple files
set_target_properties(${PRODUCT_NAME} PROPERTIES OUTPUT_NAME ${PRODUCT_NAME}.elf)
# Strip binary for upload
add_custom_target(strip ALL avr-strip ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRODUCT_NAME}.elf DEPENDS ${PRODUCT_NAME})
# Transform binary into hex file
add_custom_target(hex ALL avr-objcopy -j .text -j .data -O ihex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRODUCT_NAME}.elf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRODUCT_NAME}-${firmware_version}.hex DEPENDS strip)
# Print out the binary size
add_custom_target(size ALL avr-size -C --mcu=${MCU} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRODUCT_NAME}.elf DEPENDS hex)

# Erase via dfu-programmer
add_custom_target(dfu_erase dfu-programmer ${MCU} erase --force)
# Upload via dfu-programmer
add_custom_target(dfu_flash dfu-programmer ${MCU} erase --force && dfu-programmer ${MCU} flash ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRODUCT_NAME}-${firmware_version}.hex DEPENDS size)

# Upload the firmware with avrdude
add_custom_target(flash avrdude -c ${PROG_TYPE} -p ${MCU} -D -U flash:w:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PRODUCT_NAME}-${firmware_version}.hex:i DEPENDS size)
# Erase the chip via avrdude
add_custom_target(erase avrdude -p ${MCU} -c ${PROG_TYPE} -e)
# Write AVR Fuses via avrdude
add_custom_target(write_fuses avrdude -p ${MCU} -c ${PROG_TYPE} -U lfuse:w:${LOW_FUSE_VAL}:m -U hfuse:w:${HIGH_FUSE_VAL}:m -U efuse:w:${EXTENDED_FUSE_VAL}:m)
# Read AVR Fuses via avrdude
add_custom_target(read_fuses avrdude -p ${MCU} -c ${PROG_TYPE} -U lfuse:r:${CMAKE_SOURCE_DIR}/lfuse.txt:h -U hfuse:r:${CMAKE_SOURCE_DIR}/hfuse.txt:h -U efuse:r:${CMAKE_SOURCE_DIR}/efuse.txt:h)

# Execute our ceedling tests
add_custom_target(test cd ../ && ceedling gcov:all utils:gcov)