#-----------------------------------------------------------------------------
# Makefile
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Choose a compiler and its options
#--------------------------------------------------------------------------
CC   = gcc
OPTS = -O0
DEBUG = -g
#--------------------------------------------------------------------------
# Add the names of the directories (Add them after each ./)
#--------------------------------------------------------------------------
SRCDIR=./src
OBJDIR=./obj
INCDIR=./inc
BINDIR=./bin

#--------------------------------------------------------------------
# Add the rest of the source files. Don't forget to add the '\' character
# to continue the line. You don't need it after the last source file.
# Note: The source directory, $(SRCDIR), needs to be given for each
# individual file. The $(SRCDIR) below shows how it is done for the
# Lab8.c file. Also note that there should only be one SRCS= (i.e. don't
# add more of them as you add files).
#--------------------------------------------------------------------
#//
#--------------------------------------------------------------------
# You don't need to edit the next few lines. They define other flags
# used in the compilation of the source code
#--------------------------------------------------------------------
INCLUDE = $(addprefix -I,$(INCDIR))
OBJS=${SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o}
CFLAGS   = $(OPTS) $(INCLUDE) $(DEBUG)
#--------------------------------------------------------------------
# Add the name of the executable after the $(BINDIR)/
#--------------------------------------------------------------------
all: Stego StegoExtract
Stego: image.o Stego.o
	${CC} ${CFLAGS} -o $@ Stego.o image.o

StegoExtract: image.o StegoExtract.o
	${CC} ${CFLAGS} -o $@ StegoExtract.o image.o

StegoExtract.o: StegoExtract.c image.h
	$(CC) $(CFLAGS) -c StegoExtract.c

Stego.o: Stego.c image.h
	$(CC) $(CFLAGS) -c Stego.c

image.o: image.c image.h
	$(CC) $(CFLAGS) -c image.c

#Stego.o: image.o Stego.o
#	${CC} ${CFLAGS} -c Stego.o image.o

#$(BINDIR)/StegoExtract: $(OBJDIR)/image.o $(OBJDIR)/StegoExtract.o
#	${CC} ${CFLAGS} -o $@ $(OBJDIR)/StegoExtract.o $(OBJDIR)/image.o

#$(OBJDIR)/StegoExtract.o :$(OBJDIR)/%StegoExtract.o: $(SRCDIR)/%StegoExtract.c $(INCDIR)/%image.h
#	$(CC) $(CFLAGS) -c $(SRCDIR)/StegoExtract.c

#$(OBJDIR)/Stego.o :$(OBJDIR)/%Stego.o: $(SRCDIR)/%Stego.c $(INCDIR)/%image.h
#	$(CC) $(CFLAGS) -c $(SRCDIR)/Stego.c

#$(OBJDIR)/image.o: $(OBJDIR)/%image.o: $(SRCDIR)/%image.c $(INCDIR)/%image.h
#	$(CC) $(CFLAGS) -c $(SRCDIR)/image.c
#--------------------------------------------------------------------
# This clean target will remove all the object files, but
# not the executable
#--------------------------------------------------------------------
clean:
	rm -f *.o

#--------------------------------------------------------------------
# Add a target below named cleanall that will remove the object files
# as well as the executable
#--------------------------------------------------------------------
cleanall:
	rm -f *.o
	rm -f StegoExtract
	rm -f Stego

