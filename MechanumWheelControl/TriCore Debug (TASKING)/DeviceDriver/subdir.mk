################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DeviceDriver/Driver_Buzzer.c \
../DeviceDriver/Driver_Joystick.c \
../DeviceDriver/Driver_Potentiometer.c \
../DeviceDriver/Driver_ToF.c \
../DeviceDriver/Driver_USB.c \
../DeviceDriver/Driver_WheelFL.c \
../DeviceDriver/Driver_WheelFR.c \
../DeviceDriver/Driver_WheelRL.c \
../DeviceDriver/Driver_WheelRR.c 

COMPILED_SRCS += \
./DeviceDriver/Driver_Buzzer.src \
./DeviceDriver/Driver_Joystick.src \
./DeviceDriver/Driver_Potentiometer.src \
./DeviceDriver/Driver_ToF.src \
./DeviceDriver/Driver_USB.src \
./DeviceDriver/Driver_WheelFL.src \
./DeviceDriver/Driver_WheelFR.src \
./DeviceDriver/Driver_WheelRL.src \
./DeviceDriver/Driver_WheelRR.src 

C_DEPS += \
./DeviceDriver/Driver_Buzzer.d \
./DeviceDriver/Driver_Joystick.d \
./DeviceDriver/Driver_Potentiometer.d \
./DeviceDriver/Driver_ToF.d \
./DeviceDriver/Driver_USB.d \
./DeviceDriver/Driver_WheelFL.d \
./DeviceDriver/Driver_WheelFR.d \
./DeviceDriver/Driver_WheelRL.d \
./DeviceDriver/Driver_WheelRR.d 

OBJS += \
./DeviceDriver/Driver_Buzzer.o \
./DeviceDriver/Driver_Joystick.o \
./DeviceDriver/Driver_Potentiometer.o \
./DeviceDriver/Driver_ToF.o \
./DeviceDriver/Driver_USB.o \
./DeviceDriver/Driver_WheelFL.o \
./DeviceDriver/Driver_WheelFR.o \
./DeviceDriver/Driver_WheelRL.o \
./DeviceDriver/Driver_WheelRR.o 


# Each subdirectory must supply rules for building sources it contributes
DeviceDriver/%.src: ../DeviceDriver/%.c DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/user/Documents/GitHub/MechanumWheelControl/MechanumWheelControl/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<" && \
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

DeviceDriver/Driver_Potentiometer.o: ./DeviceDriver/Driver_Potentiometer.src DeviceDriver/subdir.mk
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

DeviceDriver/Driver_USB.o: ./DeviceDriver/Driver_USB.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DeviceDriver/Driver_WheelFL.o: ./DeviceDriver/Driver_WheelFL.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DeviceDriver/Driver_WheelFR.o: ./DeviceDriver/Driver_WheelFR.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DeviceDriver/Driver_WheelRL.o: ./DeviceDriver/Driver_WheelRL.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DeviceDriver/Driver_WheelRR.o: ./DeviceDriver/Driver_WheelRR.src DeviceDriver/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-DeviceDriver

clean-DeviceDriver:
	-$(RM) ./DeviceDriver/Driver_Buzzer.d ./DeviceDriver/Driver_Buzzer.o ./DeviceDriver/Driver_Buzzer.src ./DeviceDriver/Driver_Joystick.d ./DeviceDriver/Driver_Joystick.o ./DeviceDriver/Driver_Joystick.src ./DeviceDriver/Driver_Potentiometer.d ./DeviceDriver/Driver_Potentiometer.o ./DeviceDriver/Driver_Potentiometer.src ./DeviceDriver/Driver_ToF.d ./DeviceDriver/Driver_ToF.o ./DeviceDriver/Driver_ToF.src ./DeviceDriver/Driver_USB.d ./DeviceDriver/Driver_USB.o ./DeviceDriver/Driver_USB.src ./DeviceDriver/Driver_WheelFL.d ./DeviceDriver/Driver_WheelFL.o ./DeviceDriver/Driver_WheelFL.src ./DeviceDriver/Driver_WheelFR.d ./DeviceDriver/Driver_WheelFR.o ./DeviceDriver/Driver_WheelFR.src ./DeviceDriver/Driver_WheelRL.d ./DeviceDriver/Driver_WheelRL.o ./DeviceDriver/Driver_WheelRL.src ./DeviceDriver/Driver_WheelRR.d ./DeviceDriver/Driver_WheelRR.o ./DeviceDriver/Driver_WheelRR.src

.PHONY: clean-DeviceDriver

