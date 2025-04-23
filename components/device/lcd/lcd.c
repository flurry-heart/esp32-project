#include "lcd.h"

#ifdef LCD_USE_HARDWARE_SPI

spi_device_handle_t g_lcd_spi_device_handle;

#endif // LCD_USE_HARDWARE_SPI

static void lcd_gpio_init(void);
static void lcd_st7735_init(void);


/**
 * @brief LCD初始化函数
 * 
 */
void lcd_init(void)
{
    lcd_gpio_init();

    lcd_reset();                                                                // 复位LCD
    LCD_BLK(1);                                                                 // 开启背光
    LCD_CS(1);

    lcd_st7735_init();

    lcd_clear(WHITE);
}

/**
 * @brief LCD底层初始化函数
 * 
 */
static void lcd_gpio_init(void)
{
    gpio_config_t gpio_config_struct = {0};

    gpio_config_struct.pin_bit_mask = 1ULL << LCD_CS_GPIO_NUM;                  // 设置引脚
    gpio_config_struct.intr_type = GPIO_INTR_DISABLE;                           // 不使用中断
    gpio_config_struct.mode = GPIO_MODE_OUTPUT;                                 // 输出模式
    gpio_config_struct.pull_down_en = GPIO_PULLDOWN_DISABLE;                    // 不使用下拉
    gpio_config_struct.pull_up_en = GPIO_PULLUP_DISABLE;                        // 不使用上拉
    gpio_config(&gpio_config_struct);                                           // 配置GPIO

    gpio_config_struct.pin_bit_mask = 1ULL << LCD_DC_GPIO_NUM;                  // 设置引脚
    gpio_config(&gpio_config_struct);                                           // 配置GPIO

    gpio_config_struct.pin_bit_mask = 1ULL << LCD_RESET_GPIO_NUM;               // 设置引脚
    gpio_config(&gpio_config_struct);                                           // 配置GPIO

    gpio_config_struct.pin_bit_mask = 1ULL << LCD_BLK_GPIO_NUM;                 // 设置引脚
    gpio_config(&gpio_config_struct);                                           // 配置GPIO
}

/**
 * @brief 向寄存器写入一个字节数据函数
 * 
 * @param data 一字节数据
 * @param mode LCD状态的枚举值
 */
void lcd_write_byte(uint8_t data, lcd_write_mode_t mode)
{
    LCD_CS(0);                                                                  // 片选
    LCD_DC(mode);                                                               // 数据/命令选择

#ifdef LCD_USE_HARDWARE_SPI
    bsp_spi_send_one_byte(g_lcd_spi_device_handle, data);                       // 发送数据
#endif // LCD_USE_HARDWARE_SPI

#ifndef LCD_USE_HARDWARE_SPI
    bsp_simulate_spi_swap_one_byte(data);
#endif // !LCD_USE_HARDWARE_SPI

    LCD_CS(1);                                                                  // 取消片选
}

/**
 * @brief 向寄存器写入多个字节数据函数
 * 
 * @param data 指向要写入的数据的指针
 * @param length 要写入的数据长度
 * @param mode LCD状态的枚举值
 */
void lcd_write_bytes(uint8_t *data, uint32_t length, lcd_write_mode_t mode)
{
    LCD_CS(0);                                                                  // 片选
    LCD_DC(mode);                                                               // 数据/命令选择

#ifdef LCD_USE_HARDWARE_SPI
    bsp_spi_send_bytes(g_lcd_spi_device_handle, data, length);                  // 发送数据
#endif // LCD_USE_HARDWARE_SPI

#ifndef LCD_USE_HARDWARE_SPI
    for (uint32_t i = 0; i < length; i++)
    {
        bsp_simulate_spi_swap_one_byte(data[i]);
    }
#endif // !LCD_USE_HARDWARE_SPI

    LCD_CS(1);                                                                  // 取消片选 
}

/**
 * @brief 从寄存器读取一个字节数据函数
 * 
 * @param mode LCD状态的枚举值
 * @return uint8_t 读取的一字节数据
 */
uint8_t lcd_read_one_byte(void)
{
    uint8_t data = 0;

    LCD_CS(0);                                                                  // 片选
    LCD_DC(LCD_MODE_DATA);                                                      // 数据/命令选择

#ifdef LCD_USE_HARDWARE_SPI
    data = bsp_spi_transfer_one_byte(g_lcd_spi_device_handle, 0);               // 读取数据
#endif // LCD_USE_HARDWARE_SPI

#ifndef LCD_USE_HARDWARE_SPI
    data = bsp_simulate_spi_swap_one_byte(0);
#endif // !LCD_USE_HARDWARE_SPI

    LCD_CS(1);                                                                  // 取消片选

    return data;
}

/**
 * @brief ST7735初始化函数
 * 
 */
static void lcd_st7735_init(void)
{
    // LCD Init For 1.44Inch LCD Panel with ST7735R.
    lcd_write_byte(0x11, LCD_MODE_CMD);                                         // Sleep exit 
    vTaskDelay(120);
        
    // ST7735R Frame Rate
    lcd_write_byte(0xB1, LCD_MODE_CMD); 
    lcd_write_byte(0x01, LCD_MODE_DATA); 
    lcd_write_byte(0x2C, LCD_MODE_DATA); 
    lcd_write_byte(0x2D, LCD_MODE_DATA); 

    lcd_write_byte(0xB2, LCD_MODE_CMD); 
    lcd_write_byte(0x01, LCD_MODE_DATA); 
    lcd_write_byte(0x2C, LCD_MODE_DATA); 
    lcd_write_byte(0x2D, LCD_MODE_DATA); 

    lcd_write_byte(0xB3, LCD_MODE_CMD); 
    lcd_write_byte(0x01, LCD_MODE_DATA); 
    lcd_write_byte(0x2C, LCD_MODE_DATA); 
    lcd_write_byte(0x2D, LCD_MODE_DATA); 
    lcd_write_byte(0x01, LCD_MODE_DATA); 
    lcd_write_byte(0x2C, LCD_MODE_DATA); 
    lcd_write_byte(0x2D, LCD_MODE_DATA); 

    lcd_write_byte(0xB4, LCD_MODE_CMD);                                         // Column inversion 
    lcd_write_byte(0x07, LCD_MODE_DATA); 

    //ST7735R Power Sequence
    lcd_write_byte(0xC0, LCD_MODE_CMD); 
    lcd_write_byte(0xA2, LCD_MODE_DATA); 
    lcd_write_byte(0x02, LCD_MODE_DATA); 
    lcd_write_byte(0x84, LCD_MODE_DATA); 
    lcd_write_byte(0xC1, LCD_MODE_CMD); 
    lcd_write_byte(0xC5, LCD_MODE_DATA); 

    lcd_write_byte(0xC2, LCD_MODE_CMD); 
    lcd_write_byte(0x0A, LCD_MODE_DATA); 
    lcd_write_byte(0x00, LCD_MODE_DATA); 

    lcd_write_byte(0xC3, LCD_MODE_CMD); 
    lcd_write_byte(0x8A, LCD_MODE_DATA); 
    lcd_write_byte(0x2A, LCD_MODE_DATA); 
    lcd_write_byte(0xC4, LCD_MODE_CMD); 
    lcd_write_byte(0x8A, LCD_MODE_DATA); 
    lcd_write_byte(0xEE, LCD_MODE_DATA); 

    lcd_write_byte(0xC5, LCD_MODE_CMD);                                         // VCOM 
    lcd_write_byte(0x0E, LCD_MODE_DATA); 

    lcd_write_byte(0x36, LCD_MODE_CMD);                                         // MX, MY, RGB mode 
    lcd_write_byte(0xC0, LCD_MODE_DATA); 

    // ST7735R Gamma Sequence
    lcd_write_byte(0xe0, LCD_MODE_CMD); 
    lcd_write_byte(0x0F, LCD_MODE_DATA); 
    lcd_write_byte(0x1A, LCD_MODE_DATA); 
    lcd_write_byte(0x0F, LCD_MODE_DATA); 
    lcd_write_byte(0x18, LCD_MODE_DATA); 
    lcd_write_byte(0x2F, LCD_MODE_DATA); 
    lcd_write_byte(0x28, LCD_MODE_DATA); 
    lcd_write_byte(0x20, LCD_MODE_DATA); 
    lcd_write_byte(0x22, LCD_MODE_DATA); 
    lcd_write_byte(0x1F, LCD_MODE_DATA); 
    lcd_write_byte(0x1B, LCD_MODE_DATA); 
    lcd_write_byte(0x23, LCD_MODE_DATA); 
    lcd_write_byte(0x37, LCD_MODE_DATA); 
    lcd_write_byte(0x00, LCD_MODE_DATA); 	
    lcd_write_byte(0x07, LCD_MODE_DATA); 
    lcd_write_byte(0x02, LCD_MODE_DATA); 
    lcd_write_byte(0x10, LCD_MODE_DATA); 

    lcd_write_byte(0xE1, LCD_MODE_CMD); 
    lcd_write_byte(0x0F, LCD_MODE_DATA); 
    lcd_write_byte(0x1B, LCD_MODE_DATA); 
    lcd_write_byte(0x0F, LCD_MODE_DATA); 
    lcd_write_byte(0x17, LCD_MODE_DATA); 
    lcd_write_byte(0x33, LCD_MODE_DATA); 
    lcd_write_byte(0x2C, LCD_MODE_DATA); 
    lcd_write_byte(0x29, LCD_MODE_DATA); 
    lcd_write_byte(0x2E, LCD_MODE_DATA); 
    lcd_write_byte(0x30, LCD_MODE_DATA); 
    lcd_write_byte(0x30, LCD_MODE_DATA); 
    lcd_write_byte(0x39, LCD_MODE_DATA); 
    lcd_write_byte(0x3F, LCD_MODE_DATA); 
    lcd_write_byte(0x00, LCD_MODE_DATA); 
    lcd_write_byte(0x07, LCD_MODE_DATA); 
    lcd_write_byte(0x03, LCD_MODE_DATA); 
    lcd_write_byte(0x10, LCD_MODE_DATA);  

    lcd_write_byte(0x2A, LCD_MODE_CMD);
    lcd_write_byte(0x00, LCD_MODE_DATA);
    lcd_write_byte(0x00, LCD_MODE_DATA);
    lcd_write_byte(0x00, LCD_MODE_DATA);
    lcd_write_byte(0x7F, LCD_MODE_DATA);

    lcd_write_byte(0x2B, LCD_MODE_CMD);
    lcd_write_byte(0x00, LCD_MODE_DATA);
    lcd_write_byte(0x00, LCD_MODE_DATA);
    lcd_write_byte(0x00, LCD_MODE_DATA);
    lcd_write_byte(0x9F, LCD_MODE_DATA);

    lcd_write_byte(0xF0, LCD_MODE_CMD);                                         // Enable test command  
    lcd_write_byte(0x01, LCD_MODE_DATA); 
    lcd_write_byte(0xF6, LCD_MODE_CMD);                                         // Disable ram power save mode 
    lcd_write_byte(0x00, LCD_MODE_DATA); 

    lcd_write_byte(0x3A, LCD_MODE_CMD);                                         // 65k mode 
    lcd_write_byte(0x05, LCD_MODE_DATA); 

    lcd_write_byte(0x29, LCD_MODE_CMD);                                         // Display on	 
}

/**
 * @brief LCD复位函数
 * 
 */
void lcd_reset(void)
{
    LCD_RESET(0);
    vTaskDelay(1);
    LCD_RESET(1);
    vTaskDelay(120);
}

/**
 * @brief OLED设置坐标函数
 * 
 * @param x 坐标所在的列，范围: 0 ~ 127
 * @param y 坐标所在的行: 0 ~ 159
 */
void lcd_set_cursor(uint32_t x, uint32_t y)
{
    lcd_write_byte(0x2A, LCD_MODE_CMD);                                         // 设置列地址
    lcd_write_byte(0x00, LCD_MODE_DATA);                                        // 发送列地址的起始地址高8位
    lcd_write_byte(x, LCD_MODE_DATA);                                           // 发送列地址的起始地址低8位

    lcd_write_byte(0x2B, LCD_MODE_CMD);                                         // 发送页地址
    lcd_write_byte(0x00, LCD_MODE_DATA);                                        // 发送页地址的起始地址高8位
    lcd_write_byte(y, LCD_MODE_DATA);   
}

/**
 * @brief 设置LCD的光标范围
 * 
 * @param x1 光标的起始位置的列
 * @param y1 光标的起始位置的行
 * @param x2 光标的结束位置的列
 * @param y2 光标的结束位置的行
 */
void lcd_set_cursor_area(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2)
{
    lcd_write_byte(0x2A, LCD_MODE_CMD);                                         // 设置列地址
    lcd_write_byte(0x00, LCD_MODE_DATA);                                        // 发送列地址的起始地址高8位
    lcd_write_byte(x1, LCD_MODE_DATA);                                          // 发送列地址的起始地址低8位
    lcd_write_byte(0x00, LCD_MODE_DATA);                                        // 发送列地址的起始地址高8位
    lcd_write_byte(x2, LCD_MODE_DATA);                                          // 发送列地址的起始地址低8位

    lcd_write_byte(0x2B, LCD_MODE_CMD);                                         // 发送页地址
    lcd_write_byte(0x00, LCD_MODE_DATA);                                        // 发送页地址的起始地址高8位
    lcd_write_byte(y1, LCD_MODE_DATA);                                          // 发送页地址的起始地址低8位
    lcd_write_byte(0x00, LCD_MODE_DATA);                                        // 发送页地址的起始地址高8位
    lcd_write_byte(y2, LCD_MODE_DATA);                                          // 发送页地址的起始地址低8位
}

/**
 * @brief LCD设置显示方向函数
 * 
 * @param direction 0：从左到右，从上到下
 *                  1：从上到下，从左到右
 *                  2：从右到左，从上到下
 *                  3：从上到下，从右到左
 *                  4：从左到右，从下到上
 *                  5：从下到上，从左到右
 *                  6：从右到左，从下到上
 *                  7：从下到上，从右到左
 */
void lcd_display_direction(uint8_t mode)
{
    lcd_write_byte(0x36, LCD_MODE_CMD);                                         //设置彩屏显示方向的寄存器
    lcd_write_byte(0x00 | (mode << 5), LCD_MODE_DATA);
  
    switch (mode)
    {
    case 0:
    case 2:
    case 4:
    case 6:
        lcd_set_cursor_area(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
        break;

    case 1:
    case 3:
    case 5:
    case 7:
        lcd_set_cursor_area(0, 0, LCD_HEIGHT - 1, LCD_WIDTH - 1);
  
    default:
        break;
    }
}

/**
 * @brief LCD清屏函数
 * 
 * @param color 颜色
 */
void lcd_clear(uint16_t color)
{
    uint16_t total_point = LCD_WIDTH * LCD_HEIGHT;                              // 得到总点数
    uint8_t data[2] = {color >> 8, color & 0xFF};

    lcd_set_cursor_area(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);                   // 设置光标位置
    lcd_write_byte(0x2C, LCD_MODE_CMD);                                         // 发送写GRAM指令
    for (uint16_t index = 0; index < total_point; index++)
    {
        lcd_write_bytes(data, 2, LCD_MODE_DATA);                                // 发送颜色数据
    }
}

/**
 * @brief LCD局部清屏函数
 * 
 * @param x 要清空的区域的左上角的列坐标
 * @param y 要清空的区域的左上角的行坐标
 * @param width 要清空的区域的宽度
 * @param height 要清空的区域的高度
 * @param color 要清空的区域的颜色
 */
void lcd_clear_area(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color)
{
    for (uint8_t i = y; i < y + height; i++)
    {
        lcd_set_cursor(x, i);                                                   // 设置光标位置
        lcd_write_byte(0x2C, LCD_MODE_CMD);                                     // 发送写GRAM指令
        for (uint8_t j = x; j < x + width; j++)
        {
            uint8_t data[2] = {color >> 8, color & 0xFF};                       // 颜色数据
            lcd_write_bytes(data, 2, LCD_MODE_DATA);                            // 发送颜色数据
        }
    }
}

/**
 * @brief LCD画点函数
 * 
 * @param x 列
 * @param y 行
 * @param color 颜色
 */
void lcd_draw_point(uint32_t x, uint32_t y, uint16_t color)
{
    uint8_t data[2] = {color >> 8, color & 0xFF};                               // 16位颜色
    lcd_set_cursor(x, y);                                                       // 设置坐标
    lcd_write_byte(0x2C, LCD_MODE_CMD);                                         // 发送写GRAM指令
    lcd_write_bytes(data, 2, LCD_MODE_DATA);                                    // 写入颜色值
}

/**
 * @brief LCD读点函数
 * 
 * @param x 列数
 * @param y 行数
 * @return uint16_t 
 */
uint16_t lcd_read_point(uint32_t x, uint32_t y)
{
    uint16_t r = 0, g = 0, b = 0;

    lcd_set_cursor(x, y);                                                       // 设置坐标
    lcd_write_byte(0x2E, LCD_MODE_CMD);                                         // 读GRAM数据指令
    lcd_read_one_byte();                                                        // 假读
    r = lcd_read_one_byte();                                                    // 读取R通道和G通道的值
    b = lcd_read_one_byte();                                                    // 读取B通道的值
    g = r & 0xFF;                                                               // 获取G通道的值
    return (((r >> 11) << 11) | ((g >> 2) << 5) | (b >> 11));
}

/**
 * @brief LCD显示字符函数
 * 
 * @param x 列
 * @param y 行
 * @param chr 显示的字符
 * @param size 字体大小，这里字符的高度等于字重，字符的宽度等于字重的一半
 * @param forecolor 字符的颜色
 * @param backcolor 背景色
 * @param display_mode 显示模式的枚举值
 */
void lcd_show_char(uint32_t x, uint32_t y, char chr, uint16_t size, uint16_t forecolor, uint16_t backcolor, lcd_display_mode_t display_mode)
{
    uint8_t *pfont = NULL;
    uint8_t temp = 0;
    uint8_t high = size / 8 + ((size % 8) ? 1 : 0);                            // 得到一个字符对应的字节数

    switch (size)
    {
        case 12:
            pfont = (uint8_t *)ascii_06x12[chr - ' '];                          // 调用06x12字体
            break;

        case 16:
            pfont = (uint8_t *)ascii_08x16[chr - ' '];                          // 调用08x16字体
            break;

        case 24:
            pfont = (uint8_t *)ascii_12x24[chr - ' '];                          // 调用12x24字体
            break;

        case 32:
            pfont = (uint8_t *)ascii_16x32[chr - ' '];                          // 调用16x32字体
            break;

        default:
            return ;
    }

    for (uint8_t h = 0; h < high; h++)                                         // 遍历字符的高度
    {
        for (uint8_t w = 0; w < size / 2; w++)                                  // 遍历字符的宽度
        {
            temp = pfont[h * size / 2 + w];                                     // 获取字符对应的字节数据
            for (uint8_t k = 0; k < 8; k++)                                     // 一个字节8个像素点
            {
                if (temp & 0x01)                                                // 绘制字符
                {
                    lcd_draw_point(x + w, y + k + 8 * h , forecolor);
                }
                else
                {
                    if (display_mode == LCD_DISPLAY_NORMAL)                      // 是否绘制背景
                    {
                        lcd_draw_point(x + w, y + k + 8 * h , backcolor);
                    }
                }
                temp >>= 1;
            }
        }
    }
}

/**
 * @brief LCD显示字符串函数
 * 
 * @param x 列
 * @param y 行
 * @param str 显示的字符串
 * @param size 字体大小，这里字符的高度等于字重，字符的宽度等于字重的一半
 * @param forecolor 字符串的颜色
 * @param backcolor 背景色
 * @param display_mode 显示模式的枚举值
 */
void lcd_show_string(uint32_t x, uint32_t y, char *str, uint16_t size, uint16_t forecolor, uint16_t backcolor, lcd_display_mode_t display_mode)
{
    uint32_t x0 = x;
    for (uint32_t i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
        {
            x = x0;
            y += size;
            continue;
        }
        lcd_show_char(x, y, str[i], size, forecolor, backcolor, display_mode);
        x += (size / 2);
    }
}

/**
 * @brief LCD显示汉字函数
 * 
 * @param x 列
 * @param y 行
 * @param chinese 要显示的汉字
 * @param size 要显示的汉字大小
 * @param forecolor 汉字的颜色
 * @param backcolor 背景色
 * @param display_mode 显示模式的枚举值
 */
void lcd_show_chinese(uint32_t x, uint32_t y, char *chinese, uint16_t size, uint16_t forecolor, uint16_t backcolor, lcd_display_mode_t display_mode)
{
    char sigle_chinese[4] = {0};                                                // 存储单个汉字
    uint16_t index = 0;                                                         // 汉字索引
    uint16_t high = size / 8 + ((size % 8) ? 1 : 0);                            // 得到一个字符对应的字节数
    uint16_t temp = 0;
    uint16_t j = 0;

    switch (size)
    {
    case 32:
        for (uint32_t i = 0; chinese[i] != '\0'; i++)                           // 遍历汉字字符串
        {
            // 获取单个汉字，一般UTF-8编码使用3个字节存储汉字，GBK编码使用2个字节存储汉字
            sigle_chinese[index] = chinese[i];
            index++;
            index = index % 3;

            if (index == 0)                                                     // 汉字索引为0，说明已经获取了一个汉字
            {
                for (j = 0; strcmp(chinese_32x32[j].index, "") != 0; j++)       // 遍历汉字数组
                {
                    if (strcmp(chinese_32x32[j].index, sigle_chinese) == 0)      // 找到汉字
                    {
                        break;
                    } 
                }

                for (uint16_t h = 0; h < high; h++)                            // 遍历字符的高度
                {
                    for (uint16_t w = 0; w < size; w++)                         // 遍历字符的宽度
                    {
                        temp = chinese_32x32[j].data[h * size  + w];            // 获取字符对应的字节数据
                        for (uint16_t k = 0; k < 8; k++)                        // 一个字节8个像素点
                        {
                            if (temp & 0x01)                                    // 绘制字体
                            {
                                // ((i + 1) / 3)定位到第几个汉字
                                lcd_draw_point(x + w + ((i + 1) / 3 - 1) * size, y + k + 8 * h , forecolor);
                            }
                            else
                            {
                                if (display_mode == LCD_DISPLAY_NORMAL)         // 是否绘制背景
                                {
                                    lcd_draw_point(x + w + ((i + 1) / 3 - 1) * size, y + k + 8 * h , backcolor);
                                }  
                            }
                            
                            temp >>= 1;
                        }
                    }
                }
            }
        }
        break;

    default:
        break;
    }
}

/**
 * @brief LCD显示数字函数
 * 
 * @param x 列
 * @param y 行
 * @param num 要显示的数字
 * @param size 要显示的数字大小
 * @param forecolor 要显示的数字的颜色
 * @param backcolor 背景色
 * @param data_format 格式化结构体
 */
void lcd_show_number(uint32_t x, uint32_t y, int num, uint16_t size, uint16_t forecolor, uint16_t backcolor, lcd_data_format_t data_format)
{
    char str[20] = {0};

    if (data_format.align_right)
    {
        if (data_format.symbol)
        {
            sprintf(str, "%+*d", data_format.digit, num);                       // 显示符号右对齐
        }
        else
        {
            sprintf(str, "%*d", data_format.digit, num);                        // 不显示符号右对齐
        }
    }
    else
    {
        if (data_format.symbol)                                  
        {
            sprintf(str, "%+d", num);                                           // 显示符号左对齐
        }
        else                                                    
        {
            sprintf(str, "%d", num);                                            // 不显示符号左对齐
        }
    }

    lcd_show_string(x, y, str, size, forecolor, backcolor, data_format.display_mode);
}

/**
 * @brief LCD显示浮点数函数
 * 
 * @param x 列
 * @param y 行
 * @param num 要显示的数字
 * @param size 要显示的数字大小
 * @param forecolor 要显示的数字的前景色
 * @param backcolor 背景色
 * @param data_format 格式化结构体
 */
void lcd_show_decimal(uint32_t x, uint32_t y, double decimal, uint16_t size, uint16_t forecolor, uint16_t backcolor, lcd_data_format_t data_format)
{
    int num = (int)decimal;
    int temp = num;
    int len = 0;
    int x1 = x;

    data_format.decimal = data_format.decimal ? data_format.decimal : 2;        // 小数部分的位数默认为两位

    // 获取整数的位数
    while (temp)
    {
        temp /= 10;
        len++;
    }

    // 获取整数的位数
    if (data_format.digit > len)
    {
        data_format.digit -= data_format.decimal;                               // 获取整数的位数
        data_format.align_right ? data_format.digit-- : data_format.digit;      // 如果右对齐的话，还要左移一位，显示小数点
        data_format.digit = (data_format.digit > len) ? data_format.digit : len;// 确保整数的位数大于等于指定值的整数位
    }
  
    lcd_show_number(x, y, num, size, forecolor, backcolor, data_format);        // 显示整数位

    // 显示小数点
    if (data_format.digit > len)                                                // 如果整数位数大于实际的整数位数
    {
        if (data_format.align_right)                                            // 如果右对齐的话
        {
            x1 += (data_format.digit * (size / 2));                             // 光标移动到整数位数后
        }
        else                                                                    // 如果是左对齐的话
        {
            x1 += (len * (size / 2));                                           // 光标移动实际的整数位后
            x1 = ((num < 0) || (data_format.symbol)) ? x1 + (size / 2) : x1;    // 如果有符号的话，光标还需要右移一位
        }
    }
    else                                                                        // 如果整数位数等于实际的整数位数
    {
        x1 += (len * (size / 2));                                               // 光标移动到实际的整数位数后
        x1 = ((num < 0) || (data_format.symbol)) ? x1 + (size / 2) : x1;        // 如果有符号的话，光标还需要右移一位
    }
  
    lcd_show_char(x1, y, '.', size, forecolor, backcolor, data_format.display_mode);    // 显示小数点
    x1 += (size / 2);                                                           // 光标移动到小数点位置

    // 显示小数部分
    data_format.symbol = 0;                                                     // 小数部分不显示符号
    data_format.digit = data_format.decimal;                                    // 放大后，重新设置要显示的位数
    data_format.align_right = 0;                                                // 小数部分为左对齐

    // 放大小数位，并且确保得到的结果为整数
    temp = 1;
    for (int i = 0; i < data_format.decimal; i++)
    {
        temp *= 10;
    }
    num = decimal * temp - num * temp;                                          // 小数部分取整
    num = num > 0 ? num : -num;                                                 // 确保小数部分为正数

    lcd_show_number(x1, y, num, size, forecolor, backcolor, data_format);       // 显示小数部分
}


/**
 * @brief LCD打印函数
 * 
 * @param x 列
 * @param y 行
 * @param size 要显示的字符串大小
 * @param forecolor 字符串的景色
 * @param backcolor 背景色
 * @param display_mode 显示模式的枚举值
 * @param format 指定要显示的格式化字符串
 * @param ... 格式化字符串参数列表
 * 
 * @note 此函数对浮点数的支持不好，使用浮点数时无法显示
 */
void lcd_printf(uint32_t x, uint32_t y, uint16_t size, uint16_t forecolor, uint16_t backcolor, lcd_display_mode_t display_mode, char *format, ...)
{
    char str[256];                                                              // 定义字符数组
    va_list arg;                                                                // 定义可变参数列表数据类型的变量arg
    va_start(arg, format);                                                      // 从format开始，接收参数列表到arg变量
    vsprintf(str, format, arg);                                                 // 使用vsprintf打印格式化字符串和参数列表到字符数组中
    va_end(arg);                                                                // 结束变量arg
    lcd_show_string(x, y, str, size, forecolor, backcolor, display_mode);       // OLED显示字符数组（字符串）
}

/**
 * @brief LCD显示图片函数
 * 
 * @param image 图片数据
 * @param x 列
 * @param y 行
 * @param width 图片的宽度
 * @param height 图片的高度
 */
void lcd_show_picture(uint8_t image[], uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    uint8_t color[2] = {0};

    uint32_t i = 0, j = 0, k = 0;

    for (i = 0; i < height; i++)
    {
        lcd_set_cursor(x, y + i);                                               // 设置光标位置
        lcd_write_byte(0x2C, LCD_MODE_CMD);                                     // 发送写GRAM指令
  
        for (j = 0; j < width; j++)
        {
            color[0] = image[k + 1];                                            // 获取图片数据
            color[1] = image[k];                                                // 获取图片数据
            k += 2;
            lcd_write_bytes(color, 2, LCD_MODE_DATA);                           // 写入颜色值
        }
    }
}

/**
 * @brief LCD画线函数
 * 
 * @param x1 第一个点的列
 * @param y1 第一个点的行
 * @param x2 第二个点的列
 * @param y2 第二个点的行
 * @param size 线宽
 * @param color 线的颜色
 */
void lcd_draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color)
{
    int32_t x = 0, y = 0, dx = 0, dy = 0, d = 0, incrE = 0, incrNE = 0, temp = 0;
    int32_t x0_t = x0, y0_t = y0, x1_t = x1, y1_t = y1;
    uint32_t y_flag = 0, xy_flag = 0;

    if (y0_t == y1_t)                                                           // 横线单独处理
    {
        if (x0_t > x1_t)                                                        // 0号点x坐标大于1号点x坐标，则交换两点x坐标
        {
            temp = x0_t; 
            x0_t = x1_t; 
            x1_t = temp;
        }
  
        for (x = x0_t; x <= x1_t; x++)                                          // 遍历x坐标，依次画点
        {
            lcd_draw_point(x, y0, color);
        }
    }
    else if (x0_t == x1_t)                                                      // 竖线单独处理
    {
        if (y0_t > y1_t)                                                        // 0号点y坐标大于1号点y坐标，则交换两点y坐标
        {
            temp = y0_t;
            y0_t = y1_t;
            y1_t = temp;
        }
  
        for (y = y0_t; y <= y1_t; y++)                                          // 遍历y坐标，依次画点
        {
            lcd_draw_point(x0, y, color);
        }
    }
    else                                                                        // 斜线
    {
        // 使用Bresenham算法画直线，可以避免耗时的浮点运算，效率更高
        if (x0_t > x1_t)                                                        // 0号点x坐标大于1号点x坐标，交换两点坐标
        {
            // 交换后不影响画线，但是画线方向由第一、二、三、四象限变为第一、四象限
            temp = x0_t; x0_t = x1_t; x1_t = temp;
            temp = y0_t; y0_t = y1_t; y1_t = temp;
        }
  
        if (y0_t > y1_t)                                                        // 0号点y坐标大于1号点y坐标，将y坐标取负
        {
            // 取负后影响画线，但是画线方向由第一、四象限变为第一象限
            y0_t = -y0_t;
            y1_t = -y1_t;
  
            y_flag = 1;                                                         // 置标志位y_flag，记住当前变换，在后续实际画线时，再将坐标换回来
        }
  
        if (y1_t - y0_t > x1_t - x0_t)	                                        // 画线斜率大于1，将x坐标与y坐标互换
        {
            // 互换后影响画线，但是画线方向由第一象限0~90度范围变为第一象限0~45度范围
            temp = x0_t; x0_t = y0_t; y0_t = temp;
            temp = x1_t; x1_t = y1_t; y1_t = temp;
  
            xy_flag = 1;                                                        // 置标志位xy_flag，记住当前变换，在后续实际画线时，再将坐标换回来
        }
  
        // 以下为Bresenham算法画直线，算法要求，画线方向必须为第一象限0~45度范围
        dx = x1_t - x0_t;
        dy = y1_t - y0_t;
        incrE = 2 * dy;
        incrNE = 2 * (dy - dx);
        d = 2 * dy - dx;
        x = x0_t;
        y = y0_t;

        // 画起始点，同时判断标志位，将坐标换回来
        if (y_flag && xy_flag) { 
            lcd_draw_point(y, -x, color); 
        } else if (y_flag) { 
            lcd_draw_point(x, -y, color); 
        } else if (xy_flag) { 
            lcd_draw_point(y, x, color); 
        } else { 
            lcd_draw_point(x, y, color); 
        }

        while (x < x1_t)                                                        // 遍历x轴的每个点
        {
            x++;
            if (d < 0)                                                          // 下一个点在当前点东方
            {
                d += incrE;
            }
            else                                                                // 下一个点在当前点东北方
            {
                y++;
                d += incrNE;
            }

            // 画每一个点，同时判断标志位，将坐标换回来
            if (y_flag && xy_flag) { 
                lcd_draw_point(y, -x, color); 
            } else if (y_flag) { 
                lcd_draw_point(x, -y, color); 
            } else if (xy_flag) { 
                lcd_draw_point(y, x, color); 
            } else { 
                lcd_draw_point(x, y, color); 
            }
        }
    }
}

/**
 * @brief LCD绘制矩形
 * 
 * @param x 列
 * @param y 行
 * @param width 要绘制矩形的宽
 * @param height 要绘制矩形的高
 * @param color 要绘制的颜色
 * @param filled 是否要填充 1: 填充，0: 不填充
 */
void lcd_draw_rectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color, bool filled)
{
    int32_t i = 0, j = 0;

    if (!filled)		                                                        // 指定矩形不填充
    {
        for (i = x; i < x + width; i++)                                         // 遍历上下x坐标，画矩形上下两条线
        {
            lcd_draw_point(i, y, color);
            lcd_draw_point(i, y + height - 1, color);
        }

        for (i = y; i < y + height; i++)                                        // 遍历左右y坐标，画矩形左右两条线
        {
            lcd_draw_point(x, i, color);
            lcd_draw_point(x + width - 1, i, color);
        }
    }
    else                                                                        // 指定矩形填充
    {
        for (i = x; i < x + width; i++)                                         // 遍历x坐标
        {
            for (j = y; j < y + height; j++)                                    // 遍历y坐标
            {
                lcd_draw_point(i, j, color);                                    // 在指定区域画点，填充满矩形
            }
        }
    }
}

/**
 * @brief LCD绘制三角形
 * 
 * @param x0 第一个点所在的列
 * @param y0 第一个点所在的行
 * @param x1 第二个点所在的列
 * @param y1 第二个点所在的行
 * @param x2 第三个点所在的列
 * @param y2 第三个点所在的行
 * @param color 要绘制的颜色
 * @param filled 是否要填充 1: 填充，0: 不填充
 */
void lcd_draw_triangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint16_t color, bool filled)
{
    int32_t min_x = x0, min_y = y0, max_x = x0, max_y = y0;
    int32_t i = 0, j = 0;
    int vertexX[] = {x0, x1, x2};
    int vertexY[] = {y0, y1, y2};

    if (!filled)                                                                // 指定三角形不填充
    {
        // 调用画线函数，将三个点用直线连接
        lcd_draw_line(x0, y0, x1, y1, color);
        lcd_draw_line(x0, y0, x2, y2, color);
        lcd_draw_line(x1, y1, x2, y2, color);
    }
    else                                                                        // 指定三角形填充
    {
        // 找到三个点最小的x、y坐标
        if (x1 < min_x) {min_x = x1;}
        if (x2 < min_x) {min_x = x2;}
        if (y1 < min_y) {min_y = y1;}
        if (y2 < min_y) {min_y = y2;}
  
        // 找到三个点最大的x、y坐标
        if (x1 > max_x) {max_x = x1;}
        if (x2 > max_x) {max_x = x2;}
        if (y1 > max_y) {max_y = y1;}
        if (y2 > max_y) {max_y = y2;}
  
  
        // 最小最大坐标之间的矩形为可能需要填充的区域
        for (i = min_x; i <= max_x; i++)
        {
            for (j = min_y; j <= max_y; j++)
            {
                // 调用pnpoly算法，判断指定点是否在指定三角形之中，如果在，则画点，如果不在，则不做处理
                if (pnpoly(3, vertexX, vertexY, i, j)) 
                {
                    lcd_draw_point(i, j, color);
                }
            }
        }
    }
}

/**
 * @brief LCD绘制椭圆函数
 * 
 * @param x 圆心所在的列
 * @param y 圆心所在的行
 * @param a 椭圆的横向半轴长度
 * @param b 椭圆的纵向半轴长度
 * @param filled 是否要填充 1: 填充，0: 不填充
 */
void lcd_draw_ellipse(uint32_t x, uint32_t y, uint32_t a, uint32_t b, uint16_t color, bool filled)
{
    int32_t x_t = 0;
    int32_t y_t = b;
    int32_t a_t = a, b_t = b;
    int32_t j = 0;
    float  d1 = b_t * b_t + a_t * a_t * (-b_t + 0.5);
    float d2 = 0;

    // 使用Bresenham算法画椭圆，可以避免部分耗时的浮点运算，效率更高
    if (filled)	//指定椭圆填充
    {
        // 遍历起始点y坐标在指定区域画点，填充部分椭圆
        for (j = -y_t; j < y_t; j++)
        {
            lcd_draw_point(x, y + j, color);
            lcd_draw_point(x, y + j, color);
        }
    }

    // 画椭圆弧的起始点
    lcd_draw_point(x + x_t, y + y_t, color);
    lcd_draw_point(x - x_t, y - y_t, color);
    lcd_draw_point(x - x_t, y + y_t, color);
    lcd_draw_point(x + x_t, y - y_t, color);

    // 画椭圆中间部分
    while (b_t * b_t * (x_t + 1) < a_t * a_t * (y_t - 0.5))
    {
        if (d1 <= 0)                                                            // 下一个点在当前点东方
        {
            d1 += b_t * b_t * (2 * x_t + 3);
        }
        else                                                                    // 下一个点在当前点东南方
        {
            d1 += b_t * b_t * (2 * x_t + 3) + a_t * a_t * (-2 * y_t + 2);
            y_t--;
        }
        x_t++;
  
        if (filled)                                                             // 指定椭圆填充
        {
            // 遍历中间部分
            for (j = -y_t; j < y_t; j++)
            {
                // 在指定区域画点，填充部分椭圆
                lcd_draw_point(x + x_t, y + j, color);
                lcd_draw_point(x - x_t, y + j, color);
            }
        }
  
        // 画椭圆中间部分圆弧
        lcd_draw_point(x + x_t, y + y_t, color);
        lcd_draw_point(x - x_t, y - y_t, color);
        lcd_draw_point(x - x_t, y + y_t, color);
        lcd_draw_point(x + x_t, y - y_t, color);
    }

    // 画椭圆两侧部分
    d2 = b_t * b_t * (x_t + 0.5) * (x_t + 0.5) + a_t * a_t * (y_t - 1) * (y_t - 1) - a_t * a_t * b_t * b_t;

    while (y_t > 0)
    {
        if (d2 <= 0)		                                                    // 下一个点在当前点东方
        {
            d2 += b_t * b_t * (2 * x_t + 2) + a_t * a_t * (-2 * y_t + 3);
            x_t++;
  
        }
        else				                                                    // 下一个点在当前点东南方
        {
            d2 += a_t * a_t * (-2 * y_t + 3);
        }
        y_t--;
  
        if (filled)	                                                            // 指定椭圆填充
        {
            // 遍历两侧部分，在指定区域画点，填充部分椭圆
            for (j = -y_t; j < y_t; j++)
            {
                lcd_draw_point(x + x_t, y + j, color);
                lcd_draw_point(x - x_t, y + j, color);
            }
        }
  
        /*画椭圆两侧部分圆弧*/
        lcd_draw_point(x + x_t, y + y_t, color);
        lcd_draw_point(x - x_t, y - y_t, color);
        lcd_draw_point(x - x_t, y + y_t, color);
        lcd_draw_point(x + x_t, y - y_t, color);
    }
}
