ARMGCC = arm-none-eabi
PROGRAMMER = STM32_Programmer_CLI.exe

BINARY_NAME = main.bin

ifndef PORT
PORT = COM5
endif

# AOPS = --warn --fatal-warnings -mcpu=cortex-m0 -mthumb
# COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -mcpu=cortex-m0 -mthumb

AOPS = --warn -mcpu=cortex-m0
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding

DEST_FOLDER = target/
SRC_DIR = src/

# ------------------------------------------------------------------------------

all: $(BINARY_NAME)

# ------------------------------------------------------------------------------

# Compile Program and program STM32F030F4 over the defined Port (dafault COM5)
program: $(BINARY_NAME)
	$(PROGRAMMER) -c port=$(PORT) -w $(BINARY_NAME) 0x08000000

# Compile Code into Binary
main.bin : startup.ld $(DEST_FOLDER)startup.o  $(DEST_FOLDER)main.o \
			$(patsubst $(SRC_DIR)%.c, $(DEST_FOLDER)%.o, $(wildcard $(SRC_DIR)*.c))
	$(ARMGCC)-ld -o $(DEST_FOLDER)main.elf -T $^
	$(ARMGCC)-objdump -D $(DEST_FOLDER)main.elf > $(DEST_FOLDER)main.list
	$(ARMGCC)-objcopy $(DEST_FOLDER)main.elf $@ -O binary
	@$(MAKE) clean

# Default Rule for C-Files
$(DEST_FOLDER)%.o: %.c
	@$(MAKE) -s check_dir
	$(ARMGCC)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c $*.c -o $(DEST_FOLDER)$*.o

# Default Rule for C-Files
$(DEST_FOLDER)%.o: $(SRC_DIR)%.c
	@$(MAKE) -s check_dir
	$(ARMGCC)-gcc $(COPS) -mthumb -mcpu=cortex-m0 -march=armv6-m -c $(SRC_DIR)$*.c -o $(DEST_FOLDER)$*.o

# ------------------------------------------------------------------------------

# Default Rule for Assembler Files
$(DEST_FOLDER)startup.o : startup.S
	@$(MAKE) -s check_dir
	$(ARMGCC)-as $(AOPS) $< -o $@

# ------------------------------------------------------------------------------

.PHONY: check_dir

check_dir:
	@[ -d $(DEST_FOLDER) ] || mkdir $(DEST_FOLDER)

# ------------------------------------------------------------------------------

.PHONY: clean
.PHONY: clean_all

clean:
	# rm -f *.bin
	rm -f $(DEST_FOLDER)*.o $(DEST_FOLDER)*.o $(DEST_FOLDER)*.list $(DEST_FOLDER)*.elf
	# find . -name "*.o" -or -name "*.elf" -or -name "*.list" -or -name "*.srec" -delete

clean_all:
	rm -f *.bin
	$(MAKE) clean
