# Name of our project
# (use a single word, e.g. 'myproject')
PROJECTNAME=libmylib

# Name of compiled file
TRG=$(PROJECTNAME).a

# Source files
# List C/C++/Assembly source files:
# (list all files to compile, e.g. 'a.c b.cpp as.S'):
# Use .cc, .cpp or .C suffix for C++ files, use .S
# (NOT .s !!!) for assembly source code files.
PRJSRC=tp.cpp can.cpp memoire_24.cpp usart.cpp 

# additional includes (e.g. -I/path/to/mydir)
INC=          
              
# libraries to link in (e.g. -lmylib)
LIBS=         
        
include ../Makefile_Common.txt         
                        
$(TRG): $(OBJDEPS)
	$(AR) rcs $(TRG) $(OBJDEPS) 
