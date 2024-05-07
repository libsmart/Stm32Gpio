/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32GPIO_PINDIGITALIN_H
#define LIBSMART_STM32GPIO_PINDIGITALIN_H

#include <PinDigital.hpp>

namespace Stm32Gpio {
    class PinDigitalIn : public PinDigital {
    public:
        PinDigitalIn(GPIO_TypeDef *GPIOx, const uint16_t GPIO_Pin)
            : PinDigital(GPIOx, GPIO_Pin, pinModeType::DIGITAL_IN) {
        }

        PinDigitalIn(GPIO_TypeDef *GPIOx, const uint16_t GPIO_Pin, const bool isInverted)
            : PinDigital(GPIOx, GPIO_Pin, pinModeType::DIGITAL_IN, isInverted) {
        }

        PinDigitalIn(const char *pinName, GPIO_TypeDef *GPIOx, const uint16_t GPIO_Pin)
            : PinDigital(pinName, GPIOx, GPIO_Pin, pinModeType::DIGITAL_IN) {
        }

        PinDigitalIn(const char *pinName, GPIO_TypeDef *GPIOx, const uint16_t GPIO_Pin, const bool isInverted)
            : PinDigital(pinName, GPIOx, GPIO_Pin, pinModeType::DIGITAL_IN, isInverted) {
        }
    };
}

#endif //LIBSMART_STM32GPIO_PINDIGITALIN_H
