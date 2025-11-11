#INCLUDE_DIR := $(HOME)/wasm/include 
#LIB_DIR := $(HOME)/wasm/lib
RAYLIB_SRC := external/raylib/src
RAYLIB_OUTPUT := build/raylib-wasm
LIB_DIR := $(RAYLIB_OUTPUT)/lib
INCLUDE_DIR := $(RAYLIB_OUTPUT)/include

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

ifndef EMSDK
$(error EMSDK is not set. Source emsdk_env or use the make.sh script.)
endif

all: check-deps web

check-deps:
	@if [ ! -f "$(RAYLIB_LIB)" ]; then \
		echo "Error: raylib not built. Run 'make deps' first"; \
		exit 1; \
	fi
	
deps: check-emsdk init-submodules build-raylib

check-emsdk:
	@echo "Using EMSDK at: $(EMSDK)"

init-submodules:
	@echo "Initializing git submodules..."
	@git submodule update --init external/raylib
	@git submodule update --init external/raylib-cpp
	@echo "Submodules initialized"

clean-deps:
	@echo "Cleaning raylib build..."
	@rm -rf $(RAYLIB_OUTPUT)
	@cd $(RAYLIB_SRC) && make clean 2>/dev/null || true
	@echo "Dependencies cleaned"

build-raylib: check-emsdk
	@echo "Building raylib for WebAssembly"
	@mkdir -p $(RAYLIB_OUTPUT)
	@cd $(RAYLIB_SRC) && \
		emmake make PLATFORM=PLATFORM_WEB \
		USE_GLFW=YES \
		-s USE_GLFW=3 \
		-s ASYNCIFY=1 \
		-s WASM=1
	@mkdir -p $(LIB_DIR) $(INCLUDE_DIR)
	@cp $(RAYLIB_SRC)/libraylib.web.a $(LIB_DIR)/libraylib.a
	@cp $(RAYLIB_SRC)/*.h $(INCLUDE_DIR)/
	@echo "Raylib WASM build completed at: $(RAYLIB_LIB)"

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
