CC = g++
AR = ar
RM = rm -rf
MKDIR = mkdir -p
CP = cp
OUTDIR = lib
OBJDIR = obj
DIRS = $(OUTDIR) $(OBJDIR)
SUBDIR = examples
LIBDIR = /usr/lib
SOURCES = LM15SGFNZ07.cpp
_EXAMPLES = SmarThermo LM15SGFNZ07_functions_example LM15SGFNZ07_demo
EXAMPLES = $(patsubst %,$(SUBDIR)/%,$(_EXAMPLES))

all: libLM15SGFNZ07.a libLM15SGFNZ07.so $(EXAMPLES)

.PHONY: $(EXAMPLES)

$(EXAMPLES):
	$(MAKE) -C $@


libLM15SGFNZ07.a: LM15SGFNZ07_static.o
	$(AR) rvs $(OUTDIR)/$@ $(OBJDIR)/$^

libLM15SGFNZ07.so: LM15SGFNZ07_shared.o
	$(CC) -shared -o $(OUTDIR)/$@ $(OBJDIR)/$^

LM15SGFNZ07_static.o: $(DIRS)
	$(CC) -c $(SOURCES) -o $(OBJDIR)/$@	

LM15SGFNZ07_shared.o: $(DIRS)
	$(CC) -fpic -c $(SOURCES) -o $(OBJDIR)/$@

$(DIRS):
	$(MKDIR) $@

install:
	$(CP) $(OUTDIR)/libLM15SGFNZ07.so $(LIBDIR)
	$(CP) $(OUTDIR)/libLM15SGFNZ07.a $(LIBDIR)
	for d in $(EXAMPLES); do ( $(MAKE) -C $$d install ); done

uninstall:
	for d in $(EXAMPLES); do ( $(MAKE) -C $$d uninstall ); done
	-$(RM) $(LIBDIR)/libLM15SGFNZ07.so
	-$(RM) $(LIBDIR)/libLM15SGFNZ07.a

reinstall: uninstall install

clean:
	-$(RM) $(DIRS)
	-for d in $(EXAMPLES); do ( $(MAKE) -C $$d clean ); done
