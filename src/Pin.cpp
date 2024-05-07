/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Pin.hpp"

using namespace Stm32Gpio;

void Pin::setup() {
    PinInterface::setup();
}

void Pin::loop() {
    cb_loop != nullptr ? cb_loop(this) : (void) nullptr;
#ifdef LIBSMART_ENABLE_STD_FUNCTION
    fn_loop != nullptr ? fn_loop() : (void) nullptr;
#endif
}

uint32_t Pin::millisSinceLastOnChangeCallback() {
    return millis() - lastOnChangeCallbackMs;
}

void Pin::setForceOnChangeCallback() {
    setForceOnChangeCallback(0);
}

void Pin::setDeferOnChangeCallback(const uint32_t deferMs) {
    deferOnChangeCallbackMs = deferMs;
}
