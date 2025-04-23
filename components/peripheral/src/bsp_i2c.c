#include "bsp_i2c.h"

i2c_master_bus_handle_t g_i2c_master_bus_handle;

/**
 * @brief I2C初始化
 * 
 * @param handle I2C控制器句柄
 * @param i2c_num I2C控制器编号
 * @param scl_gpio_num SCL引脚编号
 * @param sda_gpio_num SDA引脚编号
 */
void bsp_i2c_init(i2c_master_bus_handle_t *bus_handle, i2c_port_t i2c_num, gpio_num_t scl_gpio_num, gpio_num_t sda_gpio_num)
{
    i2c_master_bus_config_t i2c_master_config = {0};

    i2c_master_config.i2c_port = i2c_num;                                       // I2C控制器编号
    i2c_master_config.scl_io_num = scl_gpio_num;                                // SCL引脚编号
    i2c_master_config.sda_io_num = sda_gpio_num;                                // SDA引脚编号
    i2c_master_config.clk_source = I2C_CLK_SRC_DEFAULT;                         // 时钟源
    i2c_master_config.glitch_ignore_cnt = 7;                                    // 滤波器宽度
    i2c_master_config.flags.enable_internal_pullup = true;                      // 启用内部上拉
    i2c_new_master_bus(&i2c_master_config, bus_handle);
}

/**
 * @brief I2C总线添加设备
 * 
 * @param device_handle I2C设备句柄
 * @param bus_handle I2C总线句柄
 * @param device_address I2C设备地址
 * @param device_address_length I2C设备地址长度
 * @param mode I2C设备通信模式
 * @param clock_speed I2C设备通信时钟频率
 * @param scl_wait_us I2C设备通信时，SCL线等待的时间
*/
void bsp_i2c_bus_add_device(i2c_master_dev_handle_t *device_handle, 
                            i2c_master_bus_handle_t bus_handle, 
                            uint16_t device_address, 
                            i2c_addr_bit_len_t device_address_length, 
                            int clock_speed, 
                            uint32_t scl_wait_us)
{
    i2c_device_config_t i2c_device_config = {0};
    
    i2c_device_config.device_address = device_address;                          // I2C设备的设备地址
    i2c_device_config.dev_addr_length = device_address_length;                  // I2C设备的设备长度
    i2c_device_config.scl_speed_hz = clock_speed;                               // I2C设备通信的时钟频率
    i2c_device_config.scl_wait_us = scl_wait_us;                                // I2C设备通信时，SCL线等待的时间
    i2c_master_bus_add_device(bus_handle, &i2c_device_config, device_handle);   // 添加I2C设备
}

/**
 * @brief I2C发送一个字节数据
 * 
 * @param device_handle I2C设备句柄
 * @param data 要发送的数据
 */
void bsp_i2c_send_one_byte(i2c_master_dev_handle_t device_handle, uint8_t data)
{
    i2c_master_transmit(device_handle, &data, 1, 1000);
}

/**
 * @brief I2C发送多个字节数据
 * 
 * @param device_handle I2C设备句柄
 * @param data 要发送的多个字节的数据的缓冲区
 */
void bsp_i2c_send_bytes(i2c_master_dev_handle_t device_handle, const uint8_t *data, uint16_t length)
{
    i2c_master_transmit(device_handle, data, length, 1000);
}

/**
 * @brief I2C读取一个字节数据
 * 
 * @param device_handle I2C设备句柄
 * @return uint8_t 读取的数据
 */
uint8_t bsp_i2c_receive_one_byte(i2c_master_dev_handle_t device_handle)
{
    uint8_t data = {0};
    i2c_master_receive(device_handle, &data, 1, 1000);
    return data;
}

/**
 * @brief I2C读取多个字节数据
 * 
 * @param device_handle I2C设备句柄
 * @param data 要发送的多个字节的数据的缓冲区
 */
void bsp_i2c_receive_bytes(i2c_master_dev_handle_t device_handle, uint8_t *data, uint16_t length)
{
    i2c_master_receive(device_handle, data, length, 1000);
}

/**
 * @brief I2C发送和接收多个字节数据
 * 
 * @param device_handle I2C设备句柄
 * @param write_buffer 要发送的多个字节的数据的缓冲区
 * @param write_size 要发送的多个字节的数据长度
 * @param read_buffer 要接收的多个字节的数据的缓冲区
 * @param read_size 要接收的多个字节的数据长度
 */
void bsp_i2c_transmit_receive(i2c_master_dev_handle_t device_handle, uint8_t *write_buffer, size_t write_size, uint8_t *read_buffer, size_t read_size)
{
    i2c_master_transmit_receive(device_handle, write_buffer, write_size, read_buffer, read_size, 1000);
}

/**
 * @brief 模拟I2C初始化函数
 * 
 */
void bsp_simulate_i2c_init(void)
{
    gpio_config_t gpio_config_struct = {0};

    gpio_config_struct.pin_bit_mask = (1ULL << I2C_SCL_GPIO_NUM) | (1ULL << I2C_SDA_GPIO_NUM);  // 设置引脚
    gpio_config_struct.intr_type = GPIO_INTR_DISABLE;                                           // 不使用中断
    gpio_config_struct.mode = GPIO_MODE_INPUT_OUTPUT_OD;                                        // 开漏输入输出模式
    gpio_config_struct.pull_up_en = GPIO_PULLUP_ENABLE;                                         // 使用上拉
    gpio_config_struct.pull_down_en = GPIO_PULLUP_DISABLE;                                      // 不使用下拉
    gpio_config(&gpio_config_struct);                                                           // 配置GPIO

    // 空闲时，I2C总线SCL为高电平，I2C SDA为高电平
    I2C_SCL(1);
    I2C_SDA(1);
}

/**
 * @brief I2C产生起始信号函数
 * 
 * @note SCL为高电平期间，SDA从高电平往低电平跳变
 */
void bsp_simulate_i2c_start(void)
{
    // 1、释放SDA和SCL，并延迟，空闲状态
    I2C_SDA(1);
    I2C_SCL(1); 
    I2C_DELAY();
    // 2、拉低SDA，SDA产生下降沿，并延迟
    I2C_SDA(0);
    I2C_DELAY();
    // 3、钳住SCL总线，准备发送数据/接收数据，并延时
    I2C_SCL(0);
    I2C_DELAY();
}

/**
 * @brief I2C产生结束信号函数
 * 
 * @note SCL为高电平期间，SDA从低电平往高电平跳变
 */
void bsp_simulate_i2c_stop(void)
{
    // 1、SDA拉低，SCL拉高，并延迟
    I2C_SDA(0);
    I2C_SCL(1);
    I2C_DELAY();
    // 2、拉高SDA，产生上升沿，并延迟
    I2C_SDA(1);
    I2C_DELAY();
}

/**
 * @brief 主机检测应答信号
 * 
 * @return uint8_t 0，接收应答成功；1，接收应答失败
 */
uint8_t bsp_simulate_i2c_wait_ack(void)
{
    uint8_t wait_time = 0;
    uint8_t ack = 0;

    // 1、主机释放SDA数据线并延迟，此时外部器件可以拉低SDA线
    I2C_SDA(1);
    I2C_DELAY();
    // 2、主机拉高SCL，此时从机可以返回ACK
    I2C_SCL(1);
    I2C_DELAY();

    // 3、SCL高电平期间主机读取SDA状态，等待应答
    while (I2C_READ_SDA())        
    {
        // 4、如果超时的话，就直接产生结束信号，非应答
        wait_time++;
        if (wait_time > 250)
        {
            bsp_simulate_i2c_stop();
            ack = 1;
            break;
        }
    }
    // 5、SCL=0，结束ACK检查
    I2C_SCL(0);
    I2C_DELAY();

    // 6、返回是否接收到应答信号
    return ack;
}

/**
 * @brief 发送应答信号或非应答信号
 * 
 * @param ack 0，发送应答信号；1，发送非应答信号
 */
void bsp_simulate_i2c_send_ack(uint8_t ack)
{
    // 1、拉低SDA，表示应答，拉高SDA，表示非应答，并延迟
    I2C_SDA(ack);
    I2C_DELAY();
    // 2、主机拉高SCL线，并延迟，确保从机能有足够时间去接收SDA线上的电平信号
    I2C_SCL(1);
    I2C_DELAY();
    // 3、主机拉低时钟线并延时，完成这一位数据的传送
    I2C_SCL(0);
    I2C_DELAY();
    // 4、释放SDA线，并延迟
    I2C_SDA(1);
    I2C_DELAY();
}

/**
 * @brief I2C读取一个字节函数
 * 
 * @param ack 0，发送应答信号，1，发送非应答信号
 * @return uint8_t 
 */
uint8_t bsp_simulate_i2c_read_one_byte(uint8_t ack)
{
    uint8_t receive = 0;

    // 1、主机释放SDA
    I2C_SDA(1);

    for (uint8_t i = 0; i < 8; i++)
    {
        // 2、释放SCL，主机将在SCL高电平期间读取数据位
        I2C_SCL(1);
        I2C_DELAY();
        // 3、读取SDA
        if (I2C_READ_SDA())
        {
            receive |= 0x80 >> i;
        }
        // 4、拉低SCL，从机切换SDA线输出数据
        I2C_SCL(0);
        I2C_DELAY();
    }

    // 5、发送应答信号或非应答信号
    bsp_simulate_i2c_send_ack(ack);

    // 6、返回读取的数据
    return receive;
}

/**
 * @brief I2C发送一个字节函数
 * 
 * @param data 待发送的数据
 */
void bsp_simulate_i2c_send_one_byte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        // 1、发送数据位的高位
        I2C_SDA((data & 0x80) >> 7);
        I2C_DELAY();
        // 2、释放SCL，从机将在SCL高电平期间读取数据位
        I2C_SCL(1);
        I2C_DELAY();
        // 3、拉低SCL
        I2C_SCL(0);
        // 4、数据左移一位，用于下次发送
        data <<= 1;
    }
    // 5、发送完成，主机释放SDA线
    I2C_SDA(1);
}