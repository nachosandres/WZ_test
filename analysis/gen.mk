
# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=

# including the module.mk file
-include $(patsubst %, %/module.mk,analyzers)

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,analyzers/%,$(FILES))

# appending the values found in DICTFILES to DICTH_modulename
DICTH_analyzers := $(foreach i, $(patsubst %,analyzers/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh))
# if dict header files exist, append to variable SRC
ifneq ($(DICTH_analyzers),)
SRC += analyzers/$(PKGNAME)_dict_analyzers
endif

PROG += $(patsubst %,$(BINDIR)/%,$(PROGRAMS))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,analyzers/%,$(PROGRAMS))

# VPATH += :analyzers

# make sure the output directories are there
__dummy := $(shell mkdir -p $(DEPDIR)/analyzers $(OBJDIR)/analyzers)

# a couple of rules to copy executable files correctly
$(BINDIR)/%: analyzers/%
	cp $^ $@

#$(BINDIR)/%: analyzers/%.bin
#	cp $^ $@

$(BINDIR)/%: analyzers/bin/%
	cp $^ $@

$(BINDIR)/%: ${OBJDIR}/analyzers/%.bin
	cp $^ $@


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
-include $(patsubst %, %/module.mk,general)

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,general/%,$(FILES))

# appending the values found in DICTFILES to DICTH_modulename
DICTH_general := $(foreach i, $(patsubst %,general/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh))
# if dict header files exist, append to variable SRC
ifneq ($(DICTH_general),)
SRC += general/$(PKGNAME)_dict_general
endif

PROG += $(patsubst %,$(BINDIR)/%,$(PROGRAMS))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,general/%,$(PROGRAMS))

# VPATH += :general

# make sure the output directories are there
__dummy := $(shell mkdir -p $(DEPDIR)/general $(OBJDIR)/general)

# a couple of rules to copy executable files correctly
$(BINDIR)/%: general/%
	cp $^ $@

#$(BINDIR)/%: general/%.bin
#	cp $^ $@

$(BINDIR)/%: general/bin/%
	cp $^ $@

$(BINDIR)/%: ${OBJDIR}/general/%.bin
	cp $^ $@


# module.mk appends to FILES and DICTFILES
FILES :=
DICTFILES :=

# including the module.mk file
-include $(patsubst %, %/module.mk,helper)

# appending the values found in FILES to the variable SRC
SRC += $(patsubst %,helper/%,$(FILES))

# appending the values found in DICTFILES to DICTH_modulename
DICTH_helper := $(foreach i, $(patsubst %,helper/%,$(DICTFILES)), $(wildcard $(i).h) $(wildcard $(i).hh))
# if dict header files exist, append to variable SRC
ifneq ($(DICTH_helper),)
SRC += helper/$(PKGNAME)_dict_helper
endif

PROG += $(patsubst %,$(BINDIR)/%,$(PROGRAMS))

# appending the values found in FILES to the variable SRC
PROGSRC += $(patsubst %,helper/%,$(PROGRAMS))

# VPATH += :helper

# make sure the output directories are there
__dummy := $(shell mkdir -p $(DEPDIR)/helper $(OBJDIR)/helper)

# a couple of rules to copy executable files correctly
$(BINDIR)/%: helper/%
	cp $^ $@

#$(BINDIR)/%: helper/%.bin
#	cp $^ $@

$(BINDIR)/%: helper/bin/%
	cp $^ $@

$(BINDIR)/%: ${OBJDIR}/helper/%.bin
	cp $^ $@

