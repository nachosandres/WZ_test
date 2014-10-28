
# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=

# including the module.mk file
-include $(patsubst %, %/module.mk,core)

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,core/%,$(FILES))

# appending the values found in DICTFILES to DICTH_modulename
DICTH_core := $(foreach i, $(patsubst %,core/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh))
# if dict header files exist, append to variable SRC
ifneq ($(DICTH_core),)
SRC += core/$(PKGNAME)_dict_core
endif

PROG += $(patsubst %,$(BINDIR)/%,$(PROGRAMS))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,core/%,$(PROGRAMS))

# VPATH += :core

# make sure the output directories are there
__dummy := $(shell mkdir -p $(DEPDIR)/core $(OBJDIR)/core)

# a couple of rules to copy executable files correctly
$(BINDIR)/%: core/%
	cp $^ $@

#$(BINDIR)/%: core/%.bin
#	cp $^ $@

$(BINDIR)/%: core/bin/%
	cp $^ $@

$(BINDIR)/%: ${OBJDIR}/core/%.bin
	cp $^ $@

