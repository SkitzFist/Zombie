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
WEBFLAGS := -DPLATFORM_WEB -s USE_GLFW=3 -s FULL_ES2 -s FORCE_FILESYSTEM

# If on Windows, add specific flags
ifeq ($(OS),Windows_NT)
	WINDOWS_LIBS := -lwinmm -lgdi32 -luser32
endif

# Debug and Release flags
DEBUGFLAGS := -g -O0
RELEASEFLAGS := -O3

# Default to Debug build
BUILD := DEBUG

ifeq ($(BUILD), DEBUG)
	CFLAGS += $(DEBUGFLAGS)
else
	CFLAGS += $(RELEASEFLAGS)
endif

$(info SRCS is $(SRCS))
$(info OBJS is $(OBJS))

# Targets
desktop: LDFLAGS := -L$(DESKTOP_LIB_DIR) -lraylib
desktop: $(OBJS)
	$(CC) -o game $^ $(CFLAGS) $(LDFLAGS) $(WINDOWS_LIBS)

web: LDFLAGS := -L$(WEB_LIB_DIR) -lraylib # override LDFLAGS for web build
web: CC := em++ # set CC to em++ for web build
web: $(OBJS)
	em++ -o game.html $^ $(CFLAGS) $(LDFLAGS) $(WEBFLAGS) -lembind

# Rules
$(OBJDIR)%.o : $(SRCDIR)%.cpp
	$(info $(CC) compiling to .o)
	$(info $(CC) Compiling $< into $@)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
clean:
	@powershell Remove-Item $(OBJDIR)*.o -Force
	@powershell Remove-Item *.js -Force
	@powershell Remove-Item *.wasm -Force
	@powershell Remove-Item *.html -Force
	@powershell Remove-Item *.exe -Force