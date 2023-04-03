################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../firmware/at32f421_adc.c \
../firmware/at32f421_cmp.c \
../firmware/at32f421_crc.c \
../firmware/at32f421_crm.c \
../firmware/at32f421_debug.c \
../firmware/at32f421_dma.c \
../firmware/at32f421_ertc.c \
../firmware/at32f421_exint.c \
../firmware/at32f421_flash.c \
../firmware/at32f421_gpio.c \
../firmware/at32f421_i2c.c \
../firmware/at32f421_misc.c \
../firmware/at32f421_pwc.c \
../firmware/at32f421_scfg.c \
../firmware/at32f421_spi.c \
../firmware/at32f421_tmr.c \
../firmware/at32f421_usart.c \
../firmware/at32f421_wdt.c \
../firmware/at32f421_wwdt.c 

OBJS += \
./firmware/at32f421_adc.o \
./firmware/at32f421_cmp.o \
./firmware/at32f421_crc.o \
./firmware/at32f421_crm.o \
./firmware/at32f421_debug.o \
./firmware/at32f421_dma.o \
./firmware/at32f421_ertc.o \
./firmware/at32f421_exint.o \
./firmware/at32f421_flash.o \
./firmware/at32f421_gpio.o \
./firmware/at32f421_i2c.o \
./firmware/at32f421_misc.o \
./firmware/at32f421_pwc.o \
./firmware/at32f421_scfg.o \
./firmware/at32f421_spi.o \
./firmware/at32f421_tmr.o \
./firmware/at32f421_usart.o \
./firmware/at32f421_wdt.o \
./firmware/at32f421_wwdt.o 

C_DEPS += \
./firmware/at32f421_adc.d \
./firmware/at32f421_cmp.d \
./firmware/at32f421_crc.d \
./firmware/at32f421_crm.d \
./firmware/at32f421_debug.d \
./firmware/at32f421_dma.d \
./firmware/at32f421_ertc.d \
./firmware/at32f421_exint.d \
./firmware/at32f421_flash.d \
./firmware/at32f421_gpio.d \
./firmware/at32f421_i2c.d \
./firmware/at32f421_misc.d \
./firmware/at32f421_pwc.d \
./firmware/at32f421_scfg.d \
./firmware/at32f421_spi.d \
./firmware/at32f421_tmr.d \
./firmware/at32f421_usart.d \
./firmware/at32f421_wdt.d \
./firmware/at32f421_wwdt.d 


# Each subdirectory must supply rules for building sources it contributes
firmware/%.o: ../firmware/%.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -ffunction-sections  -g -DAT_START_F421_V1 -DAT32F421C8T7 -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DUSE_STDPERIPH_DRIVER -I"../include" -I"../include/libraries/drivers/inc" -I"../include/libraries/cmsis/cm4/core_support" -I"../include/libraries/cmsis/cm4/device_support" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


