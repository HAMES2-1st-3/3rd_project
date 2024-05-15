################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/AppMode.c \
../App/AppMode_Error.c \
../App/AppMode_Idle.c \
../App/AppMode_Init.c \
../App/AppMode_Operation.c \
../App/AppScheduling.c 

COMPILED_SRCS += \
./App/AppMode.src \
./App/AppMode_Error.src \
./App/AppMode_Idle.src \
./App/AppMode_Init.src \
./App/AppMode_Operation.src \
./App/AppScheduling.src 

C_DEPS += \
./App/AppMode.d \
./App/AppMode_Error.d \
./App/AppMode_Idle.d \
./App/AppMode_Init.d \
./App/AppMode_Operation.d \
./App/AppScheduling.d 

OBJS += \
./App/AppMode.o \
./App/AppMode_Error.o \
./App/AppMode_Idle.o \
./App/AppMode_Init.o \
./App/AppMode_Operation.o \
./App/AppScheduling.o 


# Each subdirectory must supply rules for building sources it contributes
App/%.src: ../App/%.c App/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc27xd "-fC:/Users/user/Documents/GitHub/MechanumWheelControl/MechanumWheelControl/TriCore Debug (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/App\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

App/AppMode.o: ./App/AppMode.src App/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

App/AppMode_Error.o: ./App/AppMode_Error.src App/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

App/AppMode_Idle.o: ./App/AppMode_Idle.src App/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

App/AppMode_Init.o: ./App/AppMode_Init.src App/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

App/AppMode_Operation.o: ./App/AppMode_Operation.src App/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

App/AppScheduling.o: ./App/AppScheduling.src App/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-App

clean-App:
	-$(RM) ./App/AppMode.d ./App/AppMode.o ./App/AppMode.src ./App/AppMode_Error.d ./App/AppMode_Error.o ./App/AppMode_Error.src ./App/AppMode_Idle.d ./App/AppMode_Idle.o ./App/AppMode_Idle.src ./App/AppMode_Init.d ./App/AppMode_Init.o ./App/AppMode_Init.src ./App/AppMode_Operation.d ./App/AppMode_Operation.o ./App/AppMode_Operation.src ./App/AppScheduling.d ./App/AppScheduling.o ./App/AppScheduling.src

.PHONY: clean-App

