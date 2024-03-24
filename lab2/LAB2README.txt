To run this in model sim, use riscv_single.sv
riscv_single_fpga.sv includes the memstrobe and pcready functionality for fpga implementation that is not present in riscv_single.sv

As described in lecture 10, the subword read and subword write modules are implemented to handle all store and load operations

othertests contains tests written by us, while lab1tests contains the tests that were given to us in lab1