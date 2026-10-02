.PHONY: help test build tidy fmt clean coverage-report compile-analysis compile-check compile resources newgame tutorial tutorial-reject

DIST_DIR    := dist
CLI_BIN     := $(DIST_DIR)/stars-asm
COVER_DIR   := $(DIST_DIR)/coverage
COVER_BIN   := $(COVER_DIR)/stars-cov
COVER_OUT   := $(COVER_DIR)/cover.out
COVER_HTML  := $(COVER_DIR)/coverage.html
MINGW_CC    ?= x86_64-w64-mingw32-gcc
MINGW_RC    ?= x86_64-w64-mingw32-windres
SRC_DIR     := decompiled
FILES       ?= $(wildcard $(SRC_DIR)/*.c)

help:
	@echo "Targets:"
	@echo "  run              Run cli"
	@echo "  test             Run all tests"
	@echo "  build            Build the CLI binary into ./dist/"
	@echo "  coverage-report  Run 'dasm all' and open HTML coverage report"
	@echo "  compile-analysis Generate non-failing MinGW C and resource diagnostics"
	@echo "  compile-check    Generate MinGW diagnostics and fail on C or resource errors"
	@echo "  compile          Print MinGW diagnostics to the terminal (FILES=decompiled/ai.c to limit)"
	@echo "  resources        Compile decompiled/res/stars.rc into ./dist/stars_res.o"
	@echo "  tutorial         Run the complete AutoHotkey v2 tutorial under Wine"
	@echo "  tutorial-reject  Verify early Generate is rejected"
	@echo "  newgame          Run the scaffold new-game smoke test under Wine (DEF=)"
	@echo "  tidy             Run go mod tidy in both modules"
	@echo "  fmt              Run go fmt in all modules"
	@echo "  clean            Remove ./dist/"

all: test dasm-all compile-analysis

dasm-all:
	go run main.go dasm all --all

test:
	go test ./...

build:
	mkdir -p $(DIST_DIR)
	go build -o ./$(CLI_BIN) .

tidy: 
	go mod tidy

fmt: 
	go fmt ./...

compile-analysis:
	go run ./tools/compile-analysis -cc "$(MINGW_CC)" -windres "$(MINGW_RC)" -out decompiled/compile-analysis.json decompiled

compile-check:
	go run ./tools/compile-analysis -fail-on-errors -cc "$(MINGW_CC)" -windres "$(MINGW_RC)" -out decompiled/compile-analysis.json decompiled

# print MinGW syntax diagnostics to the terminal, using the same flags as compile-analysis
compile:
	@for f in $(FILES); do \
		$(MINGW_CC) -std=gnu11 -fsyntax-only -fdiagnostics-color=always -fmax-errors=0 -Wno-pointer-sign -I$(SRC_DIR) $$f; \
	done; true
	@cd $(SRC_DIR)/res && $(MINGW_RC) stars.rc -O res -o /dev/null; true

# compile the resource script into an object to link with the decompiled C;
# the script names the files it includes relative to its own directory
resources:
	@mkdir -p $(DIST_DIR)
	cd $(SRC_DIR)/res && $(MINGW_RC) stars.rc -O coff -o $(CURDIR)/$(DIST_DIR)/stars_res.o

# dump a coverage report to identify ai generated slop that is not actually being used
coverage-report:
	@mkdir -p $(COVER_DIR)
	@echo "Building coverage-instrumented binary..."
	go build -cover -coverpkg=all -o $(COVER_BIN) .
	@echo "Running 'cli'..."
	@rm -f $(COVER_DIR)/covcounters.* $(COVER_DIR)/covmeta.*
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) dasm asm -f NthValidShdef --debug > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) dasm effects -f NthValidShdef --debug > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) dasm cfg -f NthValidShdef --debug > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) dasm sem -f NthValidShdef --debug > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) dasm ir -f NthValidShdef --debug > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) dasm graph -f NthValidShdef --debug > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) dasm all --all > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) ne segments > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) ne fixups --seg "000a" > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) ne string --seg "0x25" --off "0x202" > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) symbols globals > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) symbols functions > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) symbols structs > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) symbols publics > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) symbols enums > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) nb09 alignsyms > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) nb09 directories > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) nb09 fileindices > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) nb09 globalpubs > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) nb09 globalsyms > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) nb09 globaltypes > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) nb09 libraries > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) nb09 modules > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) nb09 segments > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) nb09 srcmodules > /dev/null
	GOCOVERDIR=$(COVER_DIR) $(COVER_BIN) nb09 staticsyms > /dev/null
	@echo "Converting coverage data..."
	go tool covdata textfmt -i=$(COVER_DIR) -o=$(COVER_OUT)
	@head -1 $(COVER_OUT) > $(COVER_DIR)/dasm-only.out
	@grep "^github.com/sirgwain/stars-asm/dasm" $(COVER_OUT) >> $(COVER_DIR)/dasm-only.out
	go tool cover -html=$(COVER_DIR)/dasm-only.out -o $(COVER_HTML)
	@echo ""
	@go tool cover -func=$(COVER_DIR)/dasm-only.out | tail -1
	@echo "Report: $(COVER_HTML)"
	open $(COVER_HTML)

clean:
	rm -rf $(DIST_DIR)

# generate a new game with the decompiled stars.exe under Wine and check the
# game files were written
DEF ?= tests/scaffold/fixtures/newgame/tiny/game.def
newgame:
	tests/scaffold/newgame.sh "$(DEF)"

# generate new starsbox checkpoints (takes a LONG time)
checkpoints-starsbox:
	rm -rf starsbox/c_drive/REGTEST
	python3 tests/scaffold/regression.py prepare --engine dosbox --seed 12345 --exe starsbox/c_drive/STARS/stars.exe --work starsbox/c_drive/REGTEST
	python3 tests/scaffold/regression.py run --work starsbox/c_drive/REGTEST

# generate new native checkpoints
checkpoints-native:
	rm -rf starsbox/c_drive/native 
	cmake --preset mingw-debug -B dist/regression-build -DSTARS_TEST_SEED=12345
	cmake --build dist/regression-build
	make build

	python3 tests/scaffold/regression.py prepare --engine native --seed 12345 --exe dist/regression-build/bin/stars.exe --work starsbox/c_drive/native
	python3 tests/scaffold/regression.py run --work starsbox/c_drive/native

checkpoints-compare:
	python3 tests/scaffold/regression.py compare starsbox/c_drive/REGTEST starsbox/c_drive/native

# STARS_TUTORIAL_SERIAL optionally overrides the runner's default serial.
tutorial:
	python3 tests/scaffold/tutorial/run.py --download-ahk $(TUTORIAL_ARGS)

tutorial-reject:
	python3 tests/scaffold/tutorial/run.py --download-ahk --scenario reject-generate $(TUTORIAL_ARGS)
