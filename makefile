#INCLUDE_DIR := $(HOME)/wasm/include 
#LIB_DIR := $(HOME)/wasm/lib
RAYLIB_SRC := external/raylib/src
RAYLIB_OUTPUT := external/raylib-wasm
LIB_DIR := $(RAYLIB_OUTPUT)/lib

BUILD_DIR := build
WEB_DIR := $(BUILD_DIR)/web
SRC_DIR := src
OBJ_DIR := $(BUILD_DIR)/obj

SOURCES := $(shell find $(SRC_DIR) -name "*.c++")
OBJECTS := $(patsubst $(SRC_DIR)/%.c++,$(OBJ_DIR)/%.o,$(SOURCES))

CXXFLAGS := -I./external/raylib-cpp/include \
						-I$(SRC_DIR) \
						-c -MMD -MP \
						-I./external/raylib/src
						#-I$(INCLUDE_DIR) \

LDFLAGS := -L$(LIB_DIR) -lraylib -s USE_GLFW=3 -s ASYNCIFY -s WASM=1 -O2

OUT_JS := $(WEB_DIR)/game.js
OUT_WASM := $(WEB_DIR)/game.wasm
OUT_HTML := $(WEB_DIR)/game.html

RAYLIB_LIB := $(LIB_DIR)/libraylib.a

all: check-deps web

check-deps:
	@if [ ! -f "$(RAYLIB_LIB)" ]; then \
		echo "Error: raylib not built. Run 'make deps' first"; \
		exit 1; \
	fi
	
deps: check-emsdk init-submodules build-raylib

check-emsdk:
	@if [ ! -d "$(EMSDK_DIR)" ]; then \
		echo "Error: Emscripten SDK not found at $(EMSDK_DIR)"; \
		echo "Clone it with: git clone https://github.com/emscripten-core/emsdk.git external/emsdk"; \
		echo "Then run: cd external/emsdk && ./emsdk install latest && ./emsdk activate latest"; \
		exit 1; \
	fi
	@if [ ! -f "$(EMSDK_DIR)/emsdk_env.sh" ]; then \
		echo "Error: Emscripten not activated"; \
		echo "Run: cd external/emsdk && ./emsdk install latest && ./emsdk activate latest"; \
		exit 1; \
	fi

init-submodules:
	@echo "Initializing git submodules..."
	@git submodule update --init --recursive
	@echo "Submodules initialized"

clean-deps:
	@echo "Cleaning raylib build..."
	@rm -rf $(RAYLIB_OUTPUT)
	@cd $(RAYLIB_SRC) && make clean 2>/dev/null || true
	@echo "Dependencies cleaned"

build-raylib: check-emsdk
	@echo "Building raylib for WebAssembly..."
	@if [ -f "$(RAYLIB_LIB)" ]; then \
		echo "raylib already built at $(RAYLIB_LIB)"; \
	else \
		mkdir -p $(LIB_DIR) $(INCLUDE_DIR); \
		cd $(RAYLIB_SRC) && \
		cp $(RAYLIB_SRC)/libraylib.a $(LIB_DIR)/; \
		cp $(RAYLIB_SRC)/raylib.h $(RAYLIB_SRC)/raymath.h $(RAYLIB_SRC)/rlgl.h $(INCLUDE_DIR)/; \
		echo "raylib built successfully"; \
	fi

$(OUT_JS) $(OUT_WASM) $(WEB_DIR)/game.html: $(OBJECTS)
	@mkdir -p $(WEB_DIR)
	cp assets/spleen.otf $(WEB_DIR)/
	cp assets/favicon.ico $(WEB_DIR)/
	cp assets/index.html $(WEB_DIR)/
	@em++ $(OBJECTS) $(LDFLAGS) -o $(WEB_DIR)/game.html \
		--preload-file assets/spleen.otf@spleen.otf #\
		# --preload-file build/web/fire.png@fire.png
	@echo "WebAssembly build complete."

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c++
	@mkdir -p $(OBJ_DIR)/$(dir $*)
	@em++ $< $(CXXFLAGS) -o $@
	@echo "Compiled: $< -> $@"

web: $(OUT_JS) $(OUT_WASM) $(WEB_DIR)/game.html
	@echo "Web build is up to date"

run: web
	@echo "Starting local server at http://localhost:8080"
	@python3 -m http.server 8080 --directory $(WEB_DIR)

clean-web:
	@echo "Cleaning web directory..."
	@rm -rf $(WEB_DIR)

clean-obj:
	@echo "Cleaning obj directory..."
	@rm -rf $(OBJ_DIR)

clean: clean-web clean-obj
	@echo "Cleaned build"

.PHONY: all web clean run

-include $(OBJECTS:.o=.d)
