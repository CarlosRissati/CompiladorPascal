# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 4.0

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
CMAKE_SOURCE_DIR = /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/build

# Include any dependencies generated for this target.
include CMakeFiles/compiler.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/compiler.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/compiler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/compiler.dir/flags.make

CMakeFiles/compiler.dir/codegen:
.PHONY : CMakeFiles/compiler.dir/codegen

CMakeFiles/compiler.dir/src/main.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/src/main.cpp.o: /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/src/main.cpp
CMakeFiles/compiler.dir/src/main.cpp.o: CMakeFiles/compiler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/compiler.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/compiler.dir/src/main.cpp.o -MF CMakeFiles/compiler.dir/src/main.cpp.o.d -o CMakeFiles/compiler.dir/src/main.cpp.o -c /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/src/main.cpp

CMakeFiles/compiler.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/compiler.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/src/main.cpp > CMakeFiles/compiler.dir/src/main.cpp.i

CMakeFiles/compiler.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/src/main.cpp -o CMakeFiles/compiler.dir/src/main.cpp.s

CMakeFiles/compiler.dir/src/analisador_semantico.cpp.o: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/src/analisador_semantico.cpp.o: /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/src/analisador_semantico.cpp
CMakeFiles/compiler.dir/src/analisador_semantico.cpp.o: CMakeFiles/compiler.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/compiler.dir/src/analisador_semantico.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/compiler.dir/src/analisador_semantico.cpp.o -MF CMakeFiles/compiler.dir/src/analisador_semantico.cpp.o.d -o CMakeFiles/compiler.dir/src/analisador_semantico.cpp.o -c /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/src/analisador_semantico.cpp

CMakeFiles/compiler.dir/src/analisador_semantico.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/compiler.dir/src/analisador_semantico.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/src/analisador_semantico.cpp > CMakeFiles/compiler.dir/src/analisador_semantico.cpp.i

CMakeFiles/compiler.dir/src/analisador_semantico.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/src/analisador_semantico.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/src/analisador_semantico.cpp -o CMakeFiles/compiler.dir/src/analisador_semantico.cpp.s

# Object files for target compiler
compiler_OBJECTS = \
"CMakeFiles/compiler.dir/src/main.cpp.o" \
"CMakeFiles/compiler.dir/src/analisador_semantico.cpp.o"

# External object files for target compiler
compiler_EXTERNAL_OBJECTS =

compiler: CMakeFiles/compiler.dir/src/main.cpp.o
compiler: CMakeFiles/compiler.dir/src/analisador_semantico.cpp.o
compiler: CMakeFiles/compiler.dir/build.make
compiler: CMakeFiles/compiler.dir/compiler_depend.ts
compiler: CMakeFiles/compiler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable compiler"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/compiler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/compiler.dir/build: compiler
.PHONY : CMakeFiles/compiler.dir/build

CMakeFiles/compiler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/compiler.dir/cmake_clean.cmake
.PHONY : CMakeFiles/compiler.dir/clean

CMakeFiles/compiler.dir/depend:
	cd /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/build /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/build /home/luka/Documentos/UNESPAR/Compiladores/CompiladorPascal/build/CMakeFiles/compiler.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/compiler.dir/depend

