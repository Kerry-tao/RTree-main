# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zhang/sllll/my/RTree-main/deps/emp-tool

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zhang/sllll/my/RTree-main/build/deps/emp-tool

# Include any dependencies generated for this target.
include test/CMakeFiles/test_int.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_int.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_int.dir/flags.make

test/CMakeFiles/test_int.dir/int.cpp.o: test/CMakeFiles/test_int.dir/flags.make
test/CMakeFiles/test_int.dir/int.cpp.o: /home/zhang/sllll/my/RTree-main/deps/emp-tool/test/int.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zhang/sllll/my/RTree-main/build/deps/emp-tool/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_int.dir/int.cpp.o"
	cd /home/zhang/sllll/my/RTree-main/build/deps/emp-tool/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_int.dir/int.cpp.o -c /home/zhang/sllll/my/RTree-main/deps/emp-tool/test/int.cpp

test/CMakeFiles/test_int.dir/int.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_int.dir/int.cpp.i"
	cd /home/zhang/sllll/my/RTree-main/build/deps/emp-tool/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zhang/sllll/my/RTree-main/deps/emp-tool/test/int.cpp > CMakeFiles/test_int.dir/int.cpp.i

test/CMakeFiles/test_int.dir/int.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_int.dir/int.cpp.s"
	cd /home/zhang/sllll/my/RTree-main/build/deps/emp-tool/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zhang/sllll/my/RTree-main/deps/emp-tool/test/int.cpp -o CMakeFiles/test_int.dir/int.cpp.s

# Object files for target test_int
test_int_OBJECTS = \
"CMakeFiles/test_int.dir/int.cpp.o"

# External object files for target test_int
test_int_EXTERNAL_OBJECTS =

bin/test_int: test/CMakeFiles/test_int.dir/int.cpp.o
bin/test_int: test/CMakeFiles/test_int.dir/build.make
bin/test_int: /usr/lib/x86_64-linux-gnu/libssl.so
bin/test_int: /usr/lib/x86_64-linux-gnu/libcrypto.so
bin/test_int: libemp-tool.so
bin/test_int: test/CMakeFiles/test_int.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zhang/sllll/my/RTree-main/build/deps/emp-tool/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test_int"
	cd /home/zhang/sllll/my/RTree-main/build/deps/emp-tool/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_int.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_int.dir/build: bin/test_int

.PHONY : test/CMakeFiles/test_int.dir/build

test/CMakeFiles/test_int.dir/clean:
	cd /home/zhang/sllll/my/RTree-main/build/deps/emp-tool/test && $(CMAKE_COMMAND) -P CMakeFiles/test_int.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_int.dir/clean

test/CMakeFiles/test_int.dir/depend:
	cd /home/zhang/sllll/my/RTree-main/build/deps/emp-tool && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhang/sllll/my/RTree-main/deps/emp-tool /home/zhang/sllll/my/RTree-main/deps/emp-tool/test /home/zhang/sllll/my/RTree-main/build/deps/emp-tool /home/zhang/sllll/my/RTree-main/build/deps/emp-tool/test /home/zhang/sllll/my/RTree-main/build/deps/emp-tool/test/CMakeFiles/test_int.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test_int.dir/depend

