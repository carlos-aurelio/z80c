MD ?= mkdir -p
GREP ?= grep
SED ?= sed
SORT ?= sort
ECHO ?= echo -e

LOGLEVEL ?= 0

CFLAGS += -O0 -g3 -Wall -Wextra -Iinclude -DLOGLEVEL=$(LOGLEVEL)

INSTR_HEADER ?= source/core/z80_instructions.h

SRCDIR = source
BUILDDIR = .build
CORE_SRC = $(wildcard $(SRCDIR)/core/*.c)
CORE_OBJ = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/obj/%.o,$(CORE_SRC))

.PHONY: default all core_test zdos core_lib builddir clean header

default: zdos

all: core_test zdos core_lib

core_test: $(BUILDDIR)/core_test

zdos: $(BUILDDIR)/zdos

core_lib: $(BUILDDIR)/z80.so

header: $(INSTR_HEADER)

builddir: $(BUILDDIR)/obj/core

clean:
	$(RM) -r $(BUILDDIR)

$(BUILDDIR)/obj/core:
	$(MD) $@

$(INSTR_HEADER): $(CORE_SRC)
	$(ECHO) "/*** AUTO-GENERATED FILE ***/\n" > $(INSTR_HEADER)
	$(ECHO) "#ifndef __Z80_INSTRUCTIONS_H__" >> $(INSTR_HEADER)
	$(ECHO) "#define __Z80_INSTRUCTIONS_H__\n" >> $(INSTR_HEADER)
	$(ECHO) "#include \"z80_internal.h\"\n" >> $(INSTR_HEADER)
	$(ECHO) "/* http://wikiti.brandonw.net/?title=Z80_Instruction_Set */\n"\
		>> $(INSTR_HEADER)
	$(GREP) -e "^void z80_[a-zA-Z0-9_]\+(struct z80 \*z80)"\
		source/core/z80_*.c | tr -d '\r' | $(SED) "s/^.*:\(.*\)/\1;/" |\
		sort >> $(INSTR_HEADER)
	$(ECHO) "\n#endif /* __Z80_INSTRUCTIONS_H__ */" >> $(INSTR_HEADER)

$(BUILDDIR)/obj/%.o: $(SRCDIR)/%.c | header builddir
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILDDIR)/z80.so: $(CORE_OBJ)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/core_test: $(BUILDDIR)/obj/core_test.o $(CORE_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/zdos: $(BUILDDIR)/obj/zdos.o $(CORE_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)