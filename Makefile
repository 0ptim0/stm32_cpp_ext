#######################################################################
# Makefile for STM32F1 board projects

OUTPATH = build
PROJECT = $(OUTPATH)/test
MCU = stm32f103c8t6
OPENOCD_SCRIPT_DIR ?= /usr/share/openocd/scripts
HEAP_SIZE = 0x400

################
# Sources

SOURCES_S = mcu/$(MCU)/core/startup_stm32f103xb.s

SOURCES_C_CORE = $(wildcard mcu/$(MCU)/core/*.c)
SOURCES_C_RTOS_CORE = $(wildcard freertos/core/src/*.c)
SOURCES_C_RTOS_PORT = $(wildcard freertos/port/$(MCU)/*.c)
SOURCES_C_HEAP = $(wildcard freertos/MemMang/*.c)

SOURCES_CXX_USR = $(wildcard usr/*.cpp)

SOURCES_C = $(SOURCES_C_CORE)
SOURCES_C += $(SOURCES_C_PERIPH)
SOURCES_C += $(SOURCES_C_RTOS_CORE)
SOURCES_C += $(SOURCES_C_RTOS_PORT)
SOURCES_C += $(SOURCES_C_HEAP)

SOURCES_CXX = $(wildcard *.cpp)
SOURCES_CXX += $(SOURCES_CXX_USR)

SOURCES = $(SOURCES_S) $(SOURCES_C) $(SOURCES_CXX)
OBJS = $(SOURCES_S:.s=.o) $(SOURCES_C:.c=.o) $(SOURCES_CXX:.cpp=.o)

# Includes and Defines

INC_CORE = -Imcu/$(MCU)/core
INC_PERIPH = -Imcu/$(MCU)/ll
INC_LIB = -Iusr
INC_RTOS_CORE = -Ifreertos/core/inc 
INC_RTOS_PORT = -Ifreertos/port/$(MCU)
INC_CONF = -Iconf
INCLUDES += $(INC_CORE)
INCLUDES += $(INC_PERIPH)
INCLUDES += $(INC_LIB)
INCLUDES += $(INC_RTOS_CORE)
INCLUDES += $(INC_RTOS_PORT)
INCLUDES += $(INC_CONF)

DEFINES = -DSTM32 -DSTM32F1 -DSTM32F103xB -DHEAP_SIZE=$(HEAP_SIZE)

# Compiler/Assembler/Linker/etc

PREFIX = arm-none-eabi

CC = $(PREFIX)-gcc
CXX = $(PREFIX)-g++
AS = $(PREFIX)-as
AR = $(PREFIX)-ar
LD = $(PREFIX)-gcc
NM = $(PREFIX)-nm
OBJCOPY = $(PREFIX)-objcopy
OBJDUMP = $(PREFIX)-objdump
READELF = $(PREFIX)-readelf
SIZE = $(PREFIX)-size
GDB = $(PREFIX)-gdb
RM = rm -f
OPENOCD=openocd

# Compiler options

MCUFLAGS = -mcpu=cortex-m3 -mlittle-endian -mfloat-abi=soft -mthumb \
           -mno-unaligned-access

DEBUG_OPTIMIZE_FLAGS = -O0 -ggdb -gdwarf-2

CFLAGS = -Wall -Wextra --pedantic
CFLAGS_EXTRA = -nostartfiles -nodefaultlibs -nostdlib \
               -fdata-sections -ffunction-sections

CFLAGS += $(DEFINES) $(MCUFLAGS) $(DEBUG_OPTIMIZE_FLAGS) $(CFLAGS_EXTRA) $(INCLUDES)
CXXFLAGS = $(DEFINES) $(MCUFLAGS) $(DEBUG_OPTIMIZE_FLAGS) $(INCLUDES) -std=c++17

LDFLAGS = -specs=nano.specs -specs=nosys.specs -static $(MCUFLAGS) -Wl,--start-group -lgcc -lc -lg -Wl,--end-group \
          -Wl,--gc-sections -T mcu/$(MCU)/core/STM32F103XB_FLASH.ld -Xlinker -Map=mem.map 

.PHONY: dirs all clean flash erase

all: dirs $(PROJECT).bin $(PROJECT).asm

dirs: ${OUTPATH}

${OUTPATH}:
	mkdir -p ${OUTPATH}

clean:
	$(RM) $(OBJS) $(PROJECT).elf $(PROJECT).bin $(PROJECT).asm
	rm -rf ${OUTPATH}

# Hardware specific

flash: $(PROJECT).bin
	st-flash write $(PROJECT).bin 0x08000000

erase:
	st-flash erase

gdb-server-ocd:
	$(OPENOCD) -f $(OPENOCD_SCRIPT_DIR)/board/bluepill.cfg

gdb-server-st:
	st-util

OPENOCD_P=3333
gdb-openocd: $(PROJECT).elf
	$(GDB) --eval-command="target extended-remote localhost:$(OPENOCD_P)" \
           --eval-command="load" $(PROJECT).elf

GDB_P=4242
gdb-st-util: $(PROJECT).elf
	$(GDB) --eval-command="target extended-remote localhost:$(GDB_P)" \
           --eval-command="load" $(PROJECT).elf

$(PROJECT).elf: $(OBJS)

%.elf:
	$(LD) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) -A $@

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.asm: %.elf
	$(OBJDUMP) -dwh $< > $@
