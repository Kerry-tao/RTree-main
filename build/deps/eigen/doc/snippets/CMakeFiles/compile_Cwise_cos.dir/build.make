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
include doc/snippets/CMakeFiles/compile_Cwise_cos.dir/depend.make

# Include the progress variables for this target.
include doc/snippets/CMakeFiles/compile_Cwise_cos.dir/progress.make

# Include the compile flags for this target's objects.
include doc/snippets/CMakeFiles/compile_Cwise_cos.dir/flags.make

doc/snippets/CMakeFiles/compile_Cwise_cos.dir/compile_Cwise_cos.cpp.o: doc/snippets/CMakeFiles/compile_Cwise_cos.dir/flags.make
doc/snippets/CMakeFiles/compile_Cwise_cos.dir/compile_Cwise_cos.cpp.o: doc/snippets/compile_Cwise_cos.cpp
doc/snippets/CMakeFiles/compile_Cwise_cos.dir/compile_Cwise_cos.cpp.o: /home/zhang/sllll/my/RTree-main/deps/eigen/doc/snippets/Cwise_cos.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zhang/sllll/my/RTree-main/build/deps/eigen/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object doc/snippets/CMakeFiles/compile_Cwise_cos.dir/compile_Cwise_cos.cpp.o"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/snippets && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compile_Cwise_cos.dir/compile_Cwise_cos.cpp.o -c /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/snippets/compile_Cwise_cos.cpp

doc/snippets/CMakeFiles/compile_Cwise_cos.dir/compile_Cwise_cos.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compile_Cwise_cos.dir/compile_Cwise_cos.cpp.i"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/snippets && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/snippets/compile_Cwise_cos.cpp > CMakeFiles/compile_Cwise_cos.dir/compile_Cwise_cos.cpp.i

doc/snippets/CMakeFiles/compile_Cwise_cos.dir/compile_Cwise_cos.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compile_Cwise_cos.dir/compile_Cwise_cos.cpp.s"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/snippets && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/snippets/compile_Cwise_cos.cpp -o CMakeFiles/compile_Cwise_cos.dir/compile_Cwise_cos.cpp.s

# Object files for target compile_Cwise_cos
compile_Cwise_cos_OBJECTS = \
"CMakeFiles/compile_Cwise_cos.dir/compile_Cwise_cos.cpp.o"

# External object files for target compile_Cwise_cos
compile_Cwise_cos_EXTERNAL_OBJECTS =

doc/snippets/compile_Cwise_cos: doc/snippets/CMakeFiles/compile_Cwise_cos.dir/compile_Cwise_cos.cpp.o
doc/snippets/compile_Cwise_cos: doc/snippets/CMakeFiles/compile_Cwise_cos.dir/build.make
doc/snippets/compile_Cwise_cos: doc/snippets/CMakeFiles/compile_Cwise_cos.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zhang/sllll/my/RTree-main/build/deps/eigen/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable compile_Cwise_cos"
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/snippets && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/compile_Cwise_cos.dir/link.txt --verbose=$(VERBOSE)
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/snippets && ./compile_Cwise_cos >/home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/snippets/Cwise_cos.out

# Rule to build all files generated by this target.
doc/snippets/CMakeFiles/compile_Cwise_cos.dir/build: doc/snippets/compile_Cwise_cos

.PHONY : doc/snippets/CMakeFiles/compile_Cwise_cos.dir/build

doc/snippets/CMakeFiles/compile_Cwise_cos.dir/clean:
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/snippets && $(CMAKE_COMMAND) -P CMakeFiles/compile_Cwise_cos.dir/cmake_clean.cmake
.PHONY : doc/snippets/CMakeFiles/compile_Cwise_cos.dir/clean

doc/snippets/CMakeFiles/compile_Cwise_cos.dir/depend:
	cd /home/zhang/sllll/my/RTree-main/build/deps/eigen && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zhang/sllll/my/RTree-main/deps/eigen /home/zhang/sllll/my/RTree-main/deps/eigen/doc/snippets /home/zhang/sllll/my/RTree-main/build/deps/eigen /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/snippets /home/zhang/sllll/my/RTree-main/build/deps/eigen/doc/snippets/CMakeFiles/compile_Cwise_cos.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : doc/snippets/CMakeFiles/compile_Cwise_cos.dir/depend

