################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device_driver/ASCLIN3_USB_UART.c \
../Device_driver/Driver_Adc.c \
../Device_driver/Driver_Asc.c \
../Device_driver/Driver_Gtm.c \
../Device_driver/Driver_Port.c \
../Device_driver/Driver_Stm.c \
../Device_driver/Driver_Watchdog.c \
../Device_driver/Driver_encoder.c 

COMPILED_SRCS += \
./Device_driver/ASCLIN3_USB_UART.src \
./Device_driver/Driver_Adc.src \
./Device_driver/Driver_Asc.src \
./Device_driver/Driver_Gtm.src \
./Device_driver/Driver_Port.src \
./Device_driver/Driver_Stm.src \
./Device_driver/Driver_Watchdog.src \
./Device_driver/Driver_encoder.src 

C_DEPS += \
./Device_driver/ASCLIN3_USB_UART.d \
./Device_driver/Driver_Adc.d \
./Device_driver/Driver_Asc.d \
./Device_driver/Driver_Gtm.d \
./Device_driver/Driver_Port.d \
./Device_driver/Driver_Stm.d \
./Device_driver/Driver_Watchdog.d \
./Device_driver/Driver_encoder.d 

OBJS += \
./Device_driver/ASCLIN3_USB_UART.o \
./Device_driver/Driver_Adc.o \
./Device_driver/Driver_Asc.o \
./Device_driver/Driver_Gtm.o \
./Device_driver/Driver_Port.o \
./Device_driver/Driver_Stm.o \
./Device_driver/Driver_Watchdog.o \
./Device_driver/Driver_encoder.o 


# Each subdirectory must supply rules for building sources it contributes
Device_driver/%.src: ../Device_driver/%.c Device_driver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/user/AURIX-v1.9.20-workspace/Base_Project/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/Device_driver\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

Device_driver/ASCLIN3_USB_UART.o: ./Device_driver/ASCLIN3_USB_UART.src Device_driver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Device_driver/Driver_Adc.o: ./Device_driver/Driver_Adc.src Device_driver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Device_driver/Driver_Asc.o: ./Device_driver/Driver_Asc.src Device_driver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Device_driver/Driver_Gtm.o: ./Device_driver/Driver_Gtm.src Device_driver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Device_driver/Driver_Port.o: ./Device_driver/Driver_Port.src Device_driver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Device_driver/Driver_Stm.o: ./Device_driver/Driver_Stm.src Device_driver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Device_driver/Driver_Watchdog.o: ./Device_driver/Driver_Watchdog.src Device_driver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Device_driver/Driver_encoder.o: ./Device_driver/Driver_encoder.src Device_driver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-Device_driver

clean-Device_driver:
	-$(RM) ./Device_driver/ASCLIN3_USB_UART.d ./Device_driver/ASCLIN3_USB_UART.o ./Device_driver/ASCLIN3_USB_UART.src ./Device_driver/Driver_Adc.d ./Device_driver/Driver_Adc.o ./Device_driver/Driver_Adc.src ./Device_driver/Driver_Asc.d ./Device_driver/Driver_Asc.o ./Device_driver/Driver_Asc.src ./Device_driver/Driver_Gtm.d ./Device_driver/Driver_Gtm.o ./Device_driver/Driver_Gtm.src ./Device_driver/Driver_Port.d ./Device_driver/Driver_Port.o ./Device_driver/Driver_Port.src ./Device_driver/Driver_Stm.d ./Device_driver/Driver_Stm.o ./Device_driver/Driver_Stm.src ./Device_driver/Driver_Watchdog.d ./Device_driver/Driver_Watchdog.o ./Device_driver/Driver_Watchdog.src ./Device_driver/Driver_encoder.d ./Device_driver/Driver_encoder.o ./Device_driver/Driver_encoder.src

.PHONY: clean-Device_driver

