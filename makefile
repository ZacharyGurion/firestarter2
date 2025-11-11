EMSDK_ENV = $(HOME)/wasm/emsdk/emsdk_env.sh
INCLUDE_DIR := $(HOME)/wasm/include 
LIB_DIR := $(HOME)/wasm/lib
BUILD_DIR := build
WEB_DIR := $(BUILD_DIR)/web
SRC_DIR := src
OBJ_DIR := $(BUILD_DIR)/obj

SOURCES2 := $(SRC_DIR)/main.c++ \
						$(SRC_DIR)/core/game.c++ \
						$(SRC_DIR)/world/city.c++ \
						$(SRC_DIR)/world/tile.c++ \
						$(SRC_DIR)/utils/isoutils.c++ \
						$(SRC_DIR)/entities/building.c++

SOURCES := $(shell find $(SRC_DIR) -name "*.c++")
OBJECTS := $(patsubst $(SRC_DIR)/%.c++,$(OBJ_DIR)/%.o,$(SOURCES))

CXXFLAGS := -I$(INCLUDE_DIR) \
						-I./external/raylib-cpp/include \
						-I$(HOME)/wasm/emsdk/upstream/emscripten/cache/sysroot/include \
						-I$(SRC_DIR) \
						-c -MMD -MP

LDFLAGS := -L$(LIB_DIR) -lraylib -s USE_GLFW=3 -s ASYNCIFY -s WASM=1 -O2

OUT_JS := $(WEB_DIR)/game.js
OUT_WASM := $(WEB_DIR)/game.wasm
OUT_HTML := $(WEB_DIR)/game.html

all: web

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
