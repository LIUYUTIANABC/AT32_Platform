################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include user/usart2_handler/subdir.mk
-include user/m24c16_handler/subdir.mk
-include user/ir_handler/subdir.mk
-include user/int_irq_handler/subdir.mk
-include user/adc_handler/subdir.mk
-include user/subdir.mk
-include firmware/subdir.mk
-include cmsis/subdir.mk
-include bsp/usart/subdir.mk
-include bsp/timestamp/subdir.mk
-include bsp/systick/subdir.mk
-include bsp/led_base/subdir.mk
-include bsp/key_board_base/subdir.mk
-include bsp/ir/subdir.mk
-include bsp/eeprom/subdir.mk
-include bsp/adc/subdir.mk
-include bsp/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Test_ADC
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Test_ADC.hex \

SECONDARY_SIZE += \
Test_ADC.siz \


# All Target
all: main-build

# Main-build Target
main-build: Test_ADC.elf secondary-outputs

# Tool invocations
Test_ADC.elf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -ffunction-sections  -g -T "C:\U_files\Git_Repo\AT32_Platform\Test_ADC/ldscripts/AT32F421x8_FLASH.ld" -Xlinker --gc-sections -Wl,-Map,"Test_ADC.map" --specs=nano.specs --specs=nosys.specs -o "Test_ADC.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Test_ADC.hex: Test_ADC.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU Arm Cross Create Flash Image'
	arm-none-eabi-objcopy -O ihex "Test_ADC.elf"  "Test_ADC.hex"
	@echo 'Finished building: $@'
	@echo ' '

Test_ADC.siz: Test_ADC.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU Arm Cross Print Size'
	arm-none-eabi-size --format=berkeley "Test_ADC.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) Test_ADC.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
