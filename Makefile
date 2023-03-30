# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /opt/software/software/CMake/3.16.4-GCCcore-9.3.0/bin/cmake

# The command to remove a file.
RM = /opt/software/software/CMake/3.16.4-GCCcore-9.3.0/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /homes/josh811/CIS520_Proj3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /homes/josh811/CIS520_Proj3

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/opt/software/software/CMake/3.16.4-GCCcore-9.3.0/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/opt/software/software/CMake/3.16.4-GCCcore-9.3.0/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /homes/josh811/CIS520_Proj3/CMakeFiles /homes/josh811/CIS520_Proj3/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /homes/josh811/CIS520_Proj3/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named libblock_store.so

# Build rule for target.
libblock_store.so: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 libblock_store.so
.PHONY : libblock_store.so

# fast build rule for target.
libblock_store.so/fast:
	$(MAKE) -f CMakeFiles/libblock_store.so.dir/build.make CMakeFiles/libblock_store.so.dir/build
.PHONY : libblock_store.so/fast

#=============================================================================
# Target rules for targets named hw3_test

# Build rule for target.
hw3_test: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 hw3_test
.PHONY : hw3_test

# fast build rule for target.
hw3_test/fast:
	$(MAKE) -f CMakeFiles/hw3_test.dir/build.make CMakeFiles/hw3_test.dir/build
.PHONY : hw3_test/fast

src/block_store.o: src/block_store.c.o

.PHONY : src/block_store.o

# target to build an object file
src/block_store.c.o:
	$(MAKE) -f CMakeFiles/libblock_store.so.dir/build.make CMakeFiles/libblock_store.so.dir/src/block_store.c.o
.PHONY : src/block_store.c.o

src/block_store.i: src/block_store.c.i

.PHONY : src/block_store.i

# target to preprocess a source file
src/block_store.c.i:
	$(MAKE) -f CMakeFiles/libblock_store.so.dir/build.make CMakeFiles/libblock_store.so.dir/src/block_store.c.i
.PHONY : src/block_store.c.i

src/block_store.s: src/block_store.c.s

.PHONY : src/block_store.s

# target to generate assembly for a file
src/block_store.c.s:
	$(MAKE) -f CMakeFiles/libblock_store.so.dir/build.make CMakeFiles/libblock_store.so.dir/src/block_store.c.s
.PHONY : src/block_store.c.s

test/tests.o: test/tests.cpp.o

.PHONY : test/tests.o

# target to build an object file
test/tests.cpp.o:
	$(MAKE) -f CMakeFiles/hw3_test.dir/build.make CMakeFiles/hw3_test.dir/test/tests.cpp.o
.PHONY : test/tests.cpp.o

test/tests.i: test/tests.cpp.i

.PHONY : test/tests.i

# target to preprocess a source file
test/tests.cpp.i:
	$(MAKE) -f CMakeFiles/hw3_test.dir/build.make CMakeFiles/hw3_test.dir/test/tests.cpp.i
.PHONY : test/tests.cpp.i

test/tests.s: test/tests.cpp.s

.PHONY : test/tests.s

# target to generate assembly for a file
test/tests.cpp.s:
	$(MAKE) -f CMakeFiles/hw3_test.dir/build.make CMakeFiles/hw3_test.dir/test/tests.cpp.s
.PHONY : test/tests.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... libblock_store.so"
	@echo "... hw3_test"
	@echo "... src/block_store.o"
	@echo "... src/block_store.i"
	@echo "... src/block_store.s"
	@echo "... test/tests.o"
	@echo "... test/tests.i"
	@echo "... test/tests.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

