# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chpark/workspace/vector_accelerator/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chpark/workspace/vector_accelerator/test/build

# Include any dependencies generated for this target.
include CMakeFiles/gtest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/gtest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/gtest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gtest.dir/flags.make

CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.o: CMakeFiles/gtest.dir/flags.make
CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.o: /usr/src/gtest/src/gtest-all.cc
CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.o: CMakeFiles/gtest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chpark/workspace/vector_accelerator/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.o"
	/usr/bin/g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.o -MF CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.o.d -o CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.o -c /usr/src/gtest/src/gtest-all.cc

CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.i"
	/usr/bin/g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /usr/src/gtest/src/gtest-all.cc > CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.i

CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.s"
	/usr/bin/g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /usr/src/gtest/src/gtest-all.cc -o CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.s

# Object files for target gtest
gtest_OBJECTS = \
"CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.o"

# External object files for target gtest
gtest_EXTERNAL_OBJECTS =

libgtest.a: CMakeFiles/gtest.dir/usr/src/gtest/src/gtest-all.cc.o
libgtest.a: CMakeFiles/gtest.dir/build.make
libgtest.a: CMakeFiles/gtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chpark/workspace/vector_accelerator/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libgtest.a"
	$(CMAKE_COMMAND) -P CMakeFiles/gtest.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gtest.dir/build: libgtest.a
.PHONY : CMakeFiles/gtest.dir/build

CMakeFiles/gtest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gtest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gtest.dir/clean

CMakeFiles/gtest.dir/depend:
	cd /home/chpark/workspace/vector_accelerator/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chpark/workspace/vector_accelerator/test /home/chpark/workspace/vector_accelerator/test /home/chpark/workspace/vector_accelerator/test/build /home/chpark/workspace/vector_accelerator/test/build /home/chpark/workspace/vector_accelerator/test/build/CMakeFiles/gtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gtest.dir/depend

