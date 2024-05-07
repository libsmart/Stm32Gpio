/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "PinDigitalOut.hpp"

using namespace Stm32Gpio;

void PinDigitalOut::setOn() {
    if (!setupDone) return;
    fn = functionType::ON;
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, inverted ? GPIO_PIN_RESET : GPIO_PIN_SET);
    updatePinState();
}

void PinDigitalOut::setOff() {
    if (!setupDone) return;
    fn = functionType::OFF;
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, inverted ? GPIO_PIN_SET : GPIO_PIN_RESET);
    updatePinState();
}

void PinDigitalOut::toggle() {
    if (!setupDone) return;
    fn == functionType::ON ? setOff() : setOn();
}

void PinDigitalOut::setBlink(uint32_t onMs, uint32_t offMs) {
    if (!setupDone) return;
    if (onMs == 0) setOff();
    _onMs = onMs;
    _offMs = offMs == 0 ? onMs : offMs;
    if (fn != functionType::BLINK) setOn();
    fn = functionType::BLINK;
}

void PinDigitalOut::loop() {
    PinDigital::loop();
    switch (fn) {
        case functionType::ON:
            setOn();
            break;

        case functionType::OFF:
            setOff();
            break;

        case functionType::BLINK:
            if (isOn() && (millisSinceLastOn() >= _onMs)) {
                setOff();
            } else if (isOff() && (millisSinceLastOff() >= _offMs)) {
                setOn();
            }
            fn = functionType::BLINK;
            break;
    }
}


