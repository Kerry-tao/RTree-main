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
include doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/depend.make

# Include the progress variables for this target.
include doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/progress.make

# Include the compile flags for this target's objects.
include doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/flags.make

doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/QuickStart_example2_fixed.cpp.o: doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/flags.make
doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/QuickStart_example2_fixed.cpp.o: /home/zhang/sllll/my/RTree-main/deps/eigen/doc/examples/QuickStart_example2_fixed.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zhang/sllll/my/RTree-main/build/deps/eigen/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/QuickStart_example2_fixed.cpp.o"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QuickStart_example2_fixed.dir/QuickStart_example2_fixed.cpp.o -c /home/zhang/sllll/my/RTree-main/deps/eigen/doc/examples/QuickStart_example2_fixed.cpp

doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/QuickStart_example2_fixed.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QuickStart_example2_fixed.dir/QuickStart_example2_fixed.cpp.i"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zhang/sllll/my/RTree-main/deps/eigen/doc/examples/QuickStart_example2_fixed.cpp > CMakeFiles/QuickStart_example2_fixed.dir/QuickStart_example2_fixed.cpp.i

doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/QuickStart_example2_fixed.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QuickStart_example2_fixed.dir/QuickStart_example2_fixed.cpp.s"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zhang/sllll/my/RTree-main/deps/eigen/doc/examples/QuickStart_example2_fixed.cpp -o CMakeFiles/QuickStart_example2_fixed.dir/QuickStart_example2_fixed.cpp.s

# Object files for target QuickStart_example2_fixed
QuickStart_example2_fixed_OBJECTS = \
"CMakeFiles/QuickStart_example2_fixed.dir/QuickStart_example2_fixed.cpp.o"

# External object files for target QuickStart_example2_fixed
QuickStart_example2_fixed_EXTERNAL_OBJECTS =

doc/examples/QuickStart_example2_fixed: doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/QuickStart_example2_fixed.cpp.o
doc/examples/QuickStart_example2_fixed: doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/build.make
doc/examples/QuickStart_example2_fixed: doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zhang/sllll/my/RTree-main/build/deps/eigen/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable QuickStart_example2_fixed"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/QuickStart_example2_fixed.dir/link.txt --verbose=$(VERBOSE)
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/examples && ./QuickStart_example2_fixed >/home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/examples/QuickStart_example2_fixed.out

# Rule to build all files generated by this target.
doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/build: doc/examples/QuickStart_example2_fixed

.PHONY : doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/build

doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/clean:
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/examples && $(CMAKE_COMMAND) -P CMakeFiles/QuickStart_example2_fixed.dir/cmake_clean.cmake
.PHONY : doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/clean

doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/depend:
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhang/sllll/my/RTree-main/deps/eigen /home/zhang/sllll/my/RTree-main/deps/eigen/doc/examples /home/zhang/sllll/my/RTree-main/build/deps/eigen /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/examples /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : doc/examples/CMakeFiles/QuickStart_example2_fixed.dir/depend

