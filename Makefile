# Makefile
#   
# Operating and File Systems
# Summer, 2012.  Roozbeh Mehrabadi
# 
#	make 		will create the executable "execUs", 
#	make clean 	will remove object/executable and backup files (.o, *~ and the TARGETS )
#	make obj 	will only create the object file: execUs.o
#
###################################################
CC   := gcc

CFLAGS := -g -Wall 

TARGETS := tsh.o  tsh

# your "all" could be for more than one target (all should be your first target)
all: $(TARGETS) 

# Note the tab before the action/command ([tab] rm -rf ...) without it you get an error 
#  from make (multiple spaces won't do) e.g. *** missing separator.  Stop. 
clean:
	rm -rf core *.o *.out *~ $(TARGETS)

# Lable: depends-on1 depends-on2 ... 
#  Action (run something like: gcc) followed by arguments
execUs: execUs.o

	$(CC) $(CFLAGS) -o tsh tsh.o

# Just create the object file (note: the action/gcc creates the object file, not the label 
#  	the label could have been xoxo: and your command> make xoxo would create the object file
# 	execUs.o
obj: main.c

	$(CC) $(CFLAGS) -c -o tsh.o tsh.c


