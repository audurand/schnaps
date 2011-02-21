+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
SCHNAPS: SynCHroNous Agent- and Population-based Simulator

Copyright (C) 2009-2011
by Audrey Durand <audrey.durand.gif@gmail.com> and other contributors:
   Christian Gagne
   Mathieu Gagnon
   Marc-Andre Gardner
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

ABOUT
=====

DOCUMENTATION
=============

INSTALLATION
============

To install SCHNAPS, you need first to install CMake 
(http://www.cmake.org), version 2.6 are later.

To build and install SCHNAPS, follow these easy instructions:

1- Create an empty directory anywhere outside of the SCHNAPS 
   sub-tree;

  > mkdir schnaps-cmake

2- Set this directory as the current directory;

  > cd schnaps-cmake

3- Run CMake with the path to the SCHNAPS source sub-tree;

  > cmake ../SCHNAPS
  
  (the example assumes that the SCHNAPS directory is a 
   sibling of the current directory)

  During this process cmake will search for several dependencies such as PACC and Z-lib. Yous can specify additionnal search paths using CMAKE_PREFIX_PATH.
  For example:
  > cmake -D CMAKE_PREFIX_PATH=my-own-specific-search-path ../SCHNAPS

4- Run make to compile the sources and build the library and executable;

  > make

5- Run make again with the "install" target to install the 
   library and executable in a default OS dependent location (determined by 
   CMake);

  > sudo make install
  > password: enter-your-admin-password
  
  On Unix, it might be /usr/local/lib, /usr/local/bin and /usr/local/include. If you want to change 
  this location, you need during step 3 to specify the new
  location to CMake, by changing variable CMAKE_INSTALL_PREFIX. 
  For example:
  
  > cmake -D CMAKE_INSTALL_PREFIX=my-own-specific-path ../SCHNAPS

  will configure the makefile to install the library in
  the directory pointed by "my-own-specific-path"/lib, "my-own-specific-path"/include for includes and "my-own-specific-path"/bin for executable.

By default, CMake will compile the sources in release mode 
(with optimization flags and without debug symbols). If you want 
to trace your code into SCHNAPS, you need to tell CMake to compile 
with adequate debug flags using the "CMAKE_BUILD_TYPE=Debug" 
variable. This variable can be set on the command line, just like 
the "CMAKE_INSTALL_PREFIX" variable above:

  > cmake -D CMAKE_BUILD_TYPE=Debug ../SCHNAPS
  
 An alternative is to run CMake in interactive mode:

  > cmake -i ../SCHNAPS
  
CMake will then ask you questions so that you can configure every 
aspect of the build process, by allowing you to update any of its 
internal build variables. Refer to the CMake manual for further 
details.
If you have install the CMake GUI, you may also want to use it
instead of command-line interface.


Some variables are specific to SCHNAPS and should be considered :

  - SCHNAPS_LIBRARY_TYPE = [STATIC | SHARED] defines whether CMake 
  should build static or shared (dynamic) library. On Unix 
  platforms and on Windows without the MinGW (GNU) compiler, 
  default behavior is to SHARED. When using the Microsoft VC++
  compiler, default is to STATIC.
  
