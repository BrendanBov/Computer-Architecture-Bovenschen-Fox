  module regfile (input logic         clk, 
		input logic 	    we3, //Write enable, active high
		input logic [4:0]   ra1, ra2, wa3,  //2 read source register numbers, 1 write
		input logic [31:0]  wd3, //write array to register
		output logic [31:0] rd1, rd2); //2 read registers
   
   logic [31:0] rf[31:0]; //Register array (32 count 32-bit registers)
   
   // three ported register file
   // read two ports combinationally
   // write third port on rising edge of clock
   // register 0 hardwired to 0

   always_ff @ (posedge clk)
    begin

      if(we3) //write logic
        rf[wa3] = wd3; //write wd3 to rf at wa3

      rf[5'b00000] = 32'h00000000; //always write register 0 to value 0
    end

    always_comb 
      begin
        rd1 <= rf[ra1]; //write to output register 1
        rd2 <= rf[ra2]; //write to output register 2
      end
   
   
endmodule // regfile
