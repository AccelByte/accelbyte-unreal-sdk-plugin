# Copyright (c) 2017-2019 AccelByte Inc. All Rights Reserved.
# This is licensed software from AccelByte Inc, for limitations
# and restrictions contact your company contract manager.


ifeq ($(CUSTOMER),)
    SERVICE=justice-ue4-sdk
else
    SERVICE=$(CUSTOMER)-justice-ue4-sdk
endif

DEV_IMAGE=accelbyte/node:10.15.3-alpine-3.11

RUN=docker run --rm \
	-v $(CURDIR):/workspace/$(SERVICE) \
	-w /workspace/$(SERVICE)

commitHook:
	$(RUN) -i -t $(DEV_IMAGE) node_modules/.bin/git-cz --hook || true

setupCommitHook:
	echo "Setting up pre-commit hook to enforce conventional commit standard for commit messsages"
	$(RUN) $(DEV_IMAGE) yarn add commitizen cz-conventional-changelog
	$(RUN) $(DEV_IMAGE) rm -rf package.json
	$(RUN) $(DEV_IMAGE) echo "#\!/bin/bash" > .git/hooks/commit-msg
	$(RUN) $(DEV_IMAGE) echo "[ -t 1 ] && { exec < /dev/tty; make commitHook || true; } || echo "commit hook skipped"" >> .git/hooks/commit-msg
	$(RUN) $(DEV_IMAGE) chmod +x .git/hooks/commit-msg 2>&1 >/dev/null
	$(RUN) $(DEV_IMAGE) echo '{"config": {"commitizen": {"path": "cz-conventional-changelog"}}}' > package.json
	echo "Pre-commit hook set"