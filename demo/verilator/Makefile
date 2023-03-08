ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
else
export VERILATOR_ROOT
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
endif

TARGET_LIST ?= hw

INC_PATH ?=

CSRC_DIR ?= csrc
VSRC_DIR ?= vsrc
OBJ_DIR  ?= obj_dir

VERILATOR_FLAGS += -cc --exe -sv --build
VERILATOR_FLAGS += -Wall -Wno-lint
VERILATOR_FLAGS += --Mdir $(OBJ_DIR)

INCFLAGS = $(addprefix -I, $(INC_PATH))
CFLAGS  += $(INCFLAGS)

EXE ?= $(foreach target, $(TARGET_LIST), $(OBJ_DIR)/V$(target))

.PHONY: sim clean

sim: $(EXE)
	./$<

$(OBJ_DIR)/V%: $(VSRC_DIR)/%.v $(CSRC_DIR)/sim.cc
	$(VERILATOR) $(VERILATOR_FLAGS) --trace \
		-o $(abspath $@) --top $(*F) $^

clean:
	-rm -rf $(OBJ_DIR)
