module clk_div (
    input  wire  clk_50m,
    input  wire  rst_n,
    output reg   clk_1hz
);
    
localparam CNT_MAX_VAL = 'd25_000_000;
localparam CNT_WIDTH = 25;

reg [CNT_WIDTH - 1 : 0] cnt;

always @(posedge clk_50m or negedge rst_n) begin
    if(!rst_n) begin
        cnt <= {CNT_WIDTH{1'b0}};
        clk_1hz <= 1'b0;
    end else if(cnt == CNT_MAX_VAL - 1) begin
        cnt <= {CNT_WIDTH{1'b0}};
        clk_1hz <= ~clk_1hz;
    end else begin
        cnt <= cnt + 1'b1;
    end
end

endmodule