CC := g++

OBJDIR := obj/
SRCDIR := src/
WEB_LIB_DIR := lib/web
DESKTOP_LIB_DIR := lib/desktop

SRCS := $(shell powershell -Command "Get-ChildItem -Recurse -Path $(SRCDIR) -Filter *.cpp | ForEach-Object { '$(SRCDIR)' + $$_ -replace '\\', '/' }")
OBJS := $(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%.o,$(SRCS))

# Flags
CFLAGS := -Wall -Iinclude/ -g -O0
LDFLAGS := -lraylib
WEBFLAGS := -DPLATFORM_WEB -s ALLOW_MEMORY_GROWTH=1 -s ASYNCIFY -s USE_GLFW=3 -s FULL_ES2 -s FORCE_FILESYSTEM

# If on Windows, add specific flags
ifeq ($(OS),Windows_NT)
	WINDOWS_LIBS := -lwinmm -lgdi32 -luser32
endif

# Debug and Release flags
DEBUGFLAGS := -g -O0 -DDEBUG
RELEASEFLAGS := -O3

# Default to Debug build
BUILD := DEBUG

ifeq ($(BUILD), DEBUG)
	CFLAGS += $(DEBUGFLAGS)
else
	CFLAGS += $(RELEASEFLAGS)
endif

# Targets
desktop: LDFLAGS := -L$(DESKTOP_LIB_DIR) -lraylib
desktop: $(OBJS)
	$(CC) -o game $^ $(CFLAGS) $(LDFLAGS) $(WINDOWS_LIBS)

web: CC := em++ # set CC to em++ for web build
web: CFLAGS += -DPLATFORM_WEB # add -DPLATFORM_WEB to CFLAGS for web build
web: LDFLAGS := -L$(WEB_LIB_DIR) -lraylib $(WEBFLAGS) # add WEBFLAGS to LDFLAGS for web build
web: $(OBJS)
	em++ -o game.html $^ $(CFLAGS) $(LDFLAGS) -lembind

# Rules
$(OBJDIR)%.o : $(SRCDIR)%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@powershell if (Test-Path $(OBJDIR)*.o) { Remove-Item $(OBJDIR)*.o -Force }
	@powershell if (Test-Path *.exe) { Remove-Item *.exe -Force }
	@powershell if (Test-Path *.js) { Remove-Item *.js -Force }
	@powershell if (Test-Path *.wasm) { Remove-Item *.wasm -Force }
	@powershell if (Test-Path game.html) { Remove-Item game.html -Force }