core/$(PKGNAME)_dict_core.cc: $(DICTH_core)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c $(filter -I% -D%,$(CXXFLAGS)) $^
