B = build

UTILS = init shell

all : ${UTILS}

UTCS_OPT ?= -Os -s

CFLAGS = -std=c99 -m32 -nostdlib -g ${UTCS_OPT} -Wall -mno-sse
CCFLAGS = -std=c++17 -fno-exceptions -fno-rtti -m32 -ffreestanding -nostdlib -g ${UTCS_OPT} -Wall -mno-sse

CFILES = $(wildcard *.c)
CCFILES = $(wildcard *.cc)
SFILES = $(wildcard *.S) $(wildcard *.s)

ALL_OFILES = $(subst .cc,.o,$(CCFILES)) $(subst .c,.o,$(CFILES)) $(subst .s,.o,$(subst .S,.o,$(SFILES)))

OFILES = ${addprefix $B/,${ALL_OFILES}}

# keep all files
.SECONDARY :

$B/%.o :  Makefile %.c
	@echo "compiling  $*.c"
	@mkdir -p build
	gcc -I. -c -MD -MF $B/$*.d -o $B/$*.o $(CFLAGS) $*.c

$B/%.o :  Makefile %.cc
	@echo "compiling  $*.cc"
	@mkdir -p build
	g++ -I. -c -MD $ -MF $B/$*.d -o $B/$*.o $(CCFLAGS) $*.cc

$B/%.o :  Makefile %.S
	@echo "assembling $*.S"
	@mkdir -p build
	gcc -I. -MD -MF $B/$*.d -o $B/$*.o -m32 -c $*.S

$B/%.o :  Makefile %.s
	@echo "assembling $*.s"
	@mkdir -p build
	gcc -I. -MD -MF $B/$*.d -o $B/$*.o -m32 -c $*.s

clean ::
	rm -rf build

-include $B/*.d

$(UTILS) : Makefile script.ld $(OFILES)
#	ld -N -m elf_i386 -e start -Ttext=0x80000000 -o $@ $B/$@.o $(shell for d in `echo "$(OFILES)"`; do nm --defined-only "$$d" | grep -q main || echo "$$d"; done)
	ld -N -m elf_i386 -e start -Tscript.ld -o $@ $B/$@.o $(shell for d in `echo "$(OFILES)"`; do nm --defined-only "$$d" | grep -q main || echo "$$d"; done)
