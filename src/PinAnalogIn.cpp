/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "PinAnalogIn.hpp"

using namespace Stm32Gpio;

void PinAnalogIn::loop() {
    currentAdcValueReady = false;
    Pin::loop();
    changeHandler();
//    lastLoopAdcValue = readValue();
}

