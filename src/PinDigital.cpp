/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "PinDigital.hpp"

using namespace Stm32Gpio;

void PinDigital::loop() {
    Pin::loop();
    updatePinState();
}

void PinDigital::updatePinState() {
    if (isOn() && !lastLoopPinState) {
        // Pin is now on, was off before
        lastChangeToOn = millis();
    } else if (isOff() && lastLoopPinState) {
        // Pin is now off, was on before
        lastChangeToOff = millis();
    }
    lastLoopPinState = isOn();

    changeHandler();
}

void PinDigital::setInverted(bool newValue /* = true */) {
    if (newValue == inverted) return;
    //    auto *pdOut = dynamic_cast<PinDigitalOut *>(this);
    //    if (pdOut != nullptr) {
    //        pdOut->toggle();
    //    }
    lastLoopPinState = !lastLoopPinState;
    lastChangeHandlerPinState = !lastChangeHandlerPinState;
    inverted = newValue;
}

bool PinDigital::isOn() {
#if __EXCEPTIONS
    setupDone ? (void) 0 : throw "call setup() first";
#endif
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == (inverted ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

bool PinDigital::isOff() {
#if __EXCEPTIONS
    setupDone ? (void) 0 : throw "call setup() first";
#endif
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == (inverted ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

uint32_t PinDigital::millisSinceLastOn() {
    return millis() - lastChangeToOn;
}

uint32_t PinDigital::millisSinceLastOff() {
    return millis() - lastChangeToOff;
}

uint32_t PinDigital::millisSinceLastChange() {
    return isOn() ? millisSinceLastOn() : millisSinceLastOff();
}

bool PinDigital::hasChanged() {
    return (Pin::hasChanged() || (lastChangeHandlerPinState != isOn()));
}

void PinDigital::resetChange() {
    Pin::resetChange();
    lastChangeHandlerPinState = isOn();
}


