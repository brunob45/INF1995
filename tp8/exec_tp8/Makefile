# Name of our project
# (use a single word, e.g. 'myproject')
PROJECTNAME=myexec

# Name of compiled file
TRG=$(PROJECTNAME).out

# Source files
# List C/C++/Assembly source files:
# (list all files to compile, e.g. 'a.c b.cpp as.S'):
# Use .cc, .cpp or .C suffix for C++ files, use .S
# (NOT .s !!!) for assembly source code files.
PRJSRC=test.cpp

# additional includes (e.g. -I/path/to/mydir)
INC= -I ../lib_tp8/

# libraries to link in (e.g. -lmylib)
LIBS= -L. -L../lib_tp8/ -lmylib

include ../Makefile_Common.txt

$(TRG): $(OBJDEPS)
	$(CC) $(LDFLAGS) -o $(TRG) $(OBJDEPS) \
	$(LIBS)

