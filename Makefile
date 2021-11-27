BIN_FILE = tec

SRC_FILES += main.cpp
SRC_FILES += syntax_exception.cpp
SRC_FILES += token.cpp
SRC_FILES += preprocessor.cpp
SRC_FILES += stack.cpp
SRC_FILES += pda.cpp
SRC_FILES += template_symbols.cpp
SRC_FILES += 
SRC_FILES += 


VPATH += src

INSTALL_DIR = /usr/local/bin

override CFLAGS += -Iinclude
override CFLAGS += -Wall -Wextra -Wuninitialized
override CFLAGS += -std=c++14
override CFLAGS += -pipe
build: override CFLAGS += -g0 -s -O3
debug: override CFLAGS += -g3 -ggdb3 -O0 -DTEC_DEBUG

override LDFLAGS +=

CXX = g++

ifeq ($(CXX), g++)
	override LDFLAGS +=
endif

ifeq ($(CXX), clang++)
	override LDFLAGS +=
endif

ifeq ($(CXX), x86_64-w64-mingw32-g++)
	override CFLAGS +=
	override LDFLAGS +=
endif

ifeq ($(CXX), i686-w64-mingw32-g++)
	override CFLAGS +=
	override LDFLAGS +=
endif

OBJ_FILES := $(patsubst %.cpp,obj/%.o,$(SRC_FILES))
QUIET_CXX = @echo '   ' CXX $(notdir $@);

.PHONY: build debug test docs

all: build

build: mkdirs _build

debug: mkdirs _debug

_build: $(OBJ_FILES)
	$(CXX) $^ -o bin/$(BIN_FILE) $(LDFLAGS)

_debug: $(OBJ_FILES)
	$(CXX) $^ -o bin/$(BIN_FILE) $(LDFLAGS)

test:

docs:
	doxygen docs/Doxyfile

obj/%.o: %.cpp
	$(QUIET_CXX) $(CXX) -c $< -o $@ $(CFLAGS)

install: mkdirs build
	install bin/$(BIN_FILE) $(INSTALL_DIR)
	install util/tec-gcc $(INSTALL_DIR)
	install util/tec-clang $(INSTALL_DIR)
	install util/tec-i686-w64-mingw32-gcc $(INSTALL_DIR)
	install util/tec-x86_64-w64-mingw32-gcc $(INSTALL_DIR)

uninstall:
	rm -f $(INSTALL_DIR)/$(BIN_FILE)

clean:
	@rm -f obj/*
	@rm -f bin/*

mkdirs: 
	@if [ ! -d bin ]; then \
		mkdir bin; \
	fi

	@if [ ! -d obj ]; then \
		mkdir obj; \
	fi
