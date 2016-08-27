PROJ := libgloox

CC := $(shell xcrun --sdk iphoneos -f clang)
SDKROOT := $(shell xcrun --sdk iphoneos --show-sdk-path)
ARCH := arm64

SRCDIR := ./gloox-1.0.16/src
#LIBDIR := ./lib
BUILDDIR := ./build
TARGETDIR := ./gloox
TARGET := $(TARGETDIR)/lib/$(PROJ).a

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -maxdepth 1 -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# SRCEXT2 := cc
# SOURCES2 := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT2))
# OBJECTS2 := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES2:.$(SRCEXT2)=.o))


CFLAGS := -arch $(ARCH) -isysroot $(SDKROOT) -miphoneos-version-min=7.0 -fembed-bitcode 
#LDFLAGS := -arch $(ARCH) -isysroot $(SDKROOT)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGETDIR)/lib
	@mkdir -p $(TARGETDIR)/include/gloox
	@echo "Build shared library... $<"
	ar rcs $@ $(BUILDDIR)/*.o
	ranlib $@
	mkdir -p $(TARGETDIR)/include
	cp -f $(SRCDIR)/*.h $(TARGETDIR)/include/gloox/


$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo "Compile ...$<"
	@mkdir -p $(BUILDDIR)
	cp -f $(SRCDIR)/../config.h.unix.in $(SRCDIR)/config.h.unix
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -rf $(BUILDDIR)
	rm -rf $(TARGETDIR)