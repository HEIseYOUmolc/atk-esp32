/*
 * Copyright(C) 2026 ZhuiGuang Elec. All rights reserved.
 */
/*
 * config.h
 * Original Author: yaozhihui 1009764538@qq.com, 2026年02月08日
 *
 *
 *
 * History
 *  v1.0    yaozhihui 1009764538@qq.com, 2026年02月08日
 *          创建初稿
 */

#ifndef __MAIN_BOARDS_ATK_ROBOT_CONFIG_H_
#define __MAIN_BOARDS_ATK_ROBOT_CONFIG_H_

#include <driver/gpio.h>

/* 按钮配置 */
#define BOOT_BUTTON_GPIO        GPIO_NUM_0

/* 显示屏配置 */
#define DISPLAY_BACKLIGHT_PIN GPIO_NUM_21
#define DISPLAY_MOSI_PIN      GPIO_NUM_17
#define DISPLAY_CLK_PIN       GPIO_NUM_16
#define DISPLAY_DC_PIN        GPIO_NUM_14
#define DISPLAY_RST_PIN       GPIO_NUM_18
#define DISPLAY_CS_PIN        GPIO_NUM_15

#define LCD_TYPE_ST7789_SERIAL
#define DISPLAY_WIDTH   240
#define DISPLAY_HEIGHT  320
#define DISPLAY_MIRROR_X false
#define DISPLAY_MIRROR_Y false
#define DISPLAY_SWAP_XY false
#define DISPLAY_INVERT_COLOR    true
#define DISPLAY_RGB_ORDER  LCD_RGB_ELEMENT_ORDER_RGB
#define DISPLAY_OFFSET_X  0
#define DISPLAY_OFFSET_Y  0
#define DISPLAY_BACKLIGHT_OUTPUT_INVERT false
#define DISPLAY_SPI_MODE 0

/* 音频配置 */
#define AUDIO_INPUT_SAMPLE_RATE  24000
#define AUDIO_OUTPUT_SAMPLE_RATE 24000

#define AUDIO_I2S_GPIO_MCLK GPIO_NUM_5
#define AUDIO_I2S_GPIO_WS GPIO_NUM_39
#define AUDIO_I2S_GPIO_BCLK GPIO_NUM_4
#define AUDIO_I2S_GPIO_DIN  GPIO_NUM_40
#define AUDIO_I2S_GPIO_DOUT GPIO_NUM_41
#define AUDIO_CODEC_PA_PIN GPIO_NUM_6

#define AUDIO_CODEC_I2C_SDA_PIN  GPIO_NUM_3
#define AUDIO_CODEC_I2C_SCL_PIN  GPIO_NUM_2
#define AUDIO_CODEC_ES8311_ADDR  ES8311_CODEC_DEFAULT_ADDR

#endif /* __MAIN_BOARDS_ATK_ROBOT_CONFIG_H_ */
