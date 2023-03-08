module CRam #(
  parameter  int unsigned row       = 128,
  parameter  int unsigned col       = 256,
  parameter  int unsigned wordSize  = 32,
  localparam int unsigned wordNum   = (row * col) / wordSize,
  localparam int unsigned addrWidth = $clog2(wordNum)
) (
  input  logic                 clk_i,
  input  logic                 rst_ni,
  input  logic [         31:0] inst_i,
  input  logic                 we_i,
  input  logic [addrWidth-1:0] addr_i,
  input  logic [ wordSize-1:0] data_i,
  output logic [ wordSize-1:0] data_o
);

  localparam int unsigned dpiDataSize = 32;

  import "DPI-C" function void set_norm_mem_ptr(input logic [wordSize-1:0] mem []);
  import "DPI-C" function void set_norm_mem_init(output bit [wordSize-1:0] mem [0:wordNum-1]);
  import "DPI-C" function void set_carry_ptr(input logic [dpiDataSize-1:0] carry []);
  import "DPI-C" function void set_tag_ptr(input logic [dpiDataSize-1:0] tag []);

  bit   [wordSize-1:0] norm_mem_init[0:wordNum-1];
  logic [     row-1:0] cram_mem_init[0:    col-1];

  logic [dpiDataSize-1:0] dpi_carry [0:row-1];
  logic [dpiDataSize-1:0] dpi_tag   [0:row-1];

  initial begin
    set_norm_mem_ptr(norm_mem_q);
    set_carry_ptr(dpi_carry);
    set_tag_ptr(dpi_tag);
  end

  always_comb begin
    set_norm_mem_init(norm_mem_init);
  end

  always_comb begin
    for (int i = 0; i < col; i++) begin
      for (int j = 0; j < row; j++) begin
        cram_mem_init[i][j] = norm_mem_init[j+row*(i/wordSize)][(wordSize-1)-(i%wordSize)];
      end
    end
  end

  always_comb begin
    for (int i = 0; i < row; i++) begin
      dpi_carry[i] = {{(dpiDataSize-1){1'b0}}, op_carry_q[i]};
      dpi_tag[i]   = {{(dpiDataSize-1){1'b0}}, op_tag_q[i]};
    end
  end

  typedef enum logic [3:0] {
    // logic
    OPCODE_AND,
    OPCODE_OR,
    OPCODE_XOR,
    OPCODE_NAND,
    OPCODE_NOR,
    OPCODE_XNOR,
    // arithmetic
    OPCODE_ADD,
    // shift
    OPCODE_CPY,
    OPCODE_INV,
    // comparison
    OPCODE_EQU,
    // utility
    OPCODE_LDT,
    OPCODE_STC,
    OPCODE_STT,
    OPCODE_SC,
    OPCODE_CC,
    OPCODE_C2T  = 4'hf
  } OpType;

  // vertical: normal sram mode
  logic [wordSize-1:0] norm_mem_d[0:wordNum-1];
  logic [wordSize-1:0] norm_mem_q[0:wordNum-1];

  always_comb begin
    for (int i = 0; i < col; i++) begin
      for (int j = 0; j < row; j++) begin
        norm_mem_q[j+row*(i/wordSize)][(wordSize-1)-(i%wordSize)] = cram_mem_q[i][j];
      end
    end
  end

  always_comb begin
    for (int i = 0; i < col; i++) begin
      for (int j = 0; j < row; j++) begin
        write_to_cram[i][j] = norm_mem_d[j+row*(i/wordSize)][(wordSize-1)-(i%wordSize)];
      end
    end
  end

  // normal sram read/write
  always_comb begin
    for (int i = 0; i < wordNum; i++) begin
      for (int j = 0; j < wordSize; j++) begin
        if ((we_i == 1'b1) && (i == addr_i)) begin
          norm_mem_d[i][j] = data_i[j];
        end else begin
          norm_mem_d[i][j] = norm_mem_q[i][j];
        end
      end
    end
  end

  assign data_o = norm_mem_q[addr_i];

  // horizontal: CRam mode
  logic [row-1:0] cram_mem_d[0:col-1];
  logic [row-1:0] cram_mem_q[0:col-1];

  logic [row-1:0] write_to_cram[0:col-1];

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      cram_mem_q <= cram_mem_init;
    end else begin
      cram_mem_q <= cram_mem_d;
    end
  end

  logic [31:0] inst_d;
  logic [31:0] inst_q;

  assign inst_d = inst_i;

  // instruction register
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      inst_q <= {32{1'b0}};
    end else begin
      inst_q <= inst_d;
    end
  end

  // instruction
  logic [3:0] enable = inst_q[31:28];
  logic [3:0] opcode = inst_q[27:24];
  logic [7:0] addr_a = inst_q[23:16];
  logic [7:0] addr_b = inst_q[15: 8];
  logic [7:0] addr_d = inst_q[ 7: 0];

  // features
  logic instr_enable, tag_enable;

  assign instr_enable = enable[3] & (~we_i);
  assign tag_enable   = enable[0];
  // TODO: other features?

  // OpType enum
  OpType op = OpType'(opcode);

  logic [row-1:0] op_result_d, op_result_q;
  logic [row-1:0] op_carry_d, op_carry_q;
  logic [row-1:0] op_tag_d, op_tag_q;

  // debug signal
  logic [row-1:0] debug_operand_a, debug_operand_b, debug_result;
  assign debug_operand_a = cram_mem_q[addr_a];
  assign debug_operand_b = cram_mem_q[addr_b];
  assign debug_result    = cram_mem_q[addr_d];

  // op with rd
  always_comb begin
    unique case (op)
      OPCODE_AND:  op_result_d =   cram_mem_q[addr_a] & cram_mem_q[addr_b];
      OPCODE_OR:   op_result_d =   cram_mem_q[addr_a] | cram_mem_q[addr_b];
      OPCODE_XOR:  op_result_d =   cram_mem_q[addr_a] ^ cram_mem_q[addr_b];
      OPCODE_NAND: op_result_d = ~(cram_mem_q[addr_a] & cram_mem_q[addr_b]);
      OPCODE_NOR:  op_result_d = ~(cram_mem_q[addr_a] | cram_mem_q[addr_b]);
      OPCODE_XNOR: op_result_d = ~(cram_mem_q[addr_a] ^ cram_mem_q[addr_b]);
      OPCODE_ADD:  op_result_d =   cram_mem_q[addr_a] ^ cram_mem_q[addr_b] ^ op_carry_q;
      OPCODE_CPY:  op_result_d =   cram_mem_q[addr_a];
      OPCODE_INV:  op_result_d =  ~cram_mem_q[addr_a];
      OPCODE_STC:  op_result_d = op_carry_d;
      OPCODE_STT:  op_result_d = op_tag_d;
      default:     op_result_d = op_result_q;
    endcase
  end

  // op with carry
  always_comb begin
    unique case (op)
      OPCODE_ADD: op_carry_d = ((cram_mem_q[addr_a] ^ cram_mem_q[addr_b]) & op_carry_q)
                              | (cram_mem_q[addr_a] & cram_mem_q[addr_b]);
      OPCODE_SC:  op_carry_d = {row{1'b1}};
      OPCODE_CC:  op_carry_d = {row{1'b0}};
      default:    op_carry_d = op_carry_q;
    endcase
  end

  // op with tag
  always_comb begin
    unique case (op)
      OPCODE_EQU: op_tag_d = ~(cram_mem_q[addr_a] ^ {row{addr_b[0]}});
      OPCODE_LDT: op_tag_d = cram_mem_q[addr_a];
      OPCODE_C2T: op_tag_d = op_carry_q;
      default:    op_tag_d = op_tag_q;
    endcase
  end

  // update at negedge of clk_i
  always_ff @(negedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      op_result_q <= {row{1'b0}};
      op_carry_q  <= {row{1'b0}};
      op_tag_q    <= {row{1'b0}};
    end else begin
      op_result_q <= (instr_enable == 1'b1) ? op_result_d : op_result_q;
      op_carry_q  <= (instr_enable == 1'b1) ? op_carry_d  : op_carry_q;
      op_tag_q    <= (instr_enable == 1'b1) ? op_tag_d    : op_tag_q;
    end
  end

  logic op_wb_enable;

  // op with write back
  always_comb begin
    unique case (op)
      OPCODE_AND,
      OPCODE_OR,
      OPCODE_XOR,
      OPCODE_NAND,
      OPCODE_NOR,
      OPCODE_XNOR,
      OPCODE_ADD,
      OPCODE_CPY,
      OPCODE_INV,
      OPCODE_STC,
      OPCODE_STT: op_wb_enable = 1'b1;
      default:    op_wb_enable = 1'b0;
    endcase
  end

  logic [row-1:0] result_wb_enable;

  // conditional write back to cram
  always_comb begin
    for (int i = 0; i < row; i++) begin
      if (tag_enable == 1'b0) begin
        result_wb_enable[i] = op_wb_enable;
      end else begin
        result_wb_enable[i] = op_tag_q[i];
      end
    end
  end

  always_comb begin
    for (int i = 0; i < col; i++) begin
      for (int j = 0; j < row; j++) begin
        if (instr_enable == 1'b0) begin
          cram_mem_d[i][j] = write_to_cram[i][j];
        end else if ((i == addr_d) && (result_wb_enable[j] == 1'b1)) begin
          cram_mem_d[i][j] = op_result_q[j];
        end else begin
          cram_mem_d[i][j] = cram_mem_q[i][j];
        end
      end
    end
  end

endmodule
