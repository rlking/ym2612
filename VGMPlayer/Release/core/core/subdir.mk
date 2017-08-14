################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\HardwareSerial.cpp \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\IPAddress.cpp \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\Print.cpp \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\Stream.cpp \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\Tone.cpp \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\WMath.cpp \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\WString.cpp \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\main.cpp \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\new.cpp \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\usb_api.cpp \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\yield.cpp 

CXX_SRCS += \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\main.cxx 

C_SRCS += \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\WInterrupts.c \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\keylayouts.c \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\malloc.c \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\pins_teensy.c \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\usb.c \
C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\wiring.c 

C_DEPS += \
.\core\WInterrupts.c.d \
.\core\keylayouts.c.d \
.\core\malloc.c.d \
.\core\pins_teensy.c.d \
.\core\usb.c.d \
.\core\wiring.c.d 

AR_OBJ += \
.\core\HardwareSerial.cpp.o \
.\core\IPAddress.cpp.o \
.\core\Print.cpp.o \
.\core\Stream.cpp.o \
.\core\Tone.cpp.o \
.\core\WInterrupts.c.o \
.\core\WMath.cpp.o \
.\core\WString.cpp.o \
.\core\keylayouts.c.o \
.\core\main.cpp.o \
.\core\main.cxx.o \
.\core\malloc.c.o \
.\core\new.cpp.o \
.\core\pins_teensy.c.o \
.\core\usb.c.o \
.\core\usb_api.cpp.o \
.\core\wiring.c.o \
.\core\yield.cpp.o 

CXX_DEPS += \
.\core\main.cxx.d 

CPP_DEPS += \
.\core\HardwareSerial.cpp.d \
.\core\IPAddress.cpp.d \
.\core\Print.cpp.d \
.\core\Stream.cpp.d \
.\core\Tone.cpp.d \
.\core\WMath.cpp.d \
.\core\WString.cpp.d \
.\core\main.cpp.d \
.\core\new.cpp.d \
.\core\usb_api.cpp.d \
.\core\yield.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
core\HardwareSerial.cpp.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\HardwareSerial.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\IPAddress.cpp.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\IPAddress.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\Print.cpp.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\Print.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\Stream.cpp.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\Stream.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\Tone.cpp.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\Tone.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\WInterrupts.c.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\WInterrupts.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-gcc" -c -Os -g -Wall -ffunction-sections -fdata-sections -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"     -std=gnu11
	@echo 'Finished building: $<'
	@echo ' '

core\WMath.cpp.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\WMath.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\WString.cpp.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\WString.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\keylayouts.c.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\keylayouts.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-gcc" -c -Os -g -Wall -ffunction-sections -fdata-sections -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"     -std=gnu11
	@echo 'Finished building: $<'
	@echo ' '

core\main.cpp.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\main.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\main.cxx.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\main.cxx
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\malloc.c.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\malloc.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-gcc" -c -Os -g -Wall -ffunction-sections -fdata-sections -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"     -std=gnu11
	@echo 'Finished building: $<'
	@echo ' '

core\new.cpp.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\new.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\pins_teensy.c.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\pins_teensy.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-gcc" -c -Os -g -Wall -ffunction-sections -fdata-sections -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"     -std=gnu11
	@echo 'Finished building: $<'
	@echo ' '

core\usb.c.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\usb.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-gcc" -c -Os -g -Wall -ffunction-sections -fdata-sections -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"     -std=gnu11
	@echo 'Finished building: $<'
	@echo ' '

core\usb_api.cpp.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\usb_api.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\wiring.c.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\wiring.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-gcc" -c -Os -g -Wall -ffunction-sections -fdata-sections -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"     -std=gnu11
	@echo 'Finished building: $<'
	@echo ' '

core\yield.cpp.o: C:\Program\ Files\ (x86)\Arduino\hardware\teensy\avr\cores\teensy\yield.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/avr/bin/avr-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -fno-exceptions -felide-constructors -std=c++0x -mmcu=at90usb1286 -DTEENSYDUINO=137 -DARDUINO_ARCH_AVR -DARDUINO=10802 -DF_CPU=16000000L -DUSB_SERIAL -DLAYOUT_GERMAN  -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


