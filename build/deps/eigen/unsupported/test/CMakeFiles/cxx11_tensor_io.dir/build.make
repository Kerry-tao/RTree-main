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
include unsupported/test/CMakeFiles/cxx11_tensor_io.dir/depend.make

# Include the progress variables for this target.
include unsupported/test/CMakeFiles/cxx11_tensor_io.dir/progress.make

# Include the compile flags for this target's objects.
include unsupported/test/CMakeFiles/cxx11_tensor_io.dir/flags.make

unsupported/test/CMakeFiles/cxx11_tensor_io.dir/cxx11_tensor_io.cpp.o: unsupported/test/CMakeFiles/cxx11_tensor_io.dir/flags.make
unsupported/test/CMakeFiles/cxx11_tensor_io.dir/cxx11_tensor_io.cpp.o: /home/zhang/sllll/my/RTree-main/deps/eigen/unsupported/test/cxx11_tensor_io.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zhang/sllll/my/RTree-main/build/deps/eigen/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object unsupported/test/CMakeFiles/cxx11_tensor_io.dir/cxx11_tensor_io.cpp.o"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/unsupported/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cxx11_tensor_io.dir/cxx11_tensor_io.cpp.o -c /home/zhang/sllll/my/RTree-main/deps/eigen/unsupported/test/cxx11_tensor_io.cpp

unsupported/test/CMakeFiles/cxx11_tensor_io.dir/cxx11_tensor_io.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cxx11_tensor_io.dir/cxx11_tensor_io.cpp.i"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/unsupported/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zhang/sllll/my/RTree-main/deps/eigen/unsupported/test/cxx11_tensor_io.cpp > CMakeFiles/cxx11_tensor_io.dir/cxx11_tensor_io.cpp.i

unsupported/test/CMakeFiles/cxx11_tensor_io.dir/cxx11_tensor_io.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cxx11_tensor_io.dir/cxx11_tensor_io.cpp.s"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/unsupported/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zhang/sllll/my/RTree-main/deps/eigen/unsupported/test/cxx11_tensor_io.cpp -o CMakeFiles/cxx11_tensor_io.dir/cxx11_tensor_io.cpp.s

# Object files for target cxx11_tensor_io
cxx11_tensor_io_OBJECTS = \
"CMakeFiles/cxx11_tensor_io.dir/cxx11_tensor_io.cpp.o"

# External object files for target cxx11_tensor_io
cxx11_tensor_io_EXTERNAL_OBJECTS =

unsupported/test/cxx11_tensor_io: unsupported/test/CMakeFiles/cxx11_tensor_io.dir/cxx11_tensor_io.cpp.o
unsupported/test/cxx11_tensor_io: unsupported/test/CMakeFiles/cxx11_tensor_io.dir/build.make
unsupported/test/cxx11_tensor_io: unsupported/test/CMakeFiles/cxx11_tensor_io.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zhang/sllll/my/RTree-main/build/deps/eigen/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cxx11_tensor_io"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/unsupported/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cxx11_tensor_io.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
unsupported/test/CMakeFiles/cxx11_tensor_io.dir/build: unsupported/test/cxx11_tensor_io

.PHONY : unsupported/test/CMakeFiles/cxx11_tensor_io.dir/build

unsupported/test/CMakeFiles/cxx11_tensor_io.dir/clean:
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/unsupported/test && $(CMAKE_COMMAND) -P CMakeFiles/cxx11_tensor_io.dir/cmake_clean.cmake
.PHONY : unsupported/test/CMakeFiles/cxx11_tensor_io.dir/clean

unsupported/test/CMakeFiles/cxx11_tensor_io.dir/depend:
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhang/sllll/my/RTree-main/deps/eigen /home/zhang/sllll/my/RTree-main/deps/eigen/unsupported/test /home/zhang/sllll/my/RTree-main/build/deps/eigen /home/zhang/sllll/my/RTree-main/build/deps/eigen/unsupported/test /home/zhang/sllll/my/RTree-main/build/deps/eigen/unsupported/test/CMakeFiles/cxx11_tensor_io.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : unsupported/test/CMakeFiles/cxx11_tensor_io.dir/depend

