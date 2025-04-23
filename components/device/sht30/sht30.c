#include "sht30.h"

#ifdef SH30_USE_HARDWARE_I2C

i2c_master_dev_handle_t g_sht30_i2c_device_handle;

/**
 * @brief SHT30设置周期读取模式
 * 
 * @param read_cmd SHT30读取模式
 */
void sht30_set_periodic_read_mode(uint16_t read_cmd)
{
    uint8_t cmd[2] = {0};
    cmd[0] = read_cmd >> 8;
    cmd[1] = read_cmd & 0xFF;

    bsp_i2c_send_bytes(g_sht30_i2c_device_handle, cmd, 2);
}

/**
 * @brief SHT30读取数据
 * 
 * @param i2c_num I2C端口号
 * @param device_address SHT30设备地址
 * @param read_cmd SHT30读取命令
 * @param temperature 温度值
 * @param humidity 湿度值
 */
void sht30_read_data(uint16_t read_cmd, float *temperature, float *humidity)
{
    uint8_t cmd[2] = {0};
    uint8_t buff[6] = {0};
    uint16_t data = 0;
    
    cmd[0] = read_cmd >> 8;
    cmd[1] = read_cmd & 0xFF;

    bsp_i2c_transmit_receive(g_sht30_i2c_device_handle, cmd, 2, buff, 6);

    //计算温度值
    data = buff[0];
    data = (data << 8) | buff[1];
    *temperature = (data / 65535.0) * 175.0 - 45;

    //计算湿度值
    data = buff[3];
    data = (data << 8) | buff[4];
    *humidity = (data / 65535.0) * 100.0;
}

#else

/**
 * @brief SHT30设置周期读取模式
 * 
 * @param read_mode SHT30读取模式
 */
void sht30_set_periodic_read_mode(uint16_t read_cmd)
{
    bsp_simulate_i2c_start();                                                   // 发送开始信号
    bsp_simulate_i2c_send_one_byte(SHT30_I2C_DEVICE_ADDRESS << 1);              // 发送设备地址+写操作位
    bsp_simulate_i2c_wait_ack();                                                // 等待ACK信号

    // 发送16位命令
    bsp_simulate_i2c_send_one_byte(read_cmd >> 8);                              // 发送命令高8位
    bsp_simulate_i2c_wait_ack();                                                // 等待ACK信号
    bsp_simulate_i2c_send_one_byte(read_cmd & 0xFF);                            // 发送命令低8位
    bsp_simulate_i2c_wait_ack();                                                // 等待ACK信号

    bsp_simulate_i2c_stop();                                                    // 发送结束信号
}

/**
 * @brief SHT30读取数据
 * 
 * @param read_cmd SHT30读取命令，如果是周期读取模式，则发送0xE000命令
 * @param temperature 温度值
 * @param humidity 湿度值
 */
void sht30_read_data(uint16_t read_cmd, float *temperature, float *humidity)
{
    uint8_t buff[6] = {0};
    uint16_t data = 0;

    bsp_simulate_i2c_start();                                                   // 发送开始信号
    bsp_simulate_i2c_send_one_byte(SHT30_I2C_DEVICE_ADDRESS << 1);              // 发送设备地址+写操作位
    bsp_simulate_i2c_wait_ack();                                                // 等待ACK信号

    // 发送16位命令
    bsp_simulate_i2c_send_one_byte(read_cmd >> 8);                              // 发送命令高8位
    bsp_simulate_i2c_wait_ack();                                                // 等待ACK信号
    bsp_simulate_i2c_send_one_byte(read_cmd & 0xFF);                            // 发送命令低8位
    bsp_simulate_i2c_wait_ack();                                                // 等待ACK信号

    bsp_simulate_i2c_start();                                                   // 再次发送开始信号
    bsp_simulate_i2c_send_one_byte(SHT30_I2C_DEVICE_ADDRESS << 1 | 0x01);       // 发送设备地址+读操作位
    bsp_simulate_i2c_wait_ack();                                                // 等待ACK信号

    for (uint8_t i = 0; i < 6; i++)
    {
        buff[i] = bsp_simulate_i2c_read_one_byte(i == 5 ? 1 : 0);               // 接收数据
    }

    bsp_simulate_i2c_stop();                                                    // 发送结束信号

    //计算温度值
    data = buff[0];
    data = (data << 8) | buff[1];
    *temperature = (data / 65535.0) * 175.0 - 45;

    //计算湿度值
    data = buff[3];
    data = (data << 8) | buff[4];
    *humidity = (data / 65535.0) * 100.0;
}

#endif // SH30_USE_HARDWARE_I2C