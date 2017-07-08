# Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

NAME=JusticeUE4Plugin

UE_VER=UE_4.16
BATCH=$(UE_ROOT)/$(UE_VER)/Engine/Build/BatchFiles/Mac

UBT=/bin/sh $(BATCH)/RunMono.sh $(UE_ROOT)/$(UE_VER)/Engine/Binaries/DotNET/UnrealBuildTool.exe
GENERATE=/bin/sh $(BATCH)/GenerateProjectFiles.sh
EDITOR=$(UE_ROOT)/$(UE_VER)/Engine/Binaries/Mac/UE4Editor.app/Contents/MacOS/UE4Editor

build:
	cd $(BATCH) && $(GENERATE) -project=$(PWD)/$(NAME).uproject -game
	$(UBT) $(NAME) Development Mac -project=$(PWD)/JusticeUE4Plugin.uproject -editorrecompile -progress -NoHotReloadFromIDE

run:
	$(EDITOR) $(PWD)/$(NAME).uproject

_rebuild:
	$(MAKE) clean 
	$(UBT) $(NAME) Development Mac -project=$(PWD)/JusticeUE4Plugin.uproject -editorrecompile -progress -NoHotReloadFromIDE
	$(MAKE) run

autorebuild:
	@watchman-make -p 'Plugins/OnlineSubsystemJustice/Source/**/*.cpp' 'Source/JusticeUE4Plugin/**/*.cpp' -t _rebuild --root $(PWD) --settle 50000

clean:
	rm -r Intermediate/ Binaries/

