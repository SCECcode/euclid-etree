# Etree-based CVM service package

Originally developed by

    Tiankai Tu, David R. O'Hallaron, Julio Lopez
    Computer Science Department
    Carnegie Mellon University

Autotools build added by

    David Gill
    Southern California Earthquake Center
    University of Southern California

with further modifications by

    Brad Aagaard
    Geologic Hazards Science Center
    U.S. Geological Survey


## Content of the package

This package contains the programs and specifications of building the CVM etree database for the LA basin. There are three directories:
    
* `libsrc` contains the etree library souce code programs.
*  `examples` contains example programs demonstrating how to use the etree library.
* `cvm` contains the programs and specifications of building cvm etree database for the LA basin.

In each of these directories, there is a README file that explains the contents in details. 

## How to build the package

1. Run 'configure' that is in the top-level source directory.
2. Run 'make'.
3. RUn 'make install'.

You may optionally choose to build the examples. The steps are similar to that of building the etree library except for a different directory.

To build the programs for building the CVM etree database for the LA basin.

1. Enter `cvm` directory.
2. Read the `README` file. This is important. You need the instruction in README to set the variables correctly in the Makefile.
3. Modify the variables in Makefile properly as instructed.
4. Build the programs and etree database as per the instructions.

## Problem reporting 

Programs contained in this package are research codes. If you find any problem during compilation or any bugs while running the programs, please don't hesitate to send us a report describing the nature of the problem.

To report a problem, please send email to software@scec.org.
