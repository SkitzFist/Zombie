CC := g++

OBJDIR := obj/
SRCDIR := src/

SRCS := $(shell powershell -Command "Get-ChildItem -Recurse -Path $(SRCDIR) -Filter *.cpp | ForEach-Object { '$(SRCDIR)' + $$_ -replace '\\', '/' }")
OBJS := $(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%.o,$(SRCS))

# Flags
CFLAGS := -Wall -I include/ -L lib/ -lraylib
WEBFLAGS := -DPLATFORM_WEB -s USE_GLFW=3 -sFULL_ES2 -sFORCE_FILESYSTEM

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
	g++ -o game $^ $(CFLAGS)

web: CC := em++ # set CC to em++ for web build
web: $(OBJS)
	em++ -o game.html $^ $(CFLAGS) $(WEBFLAGS)

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