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
CMAKE_SOURCE_DIR = /home/zhang/sllll/my/RTree-main/deps/eigen

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zhang/sllll/my/RTree-main/build/deps/eigen

# Include any dependencies generated for this target.
include test/CMakeFiles/stddeque_overload_4.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/stddeque_overload_4.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/stddeque_overload_4.dir/flags.make

test/CMakeFiles/stddeque_overload_4.dir/stddeque_overload.cpp.o: test/CMakeFiles/stddeque_overload_4.dir/flags.make
test/CMakeFiles/stddeque_overload_4.dir/stddeque_overload.cpp.o: /home/zhang/sllll/my/RTree-main/deps/eigen/test/stddeque_overload.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zhang/sllll/my/RTree-main/build/deps/eigen/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/stddeque_overload_4.dir/stddeque_overload.cpp.o"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/stddeque_overload_4.dir/stddeque_overload.cpp.o -c /home/zhang/sllll/my/RTree-main/deps/eigen/test/stddeque_overload.cpp

test/CMakeFiles/stddeque_overload_4.dir/stddeque_overload.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/stddeque_overload_4.dir/stddeque_overload.cpp.i"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zhang/sllll/my/RTree-main/deps/eigen/test/stddeque_overload.cpp > CMakeFiles/stddeque_overload_4.dir/stddeque_overload.cpp.i

test/CMakeFiles/stddeque_overload_4.dir/stddeque_overload.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/stddeque_overload_4.dir/stddeque_overload.cpp.s"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zhang/sllll/my/RTree-main/deps/eigen/test/stddeque_overload.cpp -o CMakeFiles/stddeque_overload_4.dir/stddeque_overload.cpp.s

# Object files for target stddeque_overload_4
stddeque_overload_4_OBJECTS = \
"CMakeFiles/stddeque_overload_4.dir/stddeque_overload.cpp.o"

# External object files for target stddeque_overload_4
stddeque_overload_4_EXTERNAL_OBJECTS =

test/stddeque_overload_4: test/CMakeFiles/stddeque_overload_4.dir/stddeque_overload.cpp.o
test/stddeque_overload_4: test/CMakeFiles/stddeque_overload_4.dir/build.make
test/stddeque_overload_4: test/CMakeFiles/stddeque_overload_4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zhang/sllll/my/RTree-main/build/deps/eigen/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable stddeque_overload_4"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/stddeque_overload_4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/stddeque_overload_4.dir/build: test/stddeque_overload_4

.PHONY : test/CMakeFiles/stddeque_overload_4.dir/build

test/CMakeFiles/stddeque_overload_4.dir/clean:
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/test && $(CMAKE_COMMAND) -P CMakeFiles/stddeque_overload_4.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/stddeque_overload_4.dir/clean

test/CMakeFiles/stddeque_overload_4.dir/depend:
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhang/sllll/my/RTree-main/deps/eigen /home/zhang/sllll/my/RTree-main/deps/eigen/test /home/zhang/sllll/my/RTree-main/build/deps/eigen /home/zhang/sllll/my/RTree-main/build/deps/eigen/test /home/zhang/sllll/my/RTree-main/build/deps/eigen/test/CMakeFiles/stddeque_overload_4.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/stddeque_overload_4.dir/depend

