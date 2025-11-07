EMSDK_ENV = $(HOME)/wasm/emsdk/emsdk_env.sh
INCLUDE_DIR := $(HOME)/wasm/include 
LIB_DIR := $(HOME)/wasm/lib
BUILD_DIR := build/web
SRC_DIR := src
OBJ_DIR := build/obj

SOURCES2 := $(SRC_DIR)/main.c++ \
           $(SRC_DIR)/core/game.c++ \
           $(SRC_DIR)/world/city.c++ \
           $(SRC_DIR)/world/tile.c++ \
           $(SRC_DIR)/utils/isoutils.c++

SOURCES := $(shell find $(SRC_DIR) -name "*.c++")
OBJECTS := $(patsubst $(SRC_DIR)/%.c++,$(OBJ_DIR)/%.o,$(SOURCES))

CXXFLAGS := -I$(INCLUDE_DIR) \
            -I./external/raylib-cpp/include \
            -I$(HOME)/wasm/emsdk/upstream/emscripten/cache/sysroot/include \
            -I$(SRC_DIR) \
						-c -MMD -MP

LDFLAGS := -L$(LIB_DIR) -lraylib -s USE_GLFW=3 -s ASYNCIFY -s WASM=1 -O2

OUT_JS := $(BUILD_DIR)/game.js
OUT_WASM := $(BUILD_DIR)/game.wasm
OUT_HTML := $(BUILD_DIR)/game.html

all: web

$(OUT_JS) $(OUT_WASM) $(BUILD_DIR)/game.html: $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	# emmake handles the environment, so we can call em++ directly
	@em++ $(OBJECTS) $(LDFLAGS) -o $(BUILD_DIR)/game.html \
		--preload-file build/web/fire.png@fire.png
	@echo "WebAssembly build complete."

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c++
	@mkdir -p $(OBJ_DIR)/$(dir $*)
	# emmake handles the environment, so we can call em++ directly
	@em++ $< $(CXXFLAGS) -o $@
	@echo "Compiled: $< -> $@"

#$(OUT_JS) $(OUT_WASM): $(SOURCES)
#	@mkdir -p $(BUILD_DIR)
#	@bash -c "source $(EMSDK_ENV) && em++ $(SOURCES) $(CXXFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/game.html \
#		--preload-file build/web/fire.png@fire.png"
#	@echo "WebAssembly build complete."

$(OUT_HTML):
	@echo "<!DOCTYPE html><html><body><canvas id='canvas'></canvas><script src='game.js'></script></body></html>" > $(OUT_HTML)
	@echo "Created HTML wrapper."

web: $(OUT_JS) $(OUT_WASM) $(OUT_HTML)
	@echo "Web build is up to date"

run: web
	@echo "Starting local server at http://localhost:8080"
	@python3 -m http.server 8080 --directory $(BUILD_DIR)

clean:
	@echo "Cleaning build directory..."
	@rm -f $(OUT_JS) $(OUT_WASM) $(OUT_HTML)

.PHONY: all web clean run

-include $(OBJECTS:.o=.d)
