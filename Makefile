SRC_DIR = ntrukem

# LDFLAGS=-lcrypto
# $(SRC_DIR)/crypto_sort.c
SRC = $(SRC_DIR)/poly.c $(SRC_DIR)/fips202.c \
	  $(SRC_DIR)/randombytes.c $(SRC_DIR)/cbd.c \
	  $(SRC_DIR)/verify.c $(SRC_DIR)/owcpa.c \
	  $(SRC_DIR)/ntrukem.c \
	  c_src/ntrukem_nif.c

ERLANG_PATH = $(shell erl -eval 'io:format("~s", [lists:concat([code:root_dir(), "/erts-", erlang:system_info(version), "/include"])])' -s init stop -noshell)
CFLAGS = -O3 -fomit-frame-pointer -march=native -fPIE -pie
CFLAGS += -Wall -Wextra -Wpedantic -Wshadow -Wundef -g -I$(SRC_DIR) -Ic_src -I$(ERLANG_PATH)

KERNEL_NAME := $(shell uname -s)

LIB_NAME = priv/ntrukem_nif.so
ifneq ($(CROSSCOMPILE),)
	LIB_CFLAGS := -shared -fPIC -fvisibility=hidden
	SO_LDFLAGS := -Wl,-soname,ntrukem.so.0
else
	ifeq ($(KERNEL_NAME), Linux)
		LIB_CFLAGS := -shared -fPIC -fvisibility=hidden
		SO_LDFLAGS := -Wl,-soname,ntrukem.so.0
	endif
	ifeq ($(KERNEL_NAME), Darwin)
		LIB_CFLAGS := -dynamiclib -undefined dynamic_lookup
	endif
	ifeq ($(KERNEL_NAME), $(filter $(KERNEL_NAME),OpenBSD FreeBSD NetBSD))
		LIB_CFLAGS := -shared -fPIC
	endif
endif

all: $(LIB_NAME)

$(LIB_NAME): $(SRC) 
	mkdir -p priv
	$(CC) $(CFLAGS) $(LIB_CFLAGS) $(SO_LDFLAGS) $^ -o $@

clean:
	rm -f $(LIB_NAME)

.PHONY: all clean
