/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef LIBSMART_STM32GPIO_PINDIGITAL_H
#define LIBSMART_STM32GPIO_PINDIGITAL_H

#include <Pin.hpp>

namespace Stm32Gpio {
    class PinDigital : public Pin {
    protected:
        PinDigital(GPIO_TypeDef *GPIOx, const uint16_t gpioPin, const pinModeType pinMode)
            : Pin(GPIOx, gpioPin, pinMode) {
        }

        PinDigital(GPIO_TypeDef *GPIOx, const uint16_t gpioPin, const pinModeType pinMode, const bool isInverted)
            : Pin(GPIOx, gpioPin, pinMode),
              inverted(isInverted) {
        }

        PinDigital(const char *pinName, GPIO_TypeDef *GPIOx, const uint16_t gpioPin, const pinModeType pinMode)
            : Pin(pinName, GPIOx, gpioPin, pinMode) {
        }

        PinDigital(const char *pinName, GPIO_TypeDef *GPIOx, const uint16_t gpioPin, const pinModeType pinMode,
                   const bool isInverted)
            : Pin(pinName, GPIOx, gpioPin, pinMode),
              inverted(isInverted) {
        }

    public:
        /**
         * @brief Perform the looping actions for the PinDigital class.
         *
         * This method is responsible for performing the looping actions for the PinDigital class.
         * It first calls the parent class's loop() method, followed by updating the pin state.
         * This method is expected to be called in the main loop of the program.
         */
        void loop() override;

        /**
         * @brief Set the inverted state for the PinDigital class.
         *
         * This method sets the inverted state for the PinDigital class. When inverted is true, the pin state will be inverted.
         *
         * @param newValue The new inverted state of the pin. The default value is true.
         */
        void setInverted(bool newValue = true);

        /**
         * @brief Check if the pin is currently on.
         *
         * This method checks the state of the pin and returns true if the pin is currently on,
         * and false otherwise. It uses HAL_GPIO_ReadPin to read the pin state and takes into
         * account the inverted flag to determine the on/off state. It throws an exception if setup()
         * has not been called before calling this method.
         *
         * @return True if the pin is currently on, false otherwise.
         */
        virtual bool isOn();

        /**
         * @brief Check if the digital pin is in the off state.
         *
         * This method checks the current state of the digital pin and returns true if it is in the off state,
         * and false otherwise. The pin state is read using HAL_GPIO_ReadPin() function from the stm32f1xx_hal_gpio.h header file.
         *
         * @return True if the pin is off, false otherwise.
         */
        virtual bool isOff();

        /**
         * @brief Get the number of milliseconds since the last time the PinDigital was turned on.
         *
         * This method calculates the number of milliseconds elapsed since the last time the PinDigital object's state
         * was changed to "on". It uses the millis() function from the Helper.hpp header file to get the current time.
         *
         * Note that this method assumes the lastChangeToOn variable is a valid timestamp indicating the last time the
         * PinDigital object's state was changed to "on". If lastChangeToOn is not a valid timestamp, the result of this
         * method may not be accurate.
         *
         * @return The number of milliseconds since the last time the PinDigital was turned on.
         */
        virtual uint32_t millisSinceLastOn();

        /**
         * @brief Returns the number of milliseconds since the last time the pin was turned off.
         *
         * This method calculates the time elapsed in milliseconds since the last time the pin was turned off.
         * It uses the millis() function from the Helper.hpp file to get the current time in milliseconds.
         * The result is obtained by subtracting the value of lastChangeToOff variable from the current time.
         *
         * @return The number of milliseconds since the last time the pin was turned off.
         */
        virtual uint32_t millisSinceLastOff();

        /**
         * @brief Returns the number of milliseconds since the most recent state change of the digital pin.
         *
         * This method is responsible for calculating and returning the number of milliseconds since the most recent state change
         * of the digital pin. If the pin is currently in the on state, it calls the millisSinceLastOn() method and returns its result.
         * Otherwise, it calls the millisSinceLastOff() method and returns its result.
         *
         * @return The number of milliseconds since the most recent state change of the digital pin.
         */
        virtual uint32_t millisSinceLastChange();

    protected:
        /**
         * @brief Update the state of the pin.
         *
         * This method is responsible for updating the state of the pin. It checks if the pin is on or off and stores the last change
         * to on and off timestamps. It also calls the `changeHandler()` method.
         *
         * @note This method is called internally by the `loop()` method and should not be called directly.
         */
        virtual void updatePinState();

        /**
         * @brief Determines whether the pin state has changed or not.
         *
         * This method returns a boolean value indicating whether the pin state has changed or not.
         * It first calls the hasChanged() method of the parent class, Pin, and checks if the pin state
         * has changed. Then, it compares the current pin state with the previous pin state stored in
         * the lastChangeHandlerPinState variable. If there is any change, it returns true; otherwise,
         * it returns false.
         *
         * @return Boolean value indicating whether the pin state has changed or not.
         */
        bool hasChanged() override;

        /**
         * @brief Resets the change handler's last pin state.
         *
         * This method is responsible for resetting the change handler's last pin state in the PinDigital class.
         * It calls the resetChange() method of the parent class Pin, and then updates the lastChangeHandlerPinState
         * attribute with the current state of the pin (on/off).
         *
         * This method should be called whenever you want to reset the change handler's last pin state.
         */
        void resetChange() override;

        /**
         * @brief Indicates whether the pin is inverted.
         *
         * The `inverted` variable is a boolean that indicates whether the pin is inverted. When `inverted` is set to true,
         * the pin output will be inverted. When `inverted` is set to false, the pin output will not be inverted.
         *
         * @see PinDigital.hpp for the declaration of `inverted` variable.
         */
        bool inverted = false;

    private:
        /**
         * @brief Stores the previous pin state during the last loop iteration.
         *
         * This boolean variable is used to store the previous pin state during the last loop iteration.
         * Its initial value is set to false.
         * It is used to keep track of the previous state for comparison with the current pin state in the current loop iteration.
         * This variable is typically used in applications where it is necessary to detect changes in the pin state from one loop iteration to another.
         */
        bool lastLoopPinState = false;

        /**
         * @brief Represents the last known state of a pin used by the change handler.
         *
         * This variable is a boolean that holds the last known state of a pin used by the change handler.
         * It is initialized with the value 'false' by default and can be modified by the change handler as needed.
         *
         * @note The change handler is responsible for updating this variable based on the pin's state changes.
         */
        bool lastChangeHandlerPinState = false;

        /**
         * @brief The timestamp of the last change to the "on" state.
         *
         * This variable of type uint32_t stores the timestamp of the last change made to the "on" state.
         * It is used to track the time when the pin state transitioned from "off" to "on".
         */
        uint32_t lastChangeToOn = 0;

        /**
         * @brief The last time the pin changed state to off.
         *
         * This variable of type uint32_t stores the timestamp of the last time the pin changed
         * state to off. It is used to keep track of the time when the pin was last turned off.
         * This variable can be useful for various time-related calculations.
         */
        uint32_t lastChangeToOff = 0;
    };
}

#endif //LIBSMART_STM32GPIO_PINDIGITAL_H
