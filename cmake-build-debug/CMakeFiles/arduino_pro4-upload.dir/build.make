# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/clion-2017.2.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /opt/clion-2017.2.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mschwarz/fhnw/pro4E/arduino_pro4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mschwarz/fhnw/pro4E/arduino_pro4/cmake-build-debug

# Utility rule file for arduino_pro4-upload.

# Include the progress variables for this target.
include CMakeFiles/arduino_pro4-upload.dir/progress.make

CMakeFiles/arduino_pro4-upload: arduino_pro4.elf
	/opt/arduino-1.8.5/hardware/tools/avr/bin/avrdude -C/opt/arduino-1.8.5/hardware/tools/avr/etc/avrdude.conf -patmega2560 -cwiring -b115200 -P/dev/ttyACM0 -D -V -Uflash:w:/home/mschwarz/fhnw/pro4E/arduino_pro4/cmake-build-debug/arduino_pro4.hex:i -Ueeprom:w:/home/mschwarz/fhnw/pro4E/arduino_pro4/cmake-build-debug/arduino_pro4.eep:i

arduino_pro4-upload: CMakeFiles/arduino_pro4-upload
arduino_pro4-upload: CMakeFiles/arduino_pro4-upload.dir/build.make

.PHONY : arduino_pro4-upload

# Rule to build all files generated by this target.
CMakeFiles/arduino_pro4-upload.dir/build: arduino_pro4-upload

.PHONY : CMakeFiles/arduino_pro4-upload.dir/build

CMakeFiles/arduino_pro4-upload.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/arduino_pro4-upload.dir/cmake_clean.cmake
.PHONY : CMakeFiles/arduino_pro4-upload.dir/clean

CMakeFiles/arduino_pro4-upload.dir/depend:
	cd /home/mschwarz/fhnw/pro4E/arduino_pro4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mschwarz/fhnw/pro4E/arduino_pro4 /home/mschwarz/fhnw/pro4E/arduino_pro4 /home/mschwarz/fhnw/pro4E/arduino_pro4/cmake-build-debug /home/mschwarz/fhnw/pro4E/arduino_pro4/cmake-build-debug /home/mschwarz/fhnw/pro4E/arduino_pro4/cmake-build-debug/CMakeFiles/arduino_pro4-upload.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/arduino_pro4-upload.dir/depend

