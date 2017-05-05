SOURCES := $(shell ls *.bp)
PROGRAMS := $(addprefix build/, $(SOURCES:%.bp=%))


ELISM_DIR := ../build

all: $(PROGRAMS) build/KnapsackTest.class build/KnapsackTest2.class

build/%: build/%.o
	clang $< $(ELISM_DIR)/librt_lib.a -o $@

build/%.o: %.bp $(ELISM_DIR)/bp_jit | build
	$(ELISM_DIR)/bp_jit -o $@ $<

build/%.class: %.java | build
	javac -d build $<

build:
	mkdir -p build

$(ELISM_DIR)/bp_jit:
	mkdir -p $(ELISM_DIR)
	sh -c "cd $(ELISM_DIR); cmake ..; make -j4"

.PRECIOUS: build/%.o
