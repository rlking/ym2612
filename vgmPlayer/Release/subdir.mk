################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vgmPlayer.c 

C_DEPS += \
.\vgmPlayer.c.d 

LINK_OBJ += \
./vgmPlayer.c.o 


# Each subdirectory must supply rules for building sources it contributes
vgmPlayer.c.o: ../vgmPlayer.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"D:\sloeber\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.3-arduino2/bin/avr-gcc" -c -g -Os -Wall -Wextra -std=gnu11 -ffunction-sections -fdata-sections -flto -fno-fat-lto-objects -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10609 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR   -I"D:\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\cores\arduino" -I"D:\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.17\variants\eightanaloginputs" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


