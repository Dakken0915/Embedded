module fsm_three (
    input  wire   clk,
    input  wire   rst_n,
    input  wire   key_in,   // 按键输入
    output reg    key_out   // 消抖后的按键输出
);

    //========== 状态编码 ============
    localparam IDLE     = 3'b001;
    localparam FILTER   = 3'b010;
    localparam PRESS    = 3'b100;

    //========= 状态寄存器和次态变量 =========
    reg [2:0] cur_state;
    reg [2:0] next_state;
    reg [19:0] cnt;          // 20位计数器，用于延时消抖

    //========= 第一段：状态转移 =========
    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            cur_state <= IDLE;
        end else begin
            cur_state <= next_state;
        end
    end

    //========= 第二段：次态逻辑 =========
    // 根据当前状态和输入条件，决定下一个状态是什么
    always @(*) begin

        next_state = cur_state;  // 默认保持，防止产生锁存器

        case (cur_state)
            IDLE: begin
                // 检测到按键按下（key_in 为低）
                if (key_in == 1'b0) begin
                    next_state = FILTER;
                end else begin
                    next_state = IDLE;
                end
            end

            FILTER: begin
                    // 延时结束后再次检测
                    if (cnt == 20'd999_999) begin   // 20ms 延时（50MHz ÷ 1000）
                        if (key_in == 1'b0) begin
                            next_state = PRESS;
                        end else begin
                            next_state = IDLE;       // 抖动干扰，回到空闲
                        end
                    end else begin
                        next_state = FILTER;         // 继续计时
                    end
                end
            
            PRESS: begin
                // 等待按键释放
                if (key_in == 1'b1) begin
                    next_state = IDLE;
                end else begin
                    next_state = PRESS;
                end
            end
            
            default: begin
                next_state = IDLE;               // 安全状态
            end
        endcase
    end

    //========= 第三段：输出逻辑 =========
    // 根据当前状态，产生稳定的输出信号
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            key_out <= 1'b1;        // 默认高电平（无按键）
        end else begin
            case (cur_state)
                IDLE:   key_out <= 1'b1;
                FILTER: key_out <= 1'b1;        // 滤波期间不输出
                PRESS:  key_out <= 1'b0;        // 确认按下，输出低电平
                default: key_out <= 1'b1;
            endcase
        end
    end
    

    //============== 计数逻辑 =============
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            cnt <= 20'd0;
        end else begin
            // 只在 FILTER 状态计数，且未达到最大值
            if (cur_state == FILTER && cnt < 20'd999_999) begin
                cnt <= cnt + 1'b1;
            end else begin
                cnt <= 20'd0;       // 非 FILTER 状态或计数完成时清零
            end
        end
    end

endmodule