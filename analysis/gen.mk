
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


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=

# including the module.mk file
-include $(patsubst %, %/module.mk,exe)

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,exe/%,$(FILES))

# appending the values found in DICTFILES to DICTH_modulename
DICTH_exe := $(foreach i, $(patsubst %,exe/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh))
# if dict header files exist, append to variable SRC
ifneq ($(DICTH_exe),)
SRC += exe/$(PKGNAME)_dict_exe
endif

PROG += $(patsubst %,$(BINDIR)/%,$(PROGRAMS))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,exe/%,$(PROGRAMS))

# VPATH += :exe

# make sure the output directories are there
__dummy := $(shell mkdir -p $(DEPDIR)/exe $(OBJDIR)/exe)

# a couple of rules to copy executable files correctly
$(BINDIR)/%: exe/%
	cp $^ $@

#$(BINDIR)/%: exe/%.bin
#	cp $^ $@

$(BINDIR)/%: exe/bin/%
	cp $^ $@

$(BINDIR)/%: ${OBJDIR}/exe/%.bin
	cp $^ $@


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=

# including the module.mk file
-include $(patsubst %, %/module.mk,src)

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,src/%,$(FILES))

# appending the values found in DICTFILES to DICTH_modulename
DICTH_src := $(foreach i, $(patsubst %,src/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh))
# if dict header files exist, append to variable SRC
ifneq ($(DICTH_src),)
SRC += src/$(PKGNAME)_dict_src
endif

PROG += $(patsubst %,$(BINDIR)/%,$(PROGRAMS))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,src/%,$(PROGRAMS))

# VPATH += :src

# make sure the output directories are there
__dummy := $(shell mkdir -p $(DEPDIR)/src $(OBJDIR)/src)

# a couple of rules to copy executable files correctly
$(BINDIR)/%: src/%
	cp $^ $@

#$(BINDIR)/%: src/%.bin
#	cp $^ $@

$(BINDIR)/%: src/bin/%
	cp $^ $@

$(BINDIR)/%: ${OBJDIR}/src/%.bin
	cp $^ $@


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=

# including the module.mk file
-include $(patsubst %, %/module.mk,tools)

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,tools/%,$(FILES))

# appending the values found in DICTFILES to DICTH_modulename
DICTH_tools := $(foreach i, $(patsubst %,tools/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh))
# if dict header files exist, append to variable SRC
ifneq ($(DICTH_tools),)
SRC += tools/$(PKGNAME)_dict_tools
endif

PROG += $(patsubst %,$(BINDIR)/%,$(PROGRAMS))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,tools/%,$(PROGRAMS))

# VPATH += :tools

# make sure the output directories are there
__dummy := $(shell mkdir -p $(DEPDIR)/tools $(OBJDIR)/tools)

# a couple of rules to copy executable files correctly
$(BINDIR)/%: tools/%
	cp $^ $@

#$(BINDIR)/%: tools/%.bin
#	cp $^ $@

$(BINDIR)/%: tools/bin/%
	cp $^ $@

$(BINDIR)/%: ${OBJDIR}/tools/%.bin
	cp $^ $@


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=

# including the module.mk file
-include $(patsubst %, %/module.mk,utils)

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,utils/%,$(FILES))

# appending the values found in DICTFILES to DICTH_modulename
DICTH_utils := $(foreach i, $(patsubst %,utils/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh))
# if dict header files exist, append to variable SRC
ifneq ($(DICTH_utils),)
SRC += utils/$(PKGNAME)_dict_utils
endif

PROG += $(patsubst %,$(BINDIR)/%,$(PROGRAMS))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,utils/%,$(PROGRAMS))

# VPATH += :utils

# make sure the output directories are there
__dummy := $(shell mkdir -p $(DEPDIR)/utils $(OBJDIR)/utils)

# a couple of rules to copy executable files correctly
$(BINDIR)/%: utils/%
	cp $^ $@

#$(BINDIR)/%: utils/%.bin
#	cp $^ $@

$(BINDIR)/%: utils/bin/%
	cp $^ $@

$(BINDIR)/%: ${OBJDIR}/utils/%.bin
	cp $^ $@

