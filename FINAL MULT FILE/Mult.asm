
//putting the first number in R1
	@R1 		
	D=M 		
	@num1 		
	M=D 		
	
//putting the second number in R2
	@R2
	D=M
	@num2
	M=D 	
	
//initializing the sum so we can multiply the numbers in R3
	@R3 		
	M=0 		//initializing to 0
	D=M 		//D=0
	@sum		
	M=D 		
	
//loop to keep adding the numbers
	
(LOOP) 
	@num2 
	D=M
	@END
	D;JEQ 	//if num2 is equal to 0 go to END
	@num1
	D=M
	@sum
	M=D+M 	
	@num2
	M=M-1 
	@LOOP
	0;JMP 	//loops again
(END)
	@sum
	D=M
	@R3
	M=D 	//writes the sum in r3
	@END
	0;JMP 	//infinte loop
	
	