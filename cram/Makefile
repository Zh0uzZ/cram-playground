ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
else
export VERILATOR_ROOT
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
endif

TARGET ?= main

TOP_MODULE ?= CRam

INC_DIR   = include
SRC_DIR   = src
RTL_DIR   = rtl
BUILD_DIR = build

VCD = $(BUILD_DIR)/$(TARGET).vcd
EXE = $(BUILD_DIR)/$(TARGET)

sim: $(VCD) $(EXE)

VERILATOR_FLAGS += -cc --exe -sv --build
VERILATOR_FLAGS += -Wall -Wno-lint
VERILATOR_FLAGS += --Mdir $(BUILD_DIR)

INCFLAGS = $(addprefix -I, $(abspath $(INC_DIR)))
CFLAGS  += $(INCFLAGS)
LDFLAGS ?=

$(VCD): $(EXE)
	@$^

$(EXE): $(RTL_DIR)/$(TOP_MODULE).sv $(SRC_DIR)/*.cc
	$(VERILATOR) $(VERILATOR_FLAGS) --trace \
		$(addprefix -CFLAGS , $(CFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
		-o $(abspath $@) --top $(TOP_MODULE) $^

clean:
	-rm -rf $(BUILD_DIR)

.PHONY: sim clean
