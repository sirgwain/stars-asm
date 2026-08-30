.PHONY: help test build tidy fmt clean coverage-report

DIST_DIR    := dist
CLI_BIN     := $(DIST_DIR)/stars-asm
COVER_DIR   := $(DIST_DIR)/coverage
COVER_BIN   := $(COVER_DIR)/stars-cov
COVER_OUT   := $(COVER_DIR)/cover.out
COVER_HTML  := $(COVER_DIR)/coverage.html

help:
	@echo "Targets:"
	@echo "  run              Run cli"
	@echo "  test             Run all tests"
	@echo "  build            Build the CLI binary into ./dist/"
	@echo "  coverage-report  Run 'dasm all' and open HTML coverage report"
	@echo "  tidy             Run go mod tidy in both modules"
	@echo "  fmt              Run go fmt in all modules"
	@echo "  clean            Remove ./dist/"

test:
	go test ./...

build:
	mkdir -p $(DIST_DIR)
	go build -o ./$(CLI_BIN) .

tidy: 
	go mod tidy

fmt: 
	go fmt ./...

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
