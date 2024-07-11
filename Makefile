BUILD = build
TARGET_NAME = test_cli
TARGET_EXE = $(BUILD)/$(TARGET_NAME)

CC := clang
LD := $(CC)
SIZE = size
ECHO = echo
MAKE = make

# SRC += $(wildcard *.c littlefs/*.c)
SRC += $(wildcard uwasm/*.c)
SRC += $(wildcard cli/*.c)
OBJ := $(SRC:%.c=$(BUILD)/%.o)

# target
CFLAGS += -std=c11
CFLAGS += -Wall
# opt
ifdef DEBUG
CFLAGS += -Wextra -Werror
CFLAGS += -O0 -g -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
CFLAGS += -D UWASM_DEBUG=1
CFLAGS += -Wno-unused -Wno-unused-parameter
else
CFLAGS += -Os
endif
# include header
CFLAGS += -Icli
CFLAGS += -Iuwasm

# link
LFLAGS += 
ifdef DEBUG
LFLAGS += -g -fsanitize=address
LFLAGS += -D UWASM_DEBUG=1
endif

all: $(TARGET_EXE)

$(TARGET_EXE): $(OBJ)
	@$(ECHO) Linking...
	@$(LD) $^ $(LFLAGS) -o $@
	@$(SIZE) $(TARGET_EXE)
	@$(ECHO) done.

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) -c -MMD $(CFLAGS) $< -o $@

clean:
	@$(ECHO) Cleaning...
	@rm -f $(TARGET_EXE)
	@rm -f $(OBJ)
	@$(ECHO) done.

run: all
	@$(ECHO) ================================
	@$(ECHO) "CFLAGS: $(CFLAGS)"
	@$(ECHO) "LFLAGS: $(LFLAGS)"
	@$(ECHO) ================================
	@chmod +x $(TARGET_EXE)
	@ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer ASAN_OPTIONS=detect_leaks=1 $(TARGET_EXE)

debug_run:
	@DEBUG=1 $(MAKE) --quiet clean
	@DEBUG=1 $(MAKE) --quiet run
