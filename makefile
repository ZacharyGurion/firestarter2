# === Paths ===
EMSDK_ENV = $(HOME)/wasm/emsdk/emsdk_env.sh
INCLUDE_DIR := $(HOME)/wasm/include
LIB_DIR := $(HOME)/wasm/lib
BUILD_DIR := build/web

MAIN := src/main.c++

# Compiler flags
CXXFLAGS := -I$(INCLUDE_DIR)
LDFLAGS := -L$(LIB_DIR) -lraylib -s USE_GLFW=3 -s ASYNCIFY -s WASM=1 -O2

OUT_JS := $(BUILD_DIR)/game.js
OUT_WASM := $(BUILD_DIR)/game.wasm
OUT_HTML := $(BUILD_DIR)/game.html

# Targets
all: web

$(OUT_JS) $(OUT_WASM): $(MAIN)
	@mkdir -p $(BUILD_DIR)
	@bash -c "source $(EMSDK_ENV) && em++ $(MAIN) $(CXXFLAGS) $(LDFLAGS) -o $(BUILD_DIR)/game.html"
	@echo "✅ WebAssembly build complete."

$(OUT_HTML):
	@echo "<!DOCTYPE html><html><body><canvas id='canvas'></canvas><script src='game.js'></script></body></html>" > $(OUT_HTML)
	@echo "✅ Created HTML wrapper."

web: $(OUT_JS) $(OUT_WASM) $(OUT_HTML)
	@echo "Web build is up to date"

run: web
	@echo "Starting local server at http://localhost:8080"
	@python3 -m http.server 8080 --directory $(BUILD_DIR)

clean:
	@echo "Cleaning build directory..."
	@rm -f $(OUT_JS) $(OUT_WASM)

.PHONY: all web clean
