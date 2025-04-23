#ifndef __LCD_H__
#define __LCD_H__

#include <string.h>
#include <stdio.h>
#include <stdarg.h> 

#include "bsp_spi.h"

#include "screen/color.h"
#include "screen/font.h"
#include "screen/image.h"
#include "geometry/geometry.h"

#define LCD_USE_HARDWARE_SPI

#define LCD_CS_GPIO_NUM         GPIO_NUM_4
#define LCD_CS(x)               do{ x ? \
                                    gpio_set_level(LCD_CS_GPIO_NUM, 1) : \
                                    gpio_set_level(LCD_CS_GPIO_NUM, 0); \
                                }while(0)

#define LCD_DC_GPIO_NUM         GPIO_NUM_5
#define LCD_DC(x)               do{ x ? \
                                    gpio_set_level(LCD_DC_GPIO_NUM, 1) : \
                                    gpio_set_level(LCD_DC_GPIO_NUM, 0); \
                                }while(0)

#define LCD_RESET_GPIO_NUM      GPIO_NUM_6
#define LCD_RESET(x)            do{ x ? \
                                    gpio_set_level(LCD_RESET_GPIO_NUM, 1) : \
                                    gpio_set_level(LCD_RESET_GPIO_NUM, 0); \
                                }while(0)

#define LCD_BLK_GPIO_NUM        GPIO_NUM_7
#define LCD_BLK(x)              do{ x ? \
                                    gpio_set_level(LCD_BLK_GPIO_NUM, 1) : \
                                    gpio_set_level(LCD_BLK_GPIO_NUM, 0); \
                                }while(0)

#define LCD_WIDTH               128
#define LCD_HEIGHT              160

typedef enum LCD_Write_Mode_t
{
    LCD_MODE_CMD = 0,
    LCD_MODE_DATA = 1
} lcd_write_mode_t;

typedef enum LCD_Display_Mode_t
{
    LCD_DISPLAY_NORMAL = 0,
    LCD_DISPLAY_OVERLAPPING = 1
} lcd_display_mode_t;

typedef struct LCD_Data_Format_t
{
    uint8_t digit;                                                              // 位数
    uint8_t decimal;                                                            // 小数位数
    uint8_t symbol : 1;                                                         // 是否显示符号
    uint8_t align_right : 1;                                                    // 是否右对齐
    uint8_t display_mode : 1;                                                   // 是否叠加显示
} lcd_data_format_t;

extern spi_device_handle_t g_lcd_spi_device_handle;

void lcd_init(void);
void lcd_reset(void);

void lcd_write_byte(uint8_t data, lcd_write_mode_t mode);
void lcd_write_bytes(uint8_t *data, uint32_t length, lcd_write_mode_t mode);
uint8_t lcd_read_one_byte(void);

void lcd_set_cursor(uint32_t x, uint32_t y);
void lcd_set_cursor_area(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);
void lcd_display_direction(uint8_t mode);
void lcd_clear(uint16_t color);
void lcd_clear_area(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color);

void lcd_draw_point(uint32_t x, uint32_t y, uint16_t color);
void lcd_show_char(uint32_t x, uint32_t y, char chr, uint16_t size, uint16_t forecolor, uint16_t backcolor, lcd_display_mode_t display_mode);
void lcd_show_string(uint32_t x, uint32_t y, char *str, uint16_t size, uint16_t forecolor, uint16_t backcolor, lcd_display_mode_t display_mode);
void lcd_show_chinese(uint32_t x, uint32_t y, char *chinese, uint16_t size, uint16_t forecolor, uint16_t backcolor, lcd_display_mode_t display_mode);
void lcd_show_number(uint32_t x, uint32_t y, int num, uint16_t size, uint16_t forecolor, uint16_t backcolor, lcd_data_format_t data_format);
void lcd_show_decimal(uint32_t x, uint32_t y, double decimal, uint16_t size, uint16_t forecolor, uint16_t backcolor, lcd_data_format_t data_format);
void lcd_printf(uint32_t x, uint32_t y, uint16_t size, uint16_t forecolor, uint16_t backcolor, lcd_display_mode_t display_mode, char *format, ...);

void lcd_show_picture(uint8_t image[], uint32_t x, uint32_t y, uint32_t width, uint32_t height);

void lcd_draw_line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color);
void lcd_draw_rectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint16_t color, bool filled);
void lcd_draw_triangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint16_t color, bool filled);
void lcd_draw_ellipse(uint32_t x, uint32_t y, uint32_t a, uint32_t b, uint16_t color, bool filled);

#endif // !__LCD_H__
