#include "lvgl_demo.h"

/**
 * LVGL Demo Task 任务配置
 * 包括: 任务优先级 任务栈大小 任务句柄 任务函数
 */
#define LVGL_DEMO_TASK_PRIORITY     1
#define LVGL_DEMO_TASK_STACK_SIZE   1024 * 6

TaskHandle_t g_lvgl_demo_task_handle;

void lvgl_demo_task(void *pvParameters);

lv_indev_t *indev_encoder;

static void increase_lvgl_tick(void *arg);

static void lv_port_disp_init(void);
static void disp_init(void);
static void disp_flush(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);

static void lv_port_indev_init(void);
static void encoder_init(void);
static void encoder_read(lv_indev_t * indev, lv_indev_data_t * data);

/**
 * @brief LVGL程序入口
 * 
 */
void lvgl_demo(void)
{
    // 动态创建任务
    xTaskCreate((TaskFunction_t        ) lvgl_demo_task,                        // 任务函数
                (char *                ) "lvgl_demo_task",                      // 任务名
                (configSTACK_DEPTH_TYPE) LVGL_DEMO_TASK_STACK_SIZE,             // 任务栈大小
                (void *                ) NULL,                                  // 入口参数
                (UBaseType_t           ) LVGL_DEMO_TASK_PRIORITY,               // 任务优先级
                (TaskHandle_t *        ) &g_lvgl_demo_task_handle);             // 任务句柄
}

/**
 * @brief LVGL Demo的任务函数
 * 
 * @param pvParameters 任务函数的入口参数
 */
void lvgl_demo_task(void *pvParameters)
{
    esp_timer_handle_t lvgl_tick_timer = NULL;

    lv_init();                                                                  // 初始化LVGL图形库
    lv_port_disp_init();                                                        // LVGL显示接口初始化，放在lv_init()的后面
    lv_port_indev_init();                                                       // LVGL输入接口初始化，放在lv_init()的后面

    // 为LVGL提供时基单元
    const esp_timer_create_args_t lvgl_tick_timer_args = 
    {
        .callback = &increase_lvgl_tick,                                        // 设置定时器回调
        .name = "lvgl_tick"                                                     // 设置定时器名称
    };
    
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer)); // 创建定时器
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, 1 * 1000));       // 启动定时器

    // 测试代码

    // 使用编码器作为输入设备时，我们应该使用 lv_group_t * group = lv_group_create() 创建组
    lv_group_t * group = lv_group_create();
    lv_indev_set_group(indev_encoder, group);                                   // 将编码器输入设备分配给组
    lv_group_set_default(group);                                                // 设置默认组

    lv_obj_t *switch_obj = lv_switch_create(lv_scr_act());                      // 创建开关部件
    lv_obj_align(switch_obj, LV_ALIGN_TOP_MID, 0, 0);                           // 开关部件顶部居中对齐
    lv_group_add_obj(group, switch_obj);                                        // 将按钮部件加入到组中

    lv_obj_t *dropdown_list_obj = lv_dropdown_create(lv_scr_act());             // 创建下拉列表部件
    lv_obj_center(dropdown_list_obj);                                           // 下拉列表部件居中对齐
    lv_group_add_obj(group, dropdown_list_obj);                                 // 将下拉列表部件加入到组中

    lv_dropdown_set_options(dropdown_list_obj, "Sakura\nMikoto\nShana");        // 设置下拉列表选项
    lv_dropdown_set_selected(dropdown_list_obj, 1);                             // 设置当前的选中项

    lv_dropdown_set_dir(dropdown_list_obj, LV_DIR_BOTTOM);                      // 设置展开方式
    lv_dropdown_set_symbol(dropdown_list_obj, LV_SYMBOL_RIGHT);                 // 设置图标

    while (1)
    {
        lv_timer_handler();                                                     // LVGL计时器
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/**
 * @brief LVGL定时器回调函数，用于提供时基单元。每1ms调用一次此函数。
 * 
 * @param arg LVGL定时器回调函数参数，此处不需要使用。
 */
static void increase_lvgl_tick(void *arg)
{
    lv_tick_inc(1);                                                             // 告诉LVGL已经过了多少毫秒
}

/**
 * @brief LVGL显示接口初始化
 * 
 */
static void lv_port_disp_init(void)
{
    disp_init();                                                                // 初始化显示设备

    /**
     * LVGL需要一个缓冲区用来绘制小部件
     * 随后，这个缓冲区的内容会通过显示设备的 'flush_cb'(显示设备刷新函数) 复制到显示设备上
     * 这个缓冲区的大小需要大于显示设备一行的大小
     */
    lv_display_t * disp = lv_display_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
    lv_display_set_flush_cb(disp, disp_flush);

    /**
     *
     * 双缓冲区: LVGL 会将显示设备的内容绘制到其中一个缓冲区，并将它写入显示设备。
     *          需要使用 DMA 将要显示在显示设备的内容写入缓冲区。
     *          当数据从第一个缓冲区发送时，它将使 LVGL 能够将屏幕的下一部分绘制到另一个缓冲区。
     *          这样使得渲染和刷新可以并行执行。
     */
    static uint8_t buf_2_1[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];
    static uint8_t buf_2_2[MY_DISP_HOR_RES * 10 * BYTE_PER_PIXEL];
    lv_display_set_buffers(disp, buf_2_1, buf_2_2, sizeof(buf_2_1), LV_DISPLAY_RENDER_MODE_PARTIAL);
}

/**
 * @brief LVGL显示设备初始化函数实现部分
 * 
 */
static void disp_init(void)
{
    bsp_spi_init(SPI2_HOST, GPIO_NUM_1, GPIO_NUM_NC, GPIO_NUM_2);
    bsp_spi_bus_add_device(&g_lcd_spi_device_handle, SPI2_HOST, LCD_CS_GPIO_NUM, 0, 60000000);

    lcd_init();
}

/**
 * @brief LVGL显示设备刷新函数
 * 
 * @param disp_drv 显示设备驱动结构体
 * @param area 绘制区域
 * @param px_map 绘制的像素数据
 */
static void disp_flush(lv_display_t * disp_drv, const lv_area_t * area, uint8_t * px_map)
{
    lcd_show_picture(px_map, area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    lv_display_flush_ready(disp_drv);
}

/**
 * @brief LVGL输入接口初始化
 * 
 */
void lv_port_indev_init(void)
{
    // 编码器输入设备初始化，支持左旋、右旋和按下
    encoder_init();

    // 注册编码器输入设备
    indev_encoder = lv_indev_create();
    lv_indev_set_type(indev_encoder, LV_INDEV_TYPE_ENCODER);
    lv_indev_set_read_cb(indev_encoder, encoder_read);
}

/**
 * @brief 初始化编码器输入设备
 * 
 */
static void encoder_init(void)
{
    rotary_encoder_init(GPIO_NUM_15, GPIO_NUM_16, GPIO_NUM_17);
}

/**
 * @brief 读取编码器数值函数
 * 
 * @param indev_drv 编码器输入设备
 * @param data 传递给输入驱动程序进行填充的数据结构
 */
static void encoder_read(lv_indev_t * indev_drv, lv_indev_data_t * data)
{
    int16_t encoder_diff = 0;                                                   // 记录编码器的值
    lv_indev_state_t encoder_state = LV_INDEV_STATE_RELEASED;                   // 记录编码器的状态
    
    uint8_t result = rotary_encoder_scan(GPIO_NUM_15, GPIO_NUM_16, GPIO_NUM_17);

    switch (result)
    {
    case 1:
        encoder_diff = -1;
        break;
    
    case 2:
        encoder_diff = 1;
        break;

    case 3:
        encoder_diff = 0;
        encoder_state = LV_INDEV_STATE_PRESSED;
        break;
    }

    data->enc_diff = encoder_diff;
    data->state = encoder_state;
}