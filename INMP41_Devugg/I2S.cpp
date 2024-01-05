#include "I2S.h"

void I2S_Init(i2s_mode_t MODE, i2s_bits_per_sample_t BPS)
{
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = BPS,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = 0,
    .dma_buf_count = 64,
    .dma_buf_len = 1024,
    .use_apll = 1
  };
  if (MODE == I2S_MODE_RX || MODE == I2S_MODE_TX) {
    Serial.println("using I2S_MODE");
    const i2s_pin_config_t pin_config = {
      .bck_io_num = 32,
      .ws_io_num = 25,
      .data_out_num = -1,
      .data_in_num = 33
    };
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
    i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, BPS, I2S_CHANNEL_STEREO);
  }
}

int I2S_Read(char *data, int numData)
{
  size_t bytes_read;
  return i2s_read(I2S_NUM_0, (char *)data, numData, &bytes_read, portMAX_DELAY);
}

void I2S_Write(char *data, int numData)
{
  size_t bytes_write;
  i2s_write(I2S_NUM_0, (const char *)data, numData, &bytes_write, portMAX_DELAY);
}
