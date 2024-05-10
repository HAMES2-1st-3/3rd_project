################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DeviceDriver/Driver_Buzzer.c \
../DeviceDriver/Driver_Joystick.c \
../DeviceDriver/Driver_LED.c \
../DeviceDriver/Driver_Motor1.c \
../DeviceDriver/Driver_Motor2.c \
../DeviceDriver/Driver_Motor3.c \
../DeviceDriver/Driver_Motor4.c \
../DeviceDriver/Driver_Potentiometer.c \
../DeviceDriver/Driver_Stm.c \
../DeviceDriver/Driver_ToF.c 

COMPILED_SRCS += \
./DeviceDriver/Driver_Buzzer.src \
./DeviceDriver/Driver_Joystick.src \
./DeviceDriver/Driver_LED.src \
./DeviceDriver/Driver_Motor1.src \
./DeviceDriver/Driver_Motor2.src \
./DeviceDriver/Driver_Motor3.src \
./DeviceDriver/Driver_Motor4.src \
./DeviceDriver/Driver_Potentiometer.src \
./DeviceDriver/Driver_Stm.src \
./DeviceDriver/Driver_ToF.src 

C_DEPS += \
./DeviceDriver/Driver_Buzzer.d \
./DeviceDriver/Driver_Joystick.d \
./DeviceDriver/Driver_LED.d \
./DeviceDriver/Driver_Motor1.d \
./DeviceDriver/Driver_Motor2.d \
./DeviceDriver/Driver_Motor3.d \
./DeviceDriver/Driver_Motor4.d \
./DeviceDriver/Driver_Potentiometer.d \
./DeviceDriver/Driver_Stm.d \
./DeviceDriver/Driver_ToF.d 

OBJS += \
./DeviceDriver/Driver_Buzzer.o \
./DeviceDriver/Driver_Joystick.o \
./DeviceDriver/Driver_LED.o \
./DeviceDriver/Driver_Motor1.o \
./DeviceDriver/Driver_Motor2.o \
./DeviceDriver/Driver_Motor3.o \
./DeviceDriver/Driver_Motor4.o \
./DeviceDriver/Driver_Potentiometer.o \
./DeviceDriver/Driver_Stm.o \
./DeviceDriver/Driver_ToF.o 


# Each subdirectory must supply rules for building sources it contributes
DeviceDriver/%.src: ../DeviceDriver/%.c DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/user/AURIX-v1.9.20-workspace/MechanumWheelControl/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/DeviceDriver\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

DeviceDriver/Driver_Buzzer.o: ./DeviceDriver/Driver_Buzzer.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DeviceDriver/Driver_Joystick.o: ./DeviceDriver/Driver_Joystick.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DeviceDriver/Driver_LED.o: ./DeviceDriver/Driver_LED.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DeviceDriver/Driver_Motor1.o: ./DeviceDriver/Driver_Motor1.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DeviceDriver/Driver_Motor2.o: ./DeviceDriver/Driver_Motor2.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DeviceDriver/Driver_Motor3.o: ./DeviceDriver/Driver_Motor3.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DeviceDriver/Driver_Motor4.o: ./DeviceDriver/Driver_Motor4.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DeviceDriver/Driver_Potentiometer.o: ./DeviceDriver/Driver_Potentiometer.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DeviceDriver/Driver_Stm.o: ./DeviceDriver/Driver_Stm.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DeviceDriver/Driver_ToF.o: ./DeviceDriver/Driver_ToF.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-DeviceDriver

clean-DeviceDriver:
	-$(RM) ./DeviceDriver/Driver_Buzzer.d ./DeviceDriver/Driver_Buzzer.o ./DeviceDriver/Driver_Buzzer.src ./DeviceDriver/Driver_Joystick.d ./DeviceDriver/Driver_Joystick.o ./DeviceDriver/Driver_Joystick.src ./DeviceDriver/Driver_LED.d ./DeviceDriver/Driver_LED.o ./DeviceDriver/Driver_LED.src ./DeviceDriver/Driver_Motor1.d ./DeviceDriver/Driver_Motor1.o ./DeviceDriver/Driver_Motor1.src ./DeviceDriver/Driver_Motor2.d ./DeviceDriver/Driver_Motor2.o ./DeviceDriver/Driver_Motor2.src ./DeviceDriver/Driver_Motor3.d ./DeviceDriver/Driver_Motor3.o ./DeviceDriver/Driver_Motor3.src ./DeviceDriver/Driver_Motor4.d ./DeviceDriver/Driver_Motor4.o ./DeviceDriver/Driver_Motor4.src ./DeviceDriver/Driver_Potentiometer.d ./DeviceDriver/Driver_Potentiometer.o ./DeviceDriver/Driver_Potentiometer.src ./DeviceDriver/Driver_Stm.d ./DeviceDriver/Driver_Stm.o ./DeviceDriver/Driver_Stm.src ./DeviceDriver/Driver_ToF.d ./DeviceDriver/Driver_ToF.o ./DeviceDriver/Driver_ToF.src

.PHONY: clean-DeviceDriver

