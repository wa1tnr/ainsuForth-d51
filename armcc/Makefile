
################################################################################
# Automatically-generated file. Do not edit!
################################################################################

ifdef SystemRoot
	SHELL = cmd.exe
	MK_DIR = mkdir
else
	ifeq ($(shell uname), Linux)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), CYGWIN)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW32)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW64)
		MK_DIR = mkdir -p
	endif
endif

# List the subdirectories for creating object files
SUB_DIRS +=  \
 \
hpl/pm \
hpl/osc32kctrl \
hpl/ramecc \
hpl/dmac \
hal/src \
hpl/mclk \
hpl/systick \
hal/utils/src \
hpl/sercom \
examples \
hpl/gclk \
hpl/oscctrl \
hpl/core

# List the object files
OBJS +=  \
hal/src/hal_io.o \
hpl/systick/hpl_systick.o \
hpl/core/hpl_core_m4.o \
hal/src/hal_delay.o \
hpl/oscctrl/hpl_oscctrl.o \
hpl/core/hpl_init.o \
hpl/gclk/hpl_gclk.o \
hal/utils/src/utils_list.o \
hal/utils/src/utils_assert.o \
hpl/dmac/hpl_dmac.o \
hpl/pm/hpl_pm.o \
hal/src/hal_usart_sync.o \
hpl/mclk/hpl_mclk.o \
hpl/ramecc/hpl_ramecc.o \
hal/src/hal_init.o \
main.o \
hpl/osc32kctrl/hpl_osc32kctrl.o \
examples/driver_examples.o \
driver_init.o \
hpl/sercom/hpl_sercom.o \
hal/src/hal_gpio.o \
hal/utils/src/utils_event.o \
hal/src/hal_sleep.o \
atmel_start.o \
hal/src/hal_atomic.o

OBJS_AS_ARGS +=  \
"hal/src/hal_io.o" \
"hpl/systick/hpl_systick.o" \
"hpl/core/hpl_core_m4.o" \
"hal/src/hal_delay.o" \
"hpl/oscctrl/hpl_oscctrl.o" \
"hpl/core/hpl_init.o" \
"hpl/gclk/hpl_gclk.o" \
"hal/utils/src/utils_list.o" \
"hal/utils/src/utils_assert.o" \
"hpl/dmac/hpl_dmac.o" \
"hpl/pm/hpl_pm.o" \
"hal/src/hal_usart_sync.o" \
"hpl/mclk/hpl_mclk.o" \
"hpl/ramecc/hpl_ramecc.o" \
"hal/src/hal_init.o" \
"main.o" \
"hpl/osc32kctrl/hpl_osc32kctrl.o" \
"examples/driver_examples.o" \
"driver_init.o" \
"hpl/sercom/hpl_sercom.o" \
"hal/src/hal_gpio.o" \
"hal/utils/src/utils_event.o" \
"hal/src/hal_sleep.o" \
"atmel_start.o" \
"hal/src/hal_atomic.o"

# List the dependency files
DEPS := $(OBJS:%.o=%.d)

DEPS_AS_ARGS +=  \
"hal/utils/src/utils_event.d" \
"hal/src/hal_io.d" \
"hpl/ramecc/hpl_ramecc.d" \
"hpl/systick/hpl_systick.d" \
"hpl/core/hpl_core_m4.d" \
"hal/utils/src/utils_list.d" \
"hpl/dmac/hpl_dmac.d" \
"hal/utils/src/utils_assert.d" \
"hal/src/hal_delay.d" \
"hpl/core/hpl_init.d" \
"hpl/pm/hpl_pm.d" \
"hpl/gclk/hpl_gclk.d" \
"hal/src/hal_init.d" \
"hal/src/hal_usart_sync.d" \
"hpl/mclk/hpl_mclk.d" \
"driver_init.d" \
"hpl/osc32kctrl/hpl_osc32kctrl.d" \
"main.d" \
"examples/driver_examples.d" \
"hal/src/hal_sleep.d" \
"hpl/sercom/hpl_sercom.d" \
"hal/src/hal_gpio.d" \
"hal/src/hal_atomic.d" \
"hpl/oscctrl/hpl_oscctrl.d" \
"atmel_start.d"

OUTPUT_FILE_NAME :=AtmelStart
QUOTE := "
OUTPUT_FILE_PATH +=$(OUTPUT_FILE_NAME).elf
OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_FILE_NAME).elf

vpath %.c ../
vpath %.s ../
vpath %.S ../

# All Target
all: $(SUB_DIRS) $(OUTPUT_FILE_PATH)

# Linker target

$(OUTPUT_FILE_PATH): $(OBJS)
	@echo Building target: $@
	@echo Invoking: ARMCC Linker
	$(QUOTE)armlink$(QUOTE) --ro-base 0x00000000 --entry 0x00000000 --rw-base 0x20000000 --entry Reset_Handler --first __Vectors \
--strict --summary_stderr --info summarysizes --map --xref --callgraph --symbols \
--info sizes --info totals --info unused --info veneers --list $(OUTPUT_FILE_NAME).map \
-o $(OUTPUT_FILE_NAME).elf --cpu Cortex-M4 \
$(OBJS_AS_ARGS)

	@echo Finished building target: $@

# Compiler target(s)




%.o: %.c
	@echo Building file: $<
	@echo ARMCC Compiler
	$(QUOTE)armcc$(QUOTE) --c99 -c -DDEBUG -O1 -g --apcs=interwork --split_sections --cpu Cortex-M4 -D__SAMD51J19A__ \
-I"../" -I"../config" -I"../examples" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/gclk" -I"../hpl/mclk" -I"../hpl/osc32kctrl" -I"../hpl/oscctrl" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/ramecc" -I"../hpl/sercom" -I"../hpl/systick" -I"../hri" -I"../" -I"../CMSIS/Include" -I"../include"  \
--depend "$@"  -o "$@" "$<"

	@echo Finished building: $<

%.o: %.s
	@echo Building file: $<
	@echo ARMCC Assembler
	$(QUOTE)armasm$(QUOTE) -g --apcs=interwork --cpu Cortex-M4 --pd "D__SAMD51J19A__ SETA 1" \
-I"../" -I"../config" -I"../examples" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/gclk" -I"../hpl/mclk" -I"../hpl/osc32kctrl" -I"../hpl/oscctrl" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/ramecc" -I"../hpl/sercom" -I"../hpl/systick" -I"../hri" -I"../" -I"../CMSIS/Include" -I"../include"  \
--depend "$(@:%.o=%.d)" -o "$@" "$<"

	@echo Finished building: $<

%.o: %.S
	@echo Building file: $<
	@echo ARMCC Preprocessing Assembler
	$(QUOTE)armcc$(QUOTE) --c99 -c -DDEBUG -O1 -g --apcs=interwork --split_sections --cpu Cortex-M4 -D__SAMD51J19A__ \
-I"../" -I"../config" -I"../examples" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/gclk" -I"../hpl/mclk" -I"../hpl/osc32kctrl" -I"../hpl/oscctrl" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/ramecc" -I"../hpl/sercom" -I"../hpl/systick" -I"../hri" -I"../" -I"../CMSIS/Include" -I"../include"  \
--depend "$@"  -o "$@" "$<"

	@echo Finished building: $<

# Detect changes in the dependent files and recompile the respective object files.
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif
endif

$(SUB_DIRS):
	$(MK_DIR) "$@"

clean:
	rm -f $(OBJS_AS_ARGS)
	rm -f $(OUTPUT_FILE_PATH)
	rm -f $(DEPS_AS_ARGS)
	rm -f $(OUTPUT_FILE_NAME).map $(OUTPUT_FILE_NAME).elf
