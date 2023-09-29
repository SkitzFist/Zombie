CC := g++

OBJDIR := obj/
SRCDIR := src/

SRCS := $(shell powershell -Command "Get-ChildItem -Recurse -Path $(SRCDIR) -Filter *.cpp | ForEach-Object { '$(SRCDIR)' + $$_ -replace '\\', '/' }")
OBJS := $(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%.o,$(SRCS))

# Flags
CFLAGS := -Wall -Iinclude/ -Llib/ -g -O0
LDFLAGS := -lraylib
WEBFLAGS := -DPLATFORM_WEB -s USE_GLFW=3 -s FULL_ES2 -s FORCE_FILESYSTEM

ifeq ($(OS),Windows_NT)
	LDFLAGS += -lwinmm -lgdi32 -luser32
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
desktop: $(OBJS)
	$(CC) -o game $^ $(CFLAGS) $(LDFLAGS)

web: CC := em++ # set CC to em++ for web build
web: $(OBJS)
	em++ -lembind -o game.html $^ $(CFLAGS) $(WEBFLAGS)

# Rules
$(OBJDIR)%.o : $(SRCDIR)%.cpp
	$(info compiling to .o)
	$(info Compiling $< into $@)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
clean:
	@powershell Remove-Item $(OBJDIR)*.o -Force
	@powershell Remove-Item *.js -Force
	@powershell Remove-Item *.wasm -Force
	@powershell Remove-Item *.html -Force
	@powershell Remove-Item *.exe -Force