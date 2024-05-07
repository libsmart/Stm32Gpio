/*
 * SPDX-FileCopyrightText: 2024 Roland Rusch, easy-smart solution GmbH <roland.rusch@easy-smart.ch>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSMART_STM32GPIO_PINANALOGIN_H
#define LIBSMART_STM32GPIO_PINANALOGIN_H

#include "Pin.hpp"
#include "adc.h"

namespace Stm32Gpio {
    class PinAnalogIn : public Pin {
    public:
        /*
        PinAnalogIn(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) : Pin(GPIOx, GPIO_Pin, ANALOG_IN) {
        };

        PinAnalogIn(const char *name, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) : Pin(name, GPIOx,
            GPIO_Pin,
            ANALOG_IN) {
        };
        */

        PinAnalogIn(const char *pinName, ADC_HandleTypeDef *hadc, uint32_t ADC_Channel)
            : Pin(pinName, nullptr, 0, pinModeType::ANALOG_IN), hadc(hadc), ADC_Channel(ADC_Channel) {
        };


        void loop() override;

        uint32_t readValueFromAdc() {
            uint32_t adc_value;
            ADC_ChannelConfTypeDef sConfig;

#ifdef STM32F1
            sConfig.Channel = ADC_Channel; //8
            sConfig.Rank = 1;
            sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
#endif

#ifdef STM32F4
            sConfig.Channel = ADC_Channel; //8
            sConfig.Rank = 1;
            sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
            sConfig.Offset = 0;
#endif

            HAL_ADC_ConfigChannel(hadc, &sConfig);

            // start ADC convertion
            HAL_ADC_Start(hadc);
            // ADC poll for conversion
            HAL_ADC_PollForConversion(hadc, 100);
            // get the ADC conversion value
            adc_value = HAL_ADC_GetValue(hadc);
            // end ADC convertion
            HAL_ADC_Stop(hadc);

            currentAdcValueReady = true;

            return adc_value;
        }

        uint32_t readValue() {
            return currentAdcValueReady ? currentAdcValue : (currentAdcValue = readValueFromAdc());
        }

        static int8_t calculateValue(uint32_t val) {
            /*
             * 0    |    0V |  0mA | -27%
             * 819  | 0.66V |  4mA |   0%
             * 4096 |  3.3V | 20mA | 110%
             *
             * R = U/I = 3.3V/20mA = 165Ω
             * U(0) = R*I(0) = 165Ω*4mA = 0.66V
             * v(0) = 20mA*4096/I = 20mA*4096/4mA = 819.2
             * p = 110*(v-819)/(4096-819) =
             */
            return (int8_t) ((110L * ((int64_t) val - 819L)) / (4096L - 819L));
        }

        int8_t readCalculatedValue() {
            return calculateValue(readValue());
        }

    protected:
        bool hasChanged() override {
            return (Pin::hasChanged() || (calculateValue(lastChangeHandlerAdcValue) != readCalculatedValue()));
        }

        void resetChange() override {
            Pin::resetChange();
            lastChangeHandlerAdcValue = readValue();
        }

    private:
        uint32_t currentAdcValue = 0;
        bool currentAdcValueReady = false;
        //    uint32_t lastLoopAdcValue = 0;
        uint32_t lastChangeHandlerAdcValue = 0;

        ADC_HandleTypeDef *hadc;
        uint32_t ADC_Channel;
    };
}

#endif //LIBSMART_STM32GPIO_PINANALOGIN_H
