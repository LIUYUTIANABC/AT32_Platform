################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/int_irq_handler/int_irq_handler.c 

OBJS += \
./user/int_irq_handler/int_irq_handler.o 

C_DEPS += \
./user/int_irq_handler/int_irq_handler.d 


# Each subdirectory must supply rules for building sources it contributes
user/int_irq_handler/%.o: ../user/int_irq_handler/%.c user/int_irq_handler/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -ffunction-sections  -g -DAT_START_F421_V1 -DAT32F421C8T7 -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DUSE_STDPERIPH_DRIVER -I"../include" -I"../include/libraries/drivers/inc" -I"../include/libraries/cmsis/cm4/core_support" -I"../include/libraries/cmsis/cm4/device_support" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


