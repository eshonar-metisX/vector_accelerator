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
include CMakeFiles/test.exec.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test.exec.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test.exec.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test.exec.dir/flags.make

CMakeFiles/test.exec.dir/test_main.cpp.o: CMakeFiles/test.exec.dir/flags.make
CMakeFiles/test.exec.dir/test_main.cpp.o: ../test_main.cpp
CMakeFiles/test.exec.dir/test_main.cpp.o: CMakeFiles/test.exec.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chpark/workspace/vector_accelerator/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test.exec.dir/test_main.cpp.o"
	/usr/bin/g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.exec.dir/test_main.cpp.o -MF CMakeFiles/test.exec.dir/test_main.cpp.o.d -o CMakeFiles/test.exec.dir/test_main.cpp.o -c /home/chpark/workspace/vector_accelerator/test/test_main.cpp

CMakeFiles/test.exec.dir/test_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.exec.dir/test_main.cpp.i"
	/usr/bin/g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chpark/workspace/vector_accelerator/test/test_main.cpp > CMakeFiles/test.exec.dir/test_main.cpp.i

CMakeFiles/test.exec.dir/test_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.exec.dir/test_main.cpp.s"
	/usr/bin/g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chpark/workspace/vector_accelerator/test/test_main.cpp -o CMakeFiles/test.exec.dir/test_main.cpp.s

# Object files for target test.exec
test_exec_OBJECTS = \
"CMakeFiles/test.exec.dir/test_main.cpp.o"

# External object files for target test.exec
test_exec_EXTERNAL_OBJECTS =

test.exec: CMakeFiles/test.exec.dir/test_main.cpp.o
test.exec: CMakeFiles/test.exec.dir/build.make
test.exec: libgtest.a
test.exec: CMakeFiles/test.exec.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chpark/workspace/vector_accelerator/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test.exec"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test.exec.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test.exec.dir/build: test.exec
.PHONY : CMakeFiles/test.exec.dir/build

CMakeFiles/test.exec.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test.exec.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test.exec.dir/clean

CMakeFiles/test.exec.dir/depend:
	cd /home/chpark/workspace/vector_accelerator/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chpark/workspace/vector_accelerator/test /home/chpark/workspace/vector_accelerator/test /home/chpark/workspace/vector_accelerator/test/build /home/chpark/workspace/vector_accelerator/test/build /home/chpark/workspace/vector_accelerator/test/build/CMakeFiles/test.exec.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test.exec.dir/depend

