module enable_pulse (
    input  wire   rst_n,
    input  wire   clk_50m,
    output reg    pulse_1hz 
);

localparam CNT_MAX_VALUE = 'd50_000_000;
localparam CNT_WIDTH = 26;  // 位宽

reg [CNT_WIDTH - 1 : 0] cnt;
    
always @(posedge clk_50m or negedge rst_n) begin
    if(!rst_n)begin
        cnt <= {CNT_WIDTH{1'b0}};
        pulse_1hz <= 1'b0;
    end 
    else if(cnt == CNT_MAX_VALUE - 1) begin
        pulse_1hz <= 1'b1;
        cnt <= {CNT_WIDTH{1'b0}};
    end
    else begin
        cnt <= cnt + 1'b1;
        pulse_1hz <= 1'b0;
    end
end

endmodule