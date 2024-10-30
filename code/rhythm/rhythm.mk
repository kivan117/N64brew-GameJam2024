ASSETS_LIST += \
	filesystem/rhythm/indicator.sprite \
	filesystem/rhythm/tabloid_junkie.wav64 \
	filesystem/rhythm/tabloid_junkie.csm \
	filesystem/rhythm/privacy.wav64 \
	filesystem/rhythm/privacy.csm \
	filesystem/rhythm/breaking_news.wav64 \
	filesystem/rhythm/breaking_news.csm \
	filesystem/rhythm/screenbreaker.wav64 \
	filesystem/rhythm/screenbreaker.csm


$(FILESYSTEM_DIR)/%.csm: $(ASSETS_DIR)/%.csm
	@mkdir -p $(dir $@)
	@echo "    [CSM] $@"
	@cp $< $@
