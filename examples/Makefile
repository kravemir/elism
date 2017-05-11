SOURCES := $(shell ls *.bp)
PROGRAMS := $(addprefix build/, $(SOURCES:%.bp=%))


ELISM_DIR := ../build

all: $(PROGRAMS) build/KnapsackTest.class build/KnapsackTest2.class

programs: $(PROGRAMS)

build/%: build/%.o
	clang $< $(ELISM_DIR)/librt_lib.a -o $@

build/%.o: %.bp $(ELISM_DIR)/elism | build
	$(ELISM_DIR)/elism -o $@ $<

build/%.class: %.java | build
	javac -d build $<

build:
	mkdir -p build

$(ELISM_DIR)/elism:
	mkdir -p $(ELISM_DIR)
	sh -c "cd $(ELISM_DIR); cmake ..; make -j4"

.PRECIOUS: build/%.o
