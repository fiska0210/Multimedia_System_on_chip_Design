module ToUpper(
	input i_clk,
	input i_rst,
	input       i_valid,
	input [7:0] i_char,
	output logic       o_valid,
	output logic [7:0] o_char
);

always_ff @(posedge i_clk or negedge i_rst) begin
	if (!i_rst) begin
		o_char <= 0;
	end 
    else if (i_valid) 
    begin
        if (i_char > 8'd96 && i_char <8'd123) 
            o_char <= i_char - 8'd32;
        else 
		    o_char <= i_char;
	end

end

always_ff @(posedge i_clk or negedge i_rst) begin
	if (!i_rst) begin
		o_valid <= 1'b0;
	end else begin
		o_valid <= i_valid;
	end
end

endmodule
