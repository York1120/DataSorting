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
CMAKE_SOURCE_DIR = /windows/DataSorting/mail

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /windows/DataSorting/mail/build

# Include any dependencies generated for this target.
include CMakeFiles/EMailTest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/EMailTest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/EMailTest.dir/flags.make

CMakeFiles/EMailTest.dir/SendMail.cpp.o: CMakeFiles/EMailTest.dir/flags.make
CMakeFiles/EMailTest.dir/SendMail.cpp.o: ../SendMail.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/windows/DataSorting/mail/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/EMailTest.dir/SendMail.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EMailTest.dir/SendMail.cpp.o -c /windows/DataSorting/mail/SendMail.cpp

CMakeFiles/EMailTest.dir/SendMail.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EMailTest.dir/SendMail.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /windows/DataSorting/mail/SendMail.cpp > CMakeFiles/EMailTest.dir/SendMail.cpp.i

CMakeFiles/EMailTest.dir/SendMail.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EMailTest.dir/SendMail.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /windows/DataSorting/mail/SendMail.cpp -o CMakeFiles/EMailTest.dir/SendMail.cpp.s

CMakeFiles/EMailTest.dir/SendMailTest.cpp.o: CMakeFiles/EMailTest.dir/flags.make
CMakeFiles/EMailTest.dir/SendMailTest.cpp.o: ../SendMailTest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/windows/DataSorting/mail/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/EMailTest.dir/SendMailTest.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EMailTest.dir/SendMailTest.cpp.o -c /windows/DataSorting/mail/SendMailTest.cpp

CMakeFiles/EMailTest.dir/SendMailTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EMailTest.dir/SendMailTest.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /windows/DataSorting/mail/SendMailTest.cpp > CMakeFiles/EMailTest.dir/SendMailTest.cpp.i

CMakeFiles/EMailTest.dir/SendMailTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EMailTest.dir/SendMailTest.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /windows/DataSorting/mail/SendMailTest.cpp -o CMakeFiles/EMailTest.dir/SendMailTest.cpp.s

# Object files for target EMailTest
EMailTest_OBJECTS = \
"CMakeFiles/EMailTest.dir/SendMail.cpp.o" \
"CMakeFiles/EMailTest.dir/SendMailTest.cpp.o"

# External object files for target EMailTest
EMailTest_EXTERNAL_OBJECTS =

EMailTest: CMakeFiles/EMailTest.dir/SendMail.cpp.o
EMailTest: CMakeFiles/EMailTest.dir/SendMailTest.cpp.o
EMailTest: CMakeFiles/EMailTest.dir/build.make
EMailTest: CMakeFiles/EMailTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/windows/DataSorting/mail/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable EMailTest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EMailTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/EMailTest.dir/build: EMailTest

.PHONY : CMakeFiles/EMailTest.dir/build

CMakeFiles/EMailTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/EMailTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/EMailTest.dir/clean

CMakeFiles/EMailTest.dir/depend:
	cd /windows/DataSorting/mail/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /windows/DataSorting/mail /windows/DataSorting/mail /windows/DataSorting/mail/build /windows/DataSorting/mail/build /windows/DataSorting/mail/build/CMakeFiles/EMailTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/EMailTest.dir/depend

