################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
INO_SRCS += \
..\VGMPlayer.ino 

CPP_SRCS += \
..\.ino.cpp 

LINK_OBJ += \
.\.ino.cpp.o 

INO_DEPS += \
.\VGMPlayer.ino.d 

CPP_DEPS += \
.\.ino.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
.ino.cpp.o: ..\.ino.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

VGMPlayer.o: ..\VGMPlayer.ino
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


