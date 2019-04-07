CC = g++
RM = rm -rf
MKDIR = mkdir -p
CP = cp
SYSTEMCTL = systemctl
LIBDIR = ../../lib
INCDIR = ../../
OBJDIR = obj
OUTDIR = out
BINDIR = /usr/bin
SYSDIR = /etc/systemd/system
DIRS = $(OBJDIR) $(OUTDIR)
CFLAGS = -I$(INCDIR)
LDFLAGS = -L$(LIBDIR) -lLM15SGFNZ07 -lcurl -ljansson -lwiringPi -lrt -lpng
TARGET = SmarThermo
_OBJ = Buffer.o OWM.o OneWire.o SmarThermo.o WebService.o Hazyair.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

all: $(OUTDIR)/$(TARGET)

$(OUTDIR)/$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp $(DIRS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(DIRS):
	$(MKDIR) $@

install:
	$(CP) $(OUTDIR)/$(TARGET) $(BINDIR)
	$(CP) $(TARGET).service $(SYSDIR)
	$(SYSTEMCTL) enable $(TARGET).service
	$(SYSTEMCTL) start $(TARGET).service
	$(SYSTEMCTL) status $(TARGET).service
	
uninstall:
	-$(SYSTEMCTL) stop $(TARGET).service
	-$(SYSTEMCTL) disable $(TARGET).service
	-$(RM) $(BINDIR)/$(TARGET)
	-$(RM) $(SYSDIR)/$(TARGET).service

reinstall: uninstall install
	
clean:
	-$(RM) $(DIRS)
	
