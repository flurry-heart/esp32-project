#include "bsp_spi.h"

/**
 * @brief 初始化SPI
 * 
 * @param host_id SPI总线的主机设备ID
 * @param sclk_io_num SPI的SCLK引脚
 * @param miso_io_num SPI的MISO引脚
 * @param mosi_io_num SPI的MOSI引脚
 */
void bsp_spi_init(spi_host_device_t host_id, gpio_num_t spi_sclk_io_num, gpio_num_t spi_miso_io_num, gpio_num_t spi_mosi_io_num)
{
    spi_bus_config_t spi_bus_config = {0};

    // SPI总线配置
    spi_bus_config.sclk_io_num = spi_sclk_io_num;                               // SPI的SCLK引脚
    spi_bus_config.miso_io_num = spi_miso_io_num;                               // SPI的MISO引脚
    spi_bus_config.mosi_io_num = spi_mosi_io_num;                               // SPI的MOSI引脚
    spi_bus_config.quadwp_io_num = -1;                                          // SPI写保护信号引脚，该引脚未使能
    spi_bus_config.quadhd_io_num = -1;                                          // SPI保持信号引脚，该引脚未使能
    spi_bus_config.max_transfer_sz = 1024 * 1;                                  // 配置最大传输大小，以字节为单位
    spi_bus_initialize(host_id, &spi_bus_config, SPI_DMA_CH_AUTO);
}

/**
 * @brief SPI设备接口配置函数
 * 
 * @param device_handle SPI设备句柄
 * @param host_id SPI总线的主机设备ID
 * @param cs_gpio_num SPI的片选引脚
 * @param mode SPI的工作模式
 * @param clock_speed SPI的时钟频率
 */
void bsp_spi_bus_add_device(spi_device_handle_t *device_handle, spi_host_device_t host_id, gpio_num_t cs_gpio_num, uint8_t mode, int clock_speed)
{
    spi_device_interface_config_t spi_device_interface_config = {0};
    
    spi_device_interface_config.spics_io_num = cs_gpio_num;                     // SPI的片选引脚
    spi_device_interface_config.mode = mode;                                    // SPI的工作模式
    spi_device_interface_config.clock_speed_hz = clock_speed;                   // SPI的时钟频率
    spi_device_interface_config.queue_size = 8;                                 // 事务队列大小

    spi_bus_add_device(host_id, &spi_device_interface_config, device_handle);   // 添加SPI设备
}

/**
 * @brief SPI发送一个字节数据
 * 
 * @param device_handle SPI设备句柄
 * @param data 要发送的一个字节的数据
 */
void bsp_spi_send_one_byte(spi_device_handle_t device_handle, uint8_t data)
{
    spi_transaction_t spi_transaction = {0};

    spi_transaction.length = 8;                                                 // 要传输的位数，一个字节8位
    spi_transaction.tx_buffer = &data;                                          // 要传输的数据
    spi_device_polling_transmit(device_handle, &spi_transaction);               // 发送数据
}

/**
 * @brief SPI发送多个字节数据
 * 
 * @param device_handle SPI设备句柄
 * @param data 要发送的多个字节的数据的缓冲区
 */
void bsp_spi_send_bytes(spi_device_handle_t device_handle, const uint8_t *data, uint16_t length)
{
    spi_transaction_t spi_transaction = {0};
    
    spi_transaction.length = length * 8;                                        // 要传输的位数，一个字节8位
    spi_transaction.tx_buffer = data;                                           // 将命令填充进去
    spi_device_polling_transmit(device_handle, &spi_transaction);               // 开始传输
}

/**
 * @brief SPI传输一个字节数据
 * 
 * @param device_handle SPI设备句柄
 * @param data 要传输的一个字节的数据
 * @return uint8_t 接收的数据
 */
uint8_t bsp_spi_transfer_one_byte(spi_device_handle_t device_handle, uint8_t data)
{
    spi_transaction_t spi_transaction = {0};

    spi_transaction.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA;
    spi_transaction.length = 8;
    spi_transaction.tx_data[0] = data;
    spi_device_transmit(device_handle, &spi_transaction);

    return spi_transaction.rx_data[0];
}

/**
 * @brief SPI初始化函数
 * 
 */
void bsp_simulate_spi_init(void)
{
    gpio_config_t gpio_config_struct = {0};

    gpio_config_struct.pin_bit_mask = (1ULL << SPI_SCK_GPIO_NUM) | (1ULL << SPI_MOSI_GPIO_NUM); // 设置引脚
    gpio_config_struct.intr_type = GPIO_INTR_DISABLE;                                           // 不使用中断
    gpio_config_struct.mode = GPIO_MODE_OUTPUT;                                                 // 输出模式
    gpio_config_struct.pull_up_en = GPIO_PULLUP_DISABLE;                                        // 不使用上拉
    gpio_config_struct.pull_down_en = GPIO_PULLUP_DISABLE;                                      // 不使用下拉
    gpio_config(&gpio_config_struct);                                                           // 配置GPIO

    gpio_config_struct.pin_bit_mask = (1ULL << SPI_MISO_GPIO_NUM);
    gpio_config_struct.mode = GPIO_MODE_INPUT;
    gpio_config(&gpio_config_struct);

#if SIMULATE_SPI_MODE == SIMULATE_SPI_MODE0 || SIMULATE_SPI_MODE == SIMULATE_SPI_MODE1
    SPI_SCK(0);                                                                 // SPI的SCK引脚默认为低电平，选择工作模式0或1
#elif SIMULATE_SPI_MODE == SIMULATE_SPI_MODE2 || SIMULATE_SPI_MODE == SIMULATE_SPI_MODE3
    SPI_SCK(1);                                                                 // SPI的SCK引脚默认为高电平，选择工作模式2或3
#endif
}

/**
 * @brief SPI交换一个字节函数
 * 
 * @param data 待交换的数据
 * @return uint8_t 交换后的数据
 */
uint8_t bsp_simulate_spi_swap_one_byte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
#if SIMULATE_SPI_MODE == SIMULATE_SPI_MODE1
        // SCK上升沿
        SPI_SCK(1);
#elif SIMULATE_SPI_MODE == SIMULATE_SPI_MODE3
        // SCK下降沿
        SPI_SCK(0);
#endif
        // 移出数据
        SPI_MOSI(data & 0x80);
        data <<= 1;

#if SIMULATE_SPI_MODE == SIMULATE_SPI_MODE0 || SIMULATE_SPI_MODE == SIMULATE_SPI_MODE3
        // SCK上升沿
        SPI_SCK(1);
#elif SIMULATE_SPI_MODE == SIMULATE_SPI_MODE1 || SIMULATE_SPI_MODE == SIMULATE_SPI_MODE2
        // SCK下降沿
        SPI_SCK(0);
#endif

        // 移入数据
        if (SPI_MISO())
        {
            data |= 0x01;
        }

#if SIMULATE_SPI_MODE == SIMULATE_SPI_MODE0
        // SCK下降沿
        SPI_SCK(0);
#elif SIMULATE_SPI_MODE == SIMULATE_SPI_MODE2
        // SCK上升沿
        SPI_SCK(1);
#endif
    }

    return data;
}