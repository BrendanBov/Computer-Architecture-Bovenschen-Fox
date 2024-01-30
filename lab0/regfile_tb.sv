module stimulus ();

  //INPUTS
   logic  clock;

   logic we3; //write enable
   logic  [4:0]   ra1, ra2, wa3; //2 read source register numbers, 1 write
   logic [31:0]  wd3; //write array to register

  //OUTPUTS
   logic [31:0] rd1, rd2; //2 read registers

  //VECTOR TESTING
   logic [31:0] rd1exp, rd2exp; //expected values
   logic [31:0] vectornum, errors; //store vector position and number of errors
   logic [123:0] testvectors[10000:0]; //store all test vectors

   logic [2:0] ignore[3:0]; //used to make writing test vectors easier
   
   integer handle3; //file saving
   integer desc3;
   
   // Instantiate DUT
   regfile dut(clock, we3, ra1, ra2, wa3, wd3, rd1, rd2);

   // Setup the clock to toggle every 1 time units 
   initial 
     begin	
	clock = 1'b1;
	forever #5 clock = ~clock;
     end

   initial
     begin
      // Gives output file name
      handle3 = $fopen("testReg.out");

      $readmemh("regfile.tv", testvectors); //read test vectors
    
      vectornum = 0; errors = 0;

      // Finish simulation after 500ns
      #5000 $finish;
     end

  always @ (posedge clock)
    begin
      //write waveforms from test vectors
      //ignore bits to format readfile better in hex
      #1; {ignore[0],we3,ignore[1],ra1,ignore[2],ra2,ignore[3],wa3,wd3,rd1exp,rd2exp} = testvectors[vectornum]; 
    end

  always @ (negedge clock)
    begin
      if(rd1 !== rd1exp | rd2 !== rd2exp) //check test vector outputs
      begin
        $display("Error!");
        $display("Inputs: we3=%h ra1=%h ra2=%h wa3=%h wd3=%h",we3,ra1,ra2,wa3,wd3);
        $display("Outputs: rd1=%h (%h expected) rd2=%h (%h expected)",rd1,rd1exp,rd2,rd2exp);
        errors++;
      end

      vectornum++; //next test vector

      if(testvectors[vectornum] === 'bx) //end test vectors
        begin
          $display("%d tests completed with %d errors", vectornum, errors);
          $stop;
        end
      
    end

   always  //handle output file
     begin
	  desc3 = handle3;
	  #5 $fdisplay(desc3, "%b %h %h %h %h || %h %h", 
		     we3, ra1, ra2, wa3, wd3, rd1, rd2);
     end   

endmodule // regfile_tb

