ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
else
export VERILATOR_ROOT
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
endif

TARGET ?= sim

TOP_MODULE ?= hw

INC_DIR   = csrc
SRC_DIR   = csrc
RTL_DIR   = vsrc
BUILD_DIR = build

EXE = $(BUILD_DIR)/$(TARGET)

sim: $(EXE)
	@$^

VERILATOR_FLAGS += --cc --build --exe
VERILATOR_FLAGS += -Wall -Wno-fatal
VERILATOR_FLAGS += --top $(TOP_MODULE)
VERILATOR_FLAGS += --Mdir $(BUILD_DIR)

INCFLAGS = $(addprefix -I, $(abspath $(INC_DIR)))
CFLAGS  += $(INCFLAGS)
LDFLAGS ?=

$(EXE): $(RTL_DIR)/$(TOP_MODULE).v $(SRC_DIR)/*.cc
	$(VERILATOR) $(VERILATOR_FLAGS) \
		$(addprefix -CFLAGS , $(CFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
		-o $(abspath $@) $^

clean:
	-rm -rf $(BUILD_DIR)

.PHONY: sim clean
