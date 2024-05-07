/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#ifndef LIBSMART_STM32GPIO_PINDIGITALOUT_H
#define LIBSMART_STM32GPIO_PINDIGITALOUT_H

#include "PinDigital.hpp"

namespace Stm32Gpio {
    class PinDigitalOut : public PinDigital {
    public:
        PinDigitalOut(GPIO_TypeDef *GPIOx, const uint16_t GPIO_Pin)
            : PinDigital(GPIOx, GPIO_Pin, pinModeType::DIGITAL_OUT) {
        }

        PinDigitalOut(GPIO_TypeDef *GPIOx, const uint16_t GPIO_Pin, const bool isInverted)
            : PinDigital(GPIOx, GPIO_Pin, pinModeType::DIGITAL_OUT, isInverted) {
        };

        PinDigitalOut(const char *pinName, GPIO_TypeDef *GPIOx, const uint16_t GPIO_Pin)
            : PinDigital(pinName, GPIOx, GPIO_Pin, pinModeType::DIGITAL_OUT) {
        };

        PinDigitalOut(const char *pinName, GPIO_TypeDef *GPIOx, const uint16_t GPIO_Pin, const bool isInverted)
            : PinDigital(pinName, GPIOx, GPIO_Pin, pinModeType::DIGITAL_OUT, isInverted) {
        };

        /**
         * @brief Executes a loop iteration for the PinDigitalOut class.
         *
         * This method performs the necessary operations for a single iteration of the loop for the PinDigitalOut class.
         * It calls the loop method of the base class PinDigital, and then checks the function type of the pin. Depending on the
         * function type, it performs specific actions.
         */
        void loop() override;

        /**
         * @brief Sets the pin state to ON for the PinDigitalOut class.
         *
         * This method sets the function type of the pin to 'ON' and writes the corresponding pin state to the hardware.
         * It checks if the pin setup has been done, and returns immediately if it hasn't.
         * After setting the pin state, it calls the updatePinState() method.
         */
        virtual void setOn();

        /**
         * @brief Turns off the digital output pin.
         *
         * This method sets the function type of the PinDigitalOut class to OFF and writes the appropriate state to the pin.
         * It first checks if the setup has been done. If not, it returns. Otherwise, it sets the function type to OFF,
         * writes the pin state by calling HAL_GPIO_WritePin, and updates the pin state.
         */
        virtual void setOff();

        /**
         *
         */
        virtual void toggle();

        /**
         * @brief Sets the blink functionality for the PinDigitalOut class.
         *
         * This method sets the blink functionality for the PinDigitalOut class by calling the overloaded setBlink method
         * with an offMs value of 0.
         *
         * @param onMs The duration in milliseconds that the pin should be set to ON during each blink cycle.
         */
        virtual void setBlink(const uint32_t onMs) { setBlink(onMs, 0); };

        /**
         * @brief Sets the blink functionality for the PinDigitalOut class.
         *
         * This method sets the blink functionality for the PinDigitalOut class.
         *
         * @param onMs The duration in milliseconds that the pin should be set to ON during each blink cycle.
         * @param offMs The duration in milliseconds that the pin should be set to OFF during each blink cycle. If not provided, it defaults to the value of onMs.
         *
         * @note This method checks if the pin setup has been done. If not, it returns immediately without performing any actions.
         */
        virtual void setBlink(uint32_t onMs, uint32_t offMs);

    private:
        using functionType = enum class functionType {
            OFF, ON, BLINK
        };
        functionType fn = functionType::OFF;
        uint32_t _onMs = 0, _offMs = 0;
    };
}

#endif //LIBSMART_STM32GPIO_PINDIGITALOUT_H
