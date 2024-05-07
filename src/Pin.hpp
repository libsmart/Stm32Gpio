/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32GPIO_PIN_H
#define LIBSMART_STM32GPIO_PIN_H

#include "libsmart_config.hpp"
#include "main.h"
#include <cstdint>
#include "PinInterface.hpp"
#include "Helper.hpp"

#ifdef LIBSMART_ENABLE_STD_FUNCTION
#include <functional>
#endif


namespace Stm32Gpio {
    class Pin : public PinInterface {
    public:
        Pin() = delete;

        void setup() override;

        void loop() override;;

    protected:
        Pin(GPIO_TypeDef *GPIOx, const uint16_t GPIO_Pin, const pinModeType pinMode)
            : PinInterface(pinMode),
              GPIOx(GPIOx),
              GPIO_Pin(GPIO_Pin) {
        }

        Pin(const char *pinName, GPIO_TypeDef *GPIOx, const uint16_t GPIO_Pin, const pinModeType pinMode)
            : PinInterface(pinName, pinMode),
              GPIOx(GPIOx),
              GPIO_Pin(GPIO_Pin) {
        }

    public:
        using onChangeCallback = void (*)(PinInterface *pin);
        using loopCallback = void (*)(PinInterface *pin);
        virtual void setOnChangeCallback(const onChangeCallback cb) { cb_onChange = cb; }
        virtual void setLoopCallback(const loopCallback cb) { cb_loop = cb; }

    private:
        onChangeCallback cb_onChange = {};
        loopCallback cb_loop = {};


#ifdef LIBSMART_ENABLE_STD_FUNCTION

    public:
        using onChangeFunction = std::function<void()>;
        using loopFunction = std::function<void()>;
        virtual void setOnChangeCallback(const onChangeFunction &cb) { fn_onChange = cb; }
        virtual void setLoopCallback(const loopFunction &fn) { fn_loop = fn; }

    private:
        onChangeFunction fn_onChange = {};
        loopFunction fn_loop = {};

#endif

    public:
        /**
         * @brief Get the number of milliseconds since the last onChange callback was called.
         *
         * This method calculates the number of milliseconds since the last onChange callback
         * was called by subtracting the value of lastOnChangeCallbackMs from the current
         * value returned by the millis() function.
         *
         * @return The number of milliseconds since the last onChange callback was called.
         */
        virtual uint32_t millisSinceLastOnChangeCallback();

        /**
         * @brief Set the forceOnChangeCallback to 0 milliseconds.
         *
         * This method sets the value of forceOnChangeCallback to true, and sets the value of deferForcedOnChangeCallbackMs to 0.
         * This means that the onChange callback will be forcefully triggered on the next loop iteration, even if the specified
         * defer time has not passed.
         */
        virtual void setForceOnChangeCallback();

        /**
         * @brief Set the defer time for the onChange callback.
         *
         * This method sets the value of deferOnChangeCallbackMs to the specified number of milliseconds.
         * This means that the onChange callback will be deferred for the specified defer time,
         * as long as it is called within the defer time from the last onChange callback.
         *
         * @param deferMs The defer time for the onChange callback in milliseconds.
         */
        virtual void setDeferOnChangeCallback(const uint32_t deferMs);;

        /**
         * @brief Set the forceOnChangeCallback to the specified defer time in milliseconds.
         *
         * This method sets the value of forceOnChangeCallback to true and sets the value of deferForcedOnChangeCallbackMs
         * to the specified defer time. This means that the onChange callback will be forcefully triggered on the next
         * loop iteration, even if the specified defer time has not passed. But only if the deferForced.. time has elapsed.
         *
         * @param deferMs The defer time for the onChange callback in milliseconds.
         */
        virtual void setForceOnChangeCallback(const uint32_t deferMs) {
            forceOnChangeCallback = true;
            deferForcedOnChangeCallbackMs = deferMs;
        };

        /**
         * @brief Check if the pin state has changed and if the forced onChange callback should be triggered.
         *
         * This method checks if the forceOnChangeCallback flag is true and if the number of milliseconds
         * since the last onChange callback was called is greater than or equal to the deferForcedOnChangeCallbackMs value.
         *
         * @return true if the pin state has changed and the forced onChange callback should be triggered,
         *         false otherwise.
         * @note This method has to be overloaded by the sub classes to make any sense. Else it only works with
         * forced changes.
         */
    protected:
        virtual bool hasChanged() {
            return forceOnChangeCallback && ((millisSinceLastOnChangeCallback()) >= deferForcedOnChangeCallbackMs);
        };

        /**
         * @brief Reset the change tracking variables.
         *
         * This method sets the value of forceOnChangeCallback to false, deferForcedOnChangeCallbackMs to 0,
         * and deferOnChangeCallbackMs to 0. These variables are used to track changes and defer the onChange callback.
         */
        virtual void resetChange() {
            forceOnChangeCallback = false;
            deferForcedOnChangeCallbackMs = 0;
            deferOnChangeCallbackMs = 0;
        };

        /**
         * @brief Handle changes in the Pin's state and trigger the onChange callback if necessary.
         *
         * This method is called whenever there is a change in the Pin's state. It checks if the specified defer time
         * has passed since the last onChange callback and if the Pin's state has actually changed. If both conditions
         * are met, the method resets the change tracking variables and triggers the onChange callback.
         */
        virtual void changeHandler() {
            if ((millisSinceLastOnChangeCallback() >= deferOnChangeCallbackMs) && hasChanged()) {
                resetChange();
                cb_onChange != nullptr ? cb_onChange(this) : (void) nullptr;
#ifdef LIBSMART_ENABLE_STD_FUNCTION
                fn_onChange != nullptr ? fn_onChange() : (void) nullptr;
#endif
                if (deferOnChangeCallbackMs == 0) lastOnChangeCallbackMs = millis();
            }
        };

        /**
         * @brief Pointer to the GPIO port register.
         *
         * This variable is a pointer to the GPIO port register. It is used to access and configure
         * the GPIO pins of a specific port. GPIO_TypeDef is a typedef defined in the STM32 library,
         * representing the GPIO port register.
         */
        GPIO_TypeDef *GPIOx;

        /**
         * @brief GPIO pin number.
         *
         * This variable represents the number of a GPIO pin. It is of type uint16_t, which allows it to store values
         * ranging from 0 to 65535.
         *
         * The value of this variable can be used to identify a specific GPIO pin in order to perform operations such
         * as reading or setting its state.
         */
        uint16_t GPIO_Pin;

    private:
        /**
         * @brief The timestamp in milliseconds of the last onChange callback.
         *
         * This variable holds the value of the timestamp in milliseconds when the last onChange
         * callback was called. It is initially set to 0.
         */
        uint32_t lastOnChangeCallbackMs = 0;

        /**
         * @brief The number of milliseconds to defer the onChange callback.
         *
         * This variable holds the number of milliseconds to defer the onChange callback.
         * The value represents the time interval between when a change is detected and when
         * the onChange callback is called.
         */
        uint32_t deferOnChangeCallbackMs = 0;

        /**
         * @brief A boolean flag indicating whether to force the onChange callback.
         *
         * This flag is used to determine whether the onChange callback should be forced,
         * regardless of any conditions or constraints. If the flag is set to true, the
         * onChange callback will be called regardless of any other factors. If the flag
         * is set to false, the onChange callback will only be called under normal conditions.
         *
         * @note This flag should be used with caution, as forcing the onChange callback may
         *       have unintended side effects or disrupt the normal behavior of the program.
         *
         * @note The initial value of this flag is true.
         */
        bool forceOnChangeCallback = true;

        /**
         * @brief The number of milliseconds to defer a forced onChange callback.
         *
         * This variable stores the number of milliseconds to defer a forced onChange callback.
         * It is initialized to 0.
         */
        uint32_t deferForcedOnChangeCallbackMs = 0;
    };
}

#endif //LIBSMART_STM32GPIO_PIN_H
