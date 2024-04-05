Jhopla?
Print Tu anshil?
Ki me?
Output de.. mi aanto
Roll no wise
Okumura mhade pn pahijey?
Roll no wise change kela?
Ho
Sakshi la vichar na
Msg kr atach
Okumura nahi pahijey
Okay
.
ESP_LOGI(TAG, "Create comparators and generators from the operator");
    mcpwm_cmpr_handle_t comparators[5] = {NULL};
    mcpwm_gen_handle_t generators[5] = {NULL};

    for (int i = 0; i < 5; i++) {
        mcpwm_comparator_config_t comparator_config = {
            .flags.update_cmp_on_tez = true,
        };
        ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparators[i]));

        mcpwm_generator_config_t generator_config = {
            .gen_gpio_num = i == 0 ? SERVO_PULSE_GPIO_1 : (i == 1 ? SERVO_PULSE_GPIO_2 : (i == 2 ? SERVO_PULSE_GPIO_3 : (i == 3 ? SERVO_PULSE_GPIO_4 : SERVO_PULSE_GPIO_5))),
        };
        ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generators[i]));
    }

    for (int i = 0; i < 5; i++)…
mcpwm_generator_config_t generator_config = {
            .gen_gpio_num = (i == 0 ? SERVO_PULSE_GPIO_2 : (i == 1 ? SERVO_PULSE_GPIO_3 : (i == 2 ? SERVO_PULSE_GPIO_4 : SERVO_PULSE_GPIO_5))),
        };
#include "servo.h"

static const char *TAG_SERVO = "servo";
static int enabled_servo_flag = 0;

#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000  // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD        20000    // 20000 ticks, 20ms

mcpwm_cmpr_handle_t comparators[4] = {NULL};

esp_err_t enable_servo()
{
    ESP_LOGI(TAG_SERVO, "Create timer and operator");
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };

    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));

    mcpwm_oper_handle_t oper = NULL;…
/*
 * MIT License
 *
 * Copyright (c)  2021 Society of Robotics and Automation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SERVO_H
#define SERVO_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "esp_attr.h"
#include "driver/mcpwm_prelude.h"

#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_err.h"
#include "pin_defs.h"

typedef struct
{
    int servo_pin;
    int min_pulse_width;
    int max_pulse_width;
    int max_degree;
    int angle;
    int cmp_num;
} servo_config;

/** @struct servo_config
 *  @brief This structure contains the configuration of servos
 *  @var servo_config::servo_pin
 *  Member 'servo_pin' contains the gpio pin number to which servo is connected
 *  @var servo_config::min_pulse_width
 *  Member 'min_pulse_width' contains the minimum pulse width of servo motor
 *  @var servo_config::max_pulse_width
 *  Member 'max_pulse_width' contains the maximum pulse width of servo motor
 *  @var servo_config::max_degree
 *  Member 'max_degree' contains the maximum degree servo motor can rotate
 *  @var servo_config::mcpwm_num
 *  Member 'mcpwm_num' contains MCPWM unit to use
 *  @var servo_config::timer_num
 *  Member 'timer_num' contains MCPWM timer to use
 *  @var servo_config::gen
 *  Member 'gen' contains MCPWM operator to use
 */

/**
 * @brief Enables Servo port on the sra board, sets up PWM for the three pins in servo port.
 *
 * @return esp_err_t - returns ESP_OK if servo pins initialised, else it returns ESP_ERR_INVALID_ARG
 **/
esp_err_t enable_servo();

/**
 * @brief Set the angle of the servos attached to the servo port of SRA Board
 *
 * @param config pointer to the servo_config struct
 * @param degree_of_rotation angle to which the servo must be set, depends on value of MAX_DEGREE macro
 * @return esp_err_t
 */
esp_err_t set_angle_servo(servo_config *config, unsigned int degree_of_rotation);

/**
 * @brief Get the angle of the servos
 * @return esp_err_t
 */
int read_servo(servo_config *config);

#endif
start porting servo.c code into esp idf v5.1
#include "sra_board.h"

#define TAG "MCPWM_SERVO_CONTROL"

servo_config servo_a = {
	.servo_pin = SERVO_A,
	.min_pulse_width = CONFIG_SERVO_A_MIN_PULSEWIDTH,
	.max_pulse_width = CONFIG_SERVO_A_MAX_PULSEWIDTH,
	.max_degree = CONFIG_SERVO_A_MAX_DEGREE,
	.cmp_num=0,
	// .mcpwm_num = MCPWM_UNIT_0,
	// .timer_num = MCPWM_TIMER_0,
	// .gen = MCPWM_OPR_A,
};

servo_config servo_b = {
	.servo_pin = SERVO_B,
	.min_pulse_width = CONFIG_SERVO_B_MIN_PULSEWIDTH,
	.max_pulse_width = CONFIG_SERVO_B_MAX_PULSEWIDTH,
	.max_degree = CONFIG_SERVO_B_MAX_DEGREE,
	.cmp_num=1,
// 	.mcpwm_num = MCPWM_UNIT_0,
// 	.timer_num = MCPWM_TIMER_0,
// 	.gen = MCPWM_OPR_B,
};

servo_config servo_c = {
	.servo_pin = SERVO_C,
	.min_pulse_width = CONFIG_SERVO_C_MIN_PULSEWIDTH,
	.max_pulse_width = CONFIG_SERVO_C_MAX_PULSEWIDTH,
	.max_degree = CONFIG_SERVO_C_MAX_DEGREE,
	.cmp_num=2,
// 	.mcpwm_num = MCPWM_UNIT_0,
// 	.timer_num = MCPWM_TIMER_1,
// 	.gen = MCPWM_OPR_A,
};

servo_config servo_d = {
	.servo_pin = SERVO_D,
	.min_pulse_width = CONFIG_SERVO_D_MIN_PULSEWIDTH,
	.max_pulse_width = CONFIG_SERVO_D_MAX_PULSEWIDTH,
	.max_degree = CONFIG_SERVO_D_MAX_DEGREE,
	.cmp_num=3,
	// .mcpwm_num = MCPWM_UNIT_0,
	// .timer_num = MCPWM_TIMER_1,
	// .gen = MCPWM_OPR_B,
};

static void mcpwm_servo_control(void *arg)
{
	enable_servo();
#ifdef CONFIG_ENABLE_OLED
	// Initialising the OLED
	ESP_ERROR_CHECK(init_oled());
	display_logo(MARIO_LOGO);
	vTaskDelay(100);
#endif

	while (1)
	{
		set_angle_servo(&servo_a, 0);
		vTaskDelay(100);
		set_angle_servo(&servo_b, 0);
		vTaskDelay(100);
		set_angle_servo(&servo_c, 0);
		vTaskDelay(100);
		set_angle_servo(&servo_d, 0);
		vTaskDelay(100);
	}
}

void app_main()
{
	ESP_LOGD(TAG, "Testing servo motors\n");
	xTaskCreate(mcpwm_servo_control, "mcpwm_example_servo_control", 4096, NULL, 5, NULL);
}
