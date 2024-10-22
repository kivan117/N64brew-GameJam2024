ASSETS_LIST += \
	filesystem/rhythm/tabloid_junkie.wav64 \
	filesystem/rhythm/tabloid_junkie.csm \
	filesystem/rhythm/indicator.sprite


$(FILESYSTEM_DIR)/%.csm: $(ASSETS_DIR)/%.csm
	@mkdir -p $(dir $@)
	@echo "    [CSM] $@"
	@cp $< $@