/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

/**
 * This file holds the main setup() and loop() functions for C++ code.
 * If a RTOS is used, loop() is called in the main task and setup() is called before RTOS initialization.
 * @see App_ThreadX_Init() in Core/Src/app_threadx.c
 * If no RTOS is used, loop() is just called in the main while loop.
 */

#include "main.hpp"
#include "globals.hpp"
#include "Stm32Gpio.hpp"
#include <exception>


Stm32Gpio::PinDigitalIn pinPb0("PB0", A6_D20_GPIO_Port, A6_D20_Pin, true);
Stm32Gpio::PinDigitalIn pinPb1("PB1", A7_D21_GPIO_Port, A7_D21_Pin);

Stm32Gpio::PinAnalogIn pinPa0("PA0", &hadc1, 0);

Stm32Gpio::PinDigitalOut led1("LED1", LED1_GRN_GPIO_Port, LED1_GRN_Pin, true);
Stm32Gpio::PinDigitalOut led2("LED2", LED2_ORG_GPIO_Port, LED2_ORG_Pin, true);
Stm32Gpio::PinDigitalOut led3("LED3", LED3_RED_GPIO_Port, LED3_RED_Pin, true);
Stm32Gpio::PinDigitalOut led4("LED4", LED4_BLU_GPIO_Port, LED4_BLU_Pin, true);


/**
 * @brief Setup function.
 * This function is called once at the beginning of the program before ThreadX is initialized.
 * @see main() in Core/Src/main.c
 */
void setup() {
    dummyCpp = 0;
    dummyCandCpp = 0;

    pinPb0.setup();
    pinPb0.setOnChangeCallback([]() {
        pinPb0.isOn() ? led2.setOn() : led2.setOff();
    });
    pinPb1.setup();
    pinPa0.setup();

    led1.setup();
    led2.setup();
    led3.setup();
    led4.setup();

    led1.setOff();
    led2.setOff();
    led3.setOff();
    led4.setOff();

    // https://elegantinvention.com/blog/information/smaller-binary-size-with-c-on-baremetal-g/
    // std::set_terminate([]() {
        // Error_Handler();
    // });
}


/**
 * @brief This function is the main loop that executes continuously.
 * The function is called inside the mainLoopThread().
 * @see mainLoopThread() in AZURE_RTOS/App/app_azure_rtos.c
 */
void loop() {
    dummyCpp++;
    dummyCandCpp++;

    pinPb0.loop();
    pinPb1.loop();
    pinPa0.loop();
    led1.loop();
    led2.loop();
    led3.loop();
    led4.loop();

    // pinPb0.isOn() ? led2.setOn() : led2.setOff();
    // HAL_GPIO_WritePin(LED2_ORG_GPIO_Port, LED2_ORG_Pin, pinPb0.isOn() ? GPIO_PIN_SET : GPIO_PIN_RESET);

    pinPb1.isOn() ? led3.setOn() : led3.setOff();
    // HAL_GPIO_WritePin(LED3_RED_GPIO_Port, LED3_RED_Pin, pinPb1.isOn() ? GPIO_PIN_SET : GPIO_PIN_RESET);


    auto val=pinPa0.readValue();
    led1.setBlink(val);


    // led1.setBlink(200,200);
    // HAL_GPIO_TogglePin(LED1_GRN_GPIO_Port, LED1_GRN_Pin);
    // HAL_Delay(200);
}


/**
 * @brief This function handles fatal errors.
 * @see Error_Handler() in Core/Src/main.c
 */
void errorHandler() {
    while (true) {
        //        for (uint32_t i = (SystemCoreClock / 10); i > 0; i--) { UNUSED(i); }
    }
}
