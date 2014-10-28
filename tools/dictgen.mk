src/$(PKGNAME)_dict_src.cc: $(DICTH_src)
	@echo "Generating dictionary $@..."
	@$(ROOTCINT) -f $@ -c $(filter -I% -D%,$(CXXFLAGS)) $^
#src/$(PKGNAME)_dict_src.o: $(DICTH_src)
#	@$(ROOTCINT) -c $(filter -I% -D%,$(CXXFLAGS)) $< -o src
