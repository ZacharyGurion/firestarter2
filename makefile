RAYLIB_SRC := external/raylib/src
RAYLIB_OUTPUT := build/raylib-wasm
RAYLIB_LIB_DIR := $(RAYLIB_OUTPUT)/lib
RAYLIB_INCLUDE_DIR := $(RAYLIB_OUTPUT)/include

RAYLIB_CPP_INCLUDE_DIR := ./external/raylib-cpp/include

BUILD_DIR := build
WEB_DIR := $(BUILD_DIR)/web
SRC_DIR := src
OBJ_DIR := $(BUILD_DIR)/obj

SOURCES := $(shell find $(SRC_DIR) -name "*.c++")
OBJECTS := $(patsubst $(SRC_DIR)/%.c++,$(OBJ_DIR)/%.o,$(SOURCES))

CXXFLAGS := -I$(RAYLIB_CPP_INCLUDE_DIR) \
						-I$(SRC_DIR) \
						-c -MMD -MP \
						-I$(RAYLIB_INCLUDE_DIR)

LDFLAGS := -L$(RAYLIB_LIB_DIR) -lraylib -s USE_GLFW=3 -s ASYNCIFY -s WASM=1 -O2

OUT_JS := $(WEB_DIR)/game.js
OUT_WASM := $(WEB_DIR)/game.wasm
OUT_HTML := $(WEB_DIR)/game.html

RAYLIB_LIB := $(RAYLIB_LIB_DIR)/libraylib.a

all: check-deps web

check-deps:
	@if [ ! -f "$(RAYLIB_LIB)" ]; then \
		echo "Error: raylib not built. Run 'make deps' first"; \
		exit 1; \
	fi
	
deps: $(RAYLIB_LIB)

$(RAYLIB_LIB): check-emsdk init-submodules
	@echo "Building raylib for WebAssembly"
	@mkdir -p $(RAYLIB_OUTPUT)
	@cd $(RAYLIB_SRC) && \
		emmake make PLATFORM=PLATFORM_WEB \
		USE_GLFW=YES \
		-s USE_GLFW=3 \
		-s ASYNCIFY=1 \
		-s WASM=1
	@mkdir -p $(RAYLIB_LIB_DIR) $(RAYLIB_INCLUDE_DIR)
	@cp $(RAYLIB_SRC)/libraylib.web.a $(RAYLIB_LIB_DIR)/libraylib.a
	@cp $(RAYLIB_SRC)/*.h $(RAYLIB_INCLUDE_DIR)/
	@echo "Raylib WASM build completed at: $(RAYLIB_LIB)"

check-emsdk:
	@if [ -z "$(EMSDK)" ]; then \
		echo "Error: EMSDK is not set. Source emsdk_env or use the make.sh script."; \
		exit 1; \
	else \
		echo "Using EMSDK at: $(EMSDK)"; \
	fi

init-submodules:
	@if [ ! -d "external/raylib" ]; then \
		echo "Initializing external/raylib..."; \
		git submodule update --init external/raylib; \
	fi
	@if [ ! -d "external/raylib-cpp" ]; then \
		echo "Initializing external/raylib-cpp..."; \
		git submodule update --init external/raylib-cpp; \
	fi
	@echo "Initializing git submodules"

clean-deps:
	@echo "Cleaning raylib build"
	@rm -rf $(RAYLIB_OUTPUT)
	@cd $(RAYLIB_SRC) && make clean 2>/dev/null || true
	@echo "Dependencies cleaned"

$(OUT_JS) $(OUT_WASM) $(OUT_HTML): $(OBJECTS)
	@mkdir -p $(WEB_DIR)
	@cp assets/spleen.otf $(WEB_DIR)/
	@cp assets/favicon.ico $(WEB_DIR)/
	@cp assets/index.html $(WEB_DIR)/
	@em++ $(OBJECTS) $(LDFLAGS) -o $(OUT_HTML) \
		--preload-file assets/spleen.otf@spleen.otf
	@mv $(OUT_HTML) $(WEB_DIR)/test.html

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c++
	@mkdir -p $(OBJ_DIR)/$(dir $*)
	@em++ $< $(CXXFLAGS) -o $@

web: $(OUT_JS) $(OUT_WASM) $(OUT_HTML)
	@echo "Web build complete"
	
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

.PHONY: all web clean run clean-obj clean-web deps

-include $(OBJECTS:.o=.d)
