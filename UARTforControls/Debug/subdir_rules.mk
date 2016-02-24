################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
accelerometer.obj: ../accelerometer.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="/C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" -g --gcc --define=ccs="ccs" --define=MOTOR_TEST --define=TARGET_IS_BLIZZARD_RB1 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="accelerometer.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

i2c.obj: ../i2c.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="/C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" -g --gcc --define=ccs="ccs" --define=MOTOR_TEST --define=TARGET_IS_BLIZZARD_RB1 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="i2c.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

leds.obj: ../leds.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="/C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" -g --gcc --define=ccs="ccs" --define=MOTOR_TEST --define=TARGET_IS_BLIZZARD_RB1 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="leds.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="/C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" -g --gcc --define=ccs="ccs" --define=MOTOR_TEST --define=TARGET_IS_BLIZZARD_RB1 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

motors.obj: ../motors.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="/C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" -g --gcc --define=ccs="ccs" --define=MOTOR_TEST --define=TARGET_IS_BLIZZARD_RB1 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="motors.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

servoControl.obj: ../servoControl.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="/C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" -g --gcc --define=ccs="ccs" --define=MOTOR_TEST --define=TARGET_IS_BLIZZARD_RB1 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="servoControl.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

tm4c123gh6pm_startup_ccs.obj: ../tm4c123gh6pm_startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="/C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" -g --gcc --define=ccs="ccs" --define=MOTOR_TEST --define=TARGET_IS_BLIZZARD_RB1 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="tm4c123gh6pm_startup_ccs.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

uart.obj: ../uart.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="/C:/ti/TivaWare_C_Series-2.1.2.111" --include_path="/home/luisgonzalez/Desktop/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" -g --gcc --define=ccs="ccs" --define=MOTOR_TEST --define=TARGET_IS_BLIZZARD_RB1 --define=PART_TM4C123GH6PM --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=full --preproc_with_compile --preproc_dependency="uart.pp" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


