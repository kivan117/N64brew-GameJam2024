ASSETS_LIST += \
	filesystem/rhythm/tabloid_junkie.wav64 \
	filesystem/rhythm/tabloid_junkie.csm \
	filesystem/rhythm/tabloid_junkie_layout.layout \
	filesystem/rhythm/privacy.wav64 \
	filesystem/rhythm/privacy.csm \
	filesystem/rhythm/privacy_layout.layout \
	filesystem/rhythm/indicator.sprite


$(FILESYSTEM_DIR)/%.csm: $(ASSETS_DIR)/%.csm
	@mkdir -p $(dir $@)
	@echo "    [CSM] $@"
	@cp $< $@

$(FILESYSTEM_DIR)/%.layout: $(ASSETS_DIR)/%.layout
	@mkdir -p $(dir $@)
	@echo "    [LAYOUT] $@"
	@cp $< $@