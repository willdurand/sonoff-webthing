# Sonoff Basic
BOARD = generic
FLASH_MODE = dout
FLASH_DEF = 1M
LWIP_VARIANT = hb2n

# These variables should likely not be modified.
PROJECT_NAME = sonoff-webthing
SRC_DIR = ./sonoff
BUILD_DIR = ./build
DOCS_DIR = ./docs
SKETCH = $(SRC_DIR)/sonoff.ino
DEFAULT_GOAL = release
MAIN_NAME = sonoff-webthing
SIGNED_BIN_NAME = "$(MAIN_NAME)-signed.bin"
SIGNED_BIN = "$(BUILD_DIR)/$(SIGNED_BIN_NAME)"

# Build arguments
GIT_VERSION = $(shell git describe --tags --always --dirty 2>/dev/null || echo 'unknown')
BUILD_EXTRA_FLAGS += -DGIT_VERSION="\"$(GIT_VERSION)\""
BUILD_EXTRA_FLAGS += -DPROJECT_NAME="\"$(PROJECT_NAME)\""
BUILD_EXTRA_FLAGS += -DSIGNED_BIN_NAME="\"$(SIGNED_BIN_NAME)\""

include makeEspArduino.mk

# Add signing stuff to the regular build.
SIGNING_PY = "$(TOOLS_ROOT)/signing.py"
release: $(BUILD_DIR) $(ARDUINO_MK) $(BUILD_INFO_H) prebuild $(MAIN_EXE)
	$(SIGNING_PY) --mode sign \
		--privatekey "$(SRC_DIR)/private.key" \
		--bin $(MAIN_EXE) \
		--out "$(SIGNED_BIN)"
.PHONY: release

debug: BUILD_EXTRA_FLAGS += -DDEBUG
debug: release
.PHONY: debug

publish:
	$(eval SHA := $(shell sha256sum $(SIGNED_BIN) | cut -d ' ' -f 1))
	$(eval SIZE := $(shell ls -lh $(SIGNED_BIN) | cut -d ' ' -f 5))
	@cp $(SIGNED_BIN) $(DOCS_DIR)
	@sed -ri 's/^(fw_size:\s*)(.+)$$/\1$(SIZE)/' $(DOCS_DIR)/index.md
	@sed -ri 's/^(fw_sha256sum:\s*)(.+)$$/\1$(SHA)/' $(DOCS_DIR)/index.md
.PHONY: publish
