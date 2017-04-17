SOURCES := $(shell ls *.bp)
PROGRAMS := $(addprefix build/, $(SOURCES:%.bp=%))


ELISM_DIR := ../build

all: $(PROGRAMS)

build/%: build/%.o
	clang $< $(ELISM_DIR)/librt_lib.a -o $@

build/%.o: %.bp build $(ELISM_DIR)/bp_jit
	$(ELISM_DIR)/bp_jit -o $@ $<

build:
	mkdir -p build

$(ELISM_DIR)/bp_jit:
	mkdir -p $(ELISM_DIR)
	sh -c "cd $(ELISM_DIR); cmake ..; make -j4"