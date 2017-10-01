CC = g++
RM = rm -rf
MKDIR = mkdir -p
CP = cp
LIBDIR = ../../lib
INCDIR = ../../
OBJDIR = obj
OUTDIR = out
BINDIR = /usr/bin
DIRS = $(OBJDIR) $(OUTDIR)
CFLAGS = -I$(INCDIR)
LDFLAGS = -L$(LIBDIR) -lLM15SGFNZ07 -lwiringPi
TARGET = LM15SGFNZ07_functions_example
OBJ = $(OBJDIR)/LM15SGFNZ07_functions_example.o

all: $(OUTDIR)/$(TARGET)

$(OUTDIR)/$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp $(DIRS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(DIRS):
	$(MKDIR) $@

install:
	$(CP) $(OUTDIR)/$(TARGET) $(BINDIR)
	
uninstall:
	-$(RM) $(BINDIR)/$(TARGET)

reinstall: uninstall install
	
clean:
	-$(RM) $(DIRS)