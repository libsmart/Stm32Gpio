/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32GPIO_PININTERFACE_HPP
#define LIBSMART_STM32GPIO_PININTERFACE_HPP

namespace Stm32Gpio {
    /**
     * @class PinInterface
     * @brief An interface for interaction with pins.
     *
     * This class defines an interface for interacting with pins. It includes methods for pin setup and loop functionality.
     * It also includes enums for defining pin modes and pull types.
     *
     * @details The PinInterface class provides a set of methods and enums for interacting with pins on a microcontroller.
     * Pins can be set up for various modes such as digital output, digital input, PWM output, and analog input.
     * Pins can also have pull types set as no pull, pull-up, or pull-down.
     * The setup() method is called to initialize the pin, and the loop() method is called continuously to handle pin events.
     */
    class PinInterface {
    public:
        using pinModeType = enum class pinModeType {
            DIGITAL_OUT,
            DIGITAL_IN,
            // DIGITAL_IN_PULLDOWN,
            // DIGITAL_IN_PULLUP,
            PWM_OUT,
            ANALOG_IN
        };

        using pinPullType = enum class pinPullType {
            NOPULL,
            PULLUP,
            PULLDOWN
        };

        PinInterface() = delete;

        explicit PinInterface(const pinModeType pinMode)
            : pinMode(pinMode) {
        }

        PinInterface(const char *pinName, const pinModeType pinMode)
            : pinMode(pinMode), pinName(pinName) {
        }


        virtual ~PinInterface() = default;

        virtual void setup() { setupDone = true; };

        virtual void loop() = 0;


        virtual const char *getName() {
            return pinName;
        }

    protected:
        pinModeType pinMode;
        bool setupDone = false;

        // virtual void setName(const char *name) {
            // pinName = name;
        // }

    private:
        const char *pinName = {};
    };
}

#endif //LIBSMART_STM32GPIO_PININTERFACE_HPP
