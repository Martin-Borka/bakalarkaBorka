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
CMAKE_SOURCE_DIR = /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug

# Include any dependencies generated for this target.
include lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/compiler_depend.make

# Include the progress variables for this target.
include lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/progress.make

# Include the compile flags for this target's objects.
include lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/flags.make

lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.o: lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/flags.make
lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.o: ../lib60870-C/examples/cs104_server_no_threads/cs104_server_no_threads.c
lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.o: lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.o"
	cd /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug/lib60870-C/examples/cs104_server_no_threads && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.o -MF CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.o.d -o CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.o -c /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/examples/cs104_server_no_threads/cs104_server_no_threads.c

lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.i"
	cd /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug/lib60870-C/examples/cs104_server_no_threads && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/examples/cs104_server_no_threads/cs104_server_no_threads.c > CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.i

lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.s"
	cd /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug/lib60870-C/examples/cs104_server_no_threads && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/examples/cs104_server_no_threads/cs104_server_no_threads.c -o CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.s

# Object files for target cs104_server_no_threads
cs104_server_no_threads_OBJECTS = \
"CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.o"

# External object files for target cs104_server_no_threads
cs104_server_no_threads_EXTERNAL_OBJECTS =

lib60870-C/examples/cs104_server_no_threads/cs104_server_no_threads: lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/cs104_server_no_threads.c.o
lib60870-C/examples/cs104_server_no_threads/cs104_server_no_threads: lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/build.make
lib60870-C/examples/cs104_server_no_threads/cs104_server_no_threads: lib60870-C/src/liblib60870.a
lib60870-C/examples/cs104_server_no_threads/cs104_server_no_threads: lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable cs104_server_no_threads"
	cd /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug/lib60870-C/examples/cs104_server_no_threads && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cs104_server_no_threads.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/build: lib60870-C/examples/cs104_server_no_threads/cs104_server_no_threads
.PHONY : lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/build

lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/clean:
	cd /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug/lib60870-C/examples/cs104_server_no_threads && $(CMAKE_COMMAND) -P CMakeFiles/cs104_server_no_threads.dir/cmake_clean.cmake
.PHONY : lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/clean

lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/depend:
	cd /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/lib60870-C/examples/cs104_server_no_threads /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug/lib60870-C/examples/cs104_server_no_threads /mnt/c/Users/Martin/OneDrive/Dokumenty/GitHub/bakalarkaBorka/cmake-build-debug/lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib60870-C/examples/cs104_server_no_threads/CMakeFiles/cs104_server_no_threads.dir/depend

