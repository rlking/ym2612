################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
INO_SRCS += \
..\TeensySerialTest.ino 

CPP_SRCS += \
..\.ino.cpp 

C_SRCS += \
..\RingBuf.c 

C_DEPS += \
.\RingBuf.c.d 

LINK_OBJ += \
.\.ino.cpp.o \
.\RingBuf.c.o 

INO_DEPS += \
.\TeensySerialTest.ino.d 

CPP_DEPS += \
.\.ino.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
.ino.cpp.o: ..\.ino.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

RingBuf.c.o: ..\RingBuf.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-gcc" -c -Os -g -Wall -ffunction-sections -fdata-sections -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"     -std=gnu11
	@echo 'Finished building: $<'
	@echo ' '

TeensySerialTest.o: ..\TeensySerialTest.ino
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


