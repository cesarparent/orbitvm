# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.14.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.14.3/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/amy/workspace/dev/orbitvm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/amy/workspace/dev/orbitvm/build.js

# Include any dependencies generated for this target.
include bin/repl/CMakeFiles/orbit.dir/depend.make

# Include the progress variables for this target.
include bin/repl/CMakeFiles/orbit.dir/progress.make

# Include the compile flags for this target's objects.
include bin/repl/CMakeFiles/orbit.dir/flags.make

bin/repl/CMakeFiles/orbit.dir/main.c.o: bin/repl/CMakeFiles/orbit.dir/flags.make
bin/repl/CMakeFiles/orbit.dir/main.c.o: bin/repl/CMakeFiles/orbit.dir/includes_C.rsp
bin/repl/CMakeFiles/orbit.dir/main.c.o: ../bin/repl/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/amy/workspace/dev/orbitvm/build.js/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object bin/repl/CMakeFiles/orbit.dir/main.c.o"
	cd /Users/amy/workspace/dev/orbitvm/build.js/bin/repl && /Users/amy/toolchains/emsdk/fastcomp/emscripten/emcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/orbit.dir/main.c.o   -c /Users/amy/workspace/dev/orbitvm/bin/repl/main.c

bin/repl/CMakeFiles/orbit.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/orbit.dir/main.c.i"
	cd /Users/amy/workspace/dev/orbitvm/build.js/bin/repl && /Users/amy/toolchains/emsdk/fastcomp/emscripten/emcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/amy/workspace/dev/orbitvm/bin/repl/main.c > CMakeFiles/orbit.dir/main.c.i

bin/repl/CMakeFiles/orbit.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/orbit.dir/main.c.s"
	cd /Users/amy/workspace/dev/orbitvm/build.js/bin/repl && /Users/amy/toolchains/emsdk/fastcomp/emscripten/emcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/amy/workspace/dev/orbitvm/bin/repl/main.c -o CMakeFiles/orbit.dir/main.c.s

# Object files for target orbit
orbit_OBJECTS = \
"CMakeFiles/orbit.dir/main.c.o"

# External object files for target orbit
orbit_EXTERNAL_OBJECTS =

bin/repl/orbit.js: bin/repl/CMakeFiles/orbit.dir/main.c.o
bin/repl/orbit.js: bin/repl/CMakeFiles/orbit.dir/build.make
bin/repl/orbit.js: libs/utils/libOrbitUtils.a
bin/repl/orbit.js: libs/rt2/libOrbitRT2.a
bin/repl/orbit.js: libs/parser/libOrbitParser.a
bin/repl/orbit.js: libs/sema/libOrbitSema.a
bin/repl/orbit.js: libs/ast/libOrbitAST.a
bin/repl/orbit.js: libs/codegen/libOrbitCodegen.a
bin/repl/orbit.js: packages/TermUtils/src/libtermutils.a
bin/repl/orbit.js: libs/mangling/libOrbitMangling.a
bin/repl/orbit.js: libs/rt2/libOrbitRT2.a
bin/repl/orbit.js: libs/ast/libOrbitAST.a
bin/repl/orbit.js: libs/csupport/libOrbitCSupport.a
bin/repl/orbit.js: libs/utils/libOrbitUtils.a
bin/repl/orbit.js: packages/TermUtils/src/libtermutils.a
bin/repl/orbit.js: packages/UniC/src/libUniC.a
bin/repl/orbit.js: bin/repl/CMakeFiles/orbit.dir/linklibs.rsp
bin/repl/orbit.js: bin/repl/CMakeFiles/orbit.dir/objects1.rsp
bin/repl/orbit.js: bin/repl/CMakeFiles/orbit.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/amy/workspace/dev/orbitvm/build.js/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable orbit.js"
	cd /Users/amy/workspace/dev/orbitvm/build.js/bin/repl && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/orbit.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bin/repl/CMakeFiles/orbit.dir/build: bin/repl/orbit.js

.PHONY : bin/repl/CMakeFiles/orbit.dir/build

bin/repl/CMakeFiles/orbit.dir/clean:
	cd /Users/amy/workspace/dev/orbitvm/build.js/bin/repl && $(CMAKE_COMMAND) -P CMakeFiles/orbit.dir/cmake_clean.cmake
.PHONY : bin/repl/CMakeFiles/orbit.dir/clean

bin/repl/CMakeFiles/orbit.dir/depend:
	cd /Users/amy/workspace/dev/orbitvm/build.js && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/amy/workspace/dev/orbitvm /Users/amy/workspace/dev/orbitvm/bin/repl /Users/amy/workspace/dev/orbitvm/build.js /Users/amy/workspace/dev/orbitvm/build.js/bin/repl /Users/amy/workspace/dev/orbitvm/build.js/bin/repl/CMakeFiles/orbit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : bin/repl/CMakeFiles/orbit.dir/depend

