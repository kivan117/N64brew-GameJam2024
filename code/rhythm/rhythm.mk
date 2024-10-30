ASSETS_LIST += \
	filesystem/rhythm/tabloid_junkie.wav64 \
	filesystem/rhythm/tabloid_junkie.csm \
	filesystem/rhythm/privacy.wav64 \
	filesystem/rhythm/privacy.csm \
	filesystem/rhythm/breaking_news.wav64 \
	filesystem/rhythm/breaking_news.csm \
	filesystem/rhythm/indicator.sprite


$(FILESYSTEM_DIR)/%.csm: $(ASSETS_DIR)/%.csm
	@mkdir -p $(dir $@)
	@echo "    [CSM] $@"
	@cp $< $@
