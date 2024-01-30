  module regfile (input logic         clk, 
		input logic 	    we3, //Write enable, active high
		input logic [4:0]   ra1, ra2, wa3,  //2 read source register numbers, 1 write
		input logic [31:0]  wd3, //write array to register
		output logic [31:0] rd1, rd2); //2 read registers
   
   logic [31:0] rf[31:1]; //Register array (32 count 32-bit registers)
   
   // three ported register file
   // read two ports combinationally
   // write third port on rising edge of clock
   // register 0 hardwired to 0

   always_ff @ (posedge clk)
    begin

      if(we3 & wa3 != 5'b0) //write logic, dont write to address 0
        rf[wa3] = wd3; //write wd3 to rf at wa3
    end

    always_comb 
      begin
        //multiplexer used so value 0 is always read from register 0
        rd1 = (ra1 == 5'b0) ? (32'h0) : rf[ra1]; //write to output register 1
        rd2 = (ra2 == 5'b0) ? (32'h0) : rf[ra2]; //write to output register 2
      end
   
   
endmodule // regfile
