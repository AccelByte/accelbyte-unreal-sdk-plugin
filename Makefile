# Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

NAME=JusticeUE4Plugin
UE_VER=UE_4.16

ifndef UE_ROOT
$(error UE_ROOT is not set)
endif

BATCH_DIR=$(UE_ROOT)/$(UE_VER)/Engine/Build/BatchFiles/Mac
BIN_DIR=$(UE_ROOT)/$(UE_VER)/Engine/Binaries

UBT=/bin/sh $(BATCH_DIR)/RunMono.sh $(BIN_DIR)/DotNET/UnrealBuildTool.exe
GENERATE=/bin/sh $(BATCH_DIR)/GenerateProjectFiles.sh
PIE=$(BIN_DIR)/Mac/UE4Editor.app/Contents/MacOS/UE4Editor

build: generate
	$(UBT) $(NAME) Development Mac -project=$(PWD)/JusticeUE4Plugin.uproject -editorrecompile -progress -NoHotReloadFromIDE

generate:
	cd $(BATCH_DIR) && $(GENERATE) -project=$(PWD)/$(NAME).uproject -game

run:
	$(PIE) $(PWD)/$(NAME).uproject

_rebuild:
	$(MAKE) clean 
	$(NAME) build
#	$(UBT) $(NAME) Development Mac -project=$(PWD)/JusticeUE4Plugin.uproject -editorrecompile -progress -NoHotReloadFromIDE
#	$(MAKE) run

autorebuild:
	@watchman-make -p 'Plugins/OnlineSubsystemJustice/Source/**/*.cpp' 'Source/JusticeUE4Plugin/**/*.cpp' -t _rebuild --root . --settle 5

clean:
	rm -fr Intermediate/ Binaries/

