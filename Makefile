OS=$(shell uname -s)

# The directories containing the source files, separated by ':'
# "VPATH" is a builtin by make, do not rename
VPATH=Source

# Output folder prefix, suffix
OUT_DIR_PREFIX=
OUT_DIR_SUFFIX=

# Output folders
OUT_DIR_DEBUG=$(OUT_DIR_PREFIX)Debug$(OUT_DIR_SUFFIX)
OUT_DIR_RELEASE=$(OUT_DIR_PREFIX)Release$(OUT_DIR_SUFFIX)
OUT_DIR_DEPS_DEBUG=$(OUT_DIR_DEBUG)/Dep
OUT_DIR_OBJS_DEBUG=$(OUT_DIR_DEBUG)/Obj
OUT_DIR_BIN_DEBUG=$(OUT_DIR_DEBUG)/Bin
OUT_DIR_GEN_DEBUG=$(OUT_DIR_DEBUG)/Gen
OUT_DIR_DEPS_RELEASE=$(OUT_DIR_RELEASE)/Dep
OUT_DIR_OBJS_RELEASE=$(OUT_DIR_RELEASE)/Obj
OUT_DIR_BIN_RELEASE=$(OUT_DIR_RELEASE)/Bin
OUT_DIR_GEN_RELEASE=$(OUT_DIR_RELEASE)/Gen

# Default build type
ifeq ($(BUILD_TYPE),)
BUILD_TYPE=debug
endif

ifeq ($(BUILD_TYPE),debug)
OUT_DIR_DEPS=$(OUT_DIR_DEPS_DEBUG)
OUT_DIR_OBJS=$(OUT_DIR_OBJS_DEBUG)
OUT_DIR_BIN=$(OUT_DIR_BIN_DEBUG)
OUT_DIR_GEN=$(OUT_DIR_GEN_DEBUG)
else
OUT_DIR_DEPS=$(OUT_DIR_DEPS_RELEASE)
OUT_DIR_OBJS=$(OUT_DIR_OBJS_RELEASE)
OUT_DIR_BIN=$(OUT_DIR_BIN_RELEASE)
OUT_DIR_GEN=$(OUT_DIR_GEN_RELEASE)
endif

# List of sources (regardless of directories), located by VPATH
Group0_SRC = \
	Assert.cpp \
	ConsoleIO.cpp \
	CStringOps.cpp \
    Settings.cpp \
	Main.cpp \
	TextIO.cpp \
	XAtom.cpp \
	XErrors.cpp \
	XEvaluator.cpp \
	XFunction.cpp \
	XOperator.cpp \
	XVariable.cpp


# Build a Dependency list and an Object list, by replacing the .cpp
# extension to .d for dependency files, and .o for object files.
Group0_DEP = $(patsubst %.cpp, $(OUT_DIR_DEPS)/Group0_%.d, ${Group0_SRC})
Group0_OBJ = $(patsubst %.cpp, $(OUT_DIR_OBJS)/Group0_%.o, ${Group0_SRC})


# The final binary
TARGET = xank

# What compiler to use for generating dependencies: 
# it will be invoked with -MM -MP
CCDEP = g++
CC = g++

# What include flags to pass to the compiler
INC_FLAGS = -I Source -I $(OUT_DIR_GEN)

C_FLAGS_COMMON = -Wall -Wextra -pedantic -Wshadow -Wunused-function -Wunused-label -Wunused-value -Wunused-variable
C_FLAGS_COMMON += -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS

ifeq ($(BUILD_TYPE),debug)
C_FLAGS += -g $(C_FLAGS_COMMON) -D_DEBUG -DXANK_DEBUG ${INC_FLAGS}
else
C_FLAGS += -O2 -Wuninitialized $(C_FLAGS_COMMON) ${INC_FLAGS}
endif

# Common linker flags for all build types
LD_FLAGS += -ltermcap -lreadline -lgmpxx -lgmp

# OS specifics
ifeq ($(OS), SunOS)
 OS_NAME=XANK_OS_SOLARIS
 INC_FLAGS += -I /usr/include/gmp/
else ifeq ($(OS), Linux)
 OS_NAME= XANK_OS_LINUX
else ifeq ($(OS), Darwin)
 OS_NAME=XANK_OS_DARWIN
else
 $(error blah)
endif

C_FLAGS += -D${OS_NAME}

all: begin $(OUT_DIR_BIN)/${TARGET} done

# This is fugly, I don't know how to have a dependency on Errors.cpp before "all" rule
# Ugh, right now it works so meh.
Source/XErrors.cpp : $(OUT_DIR_GEN)/GenErrorData.h

begin:
ifneq ($(BUILD_TYPE),release)
ifneq ($(BUILD_TYPE),debug)
@echo "BUILD_TYPE '"$(BUILD_TYPE)"' is invalid."
@echo "You must specify a build type when running make, e.g."
@echo  "make BUILD_TYPE=debug"
@echo
@echo  "Valid BUILD_TYPE values 'release', 'debug'"
@exit 1
endif
endif

$(OUT_DIR_GEN)/GenErrorData.h: Source/XErrors.h
	@mkdir -p $(OUT_DIR_GEN)
	@echo "Generating error codes into $@"
	@sed -f Source/ErrorData.sed Source/XErrors.h > $@

done:
	@echo "Done."

$(OUT_DIR_BIN)/${TARGET}: ${Group0_OBJ} | begin
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ ${LD_FLAGS}
	ln -sf $(OUT_DIR_BIN) bin

$(OUT_DIR_OBJS)/Group0_%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) -c $(C_FLAGS) -o $@ $<

$(OUT_DIR_DEPS)/Group0_%.d: %.cpp
	@mkdir -p $(dir $@)
	@echo Generating $(BUILD_TYPE) dependencies for $<
	@set -e ; $(CCDEP) -MM -MP $(INC_FLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OUT_DIR_OBJS)\/Group0_\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean:
	@rm -rf \
	$(OUT_DIR_DEBUG) \
	$(OUT_DIR_RELEASE) \
	bin

# Unless "make clean" is called, include the dependency files
# which are auto-generated. Don't fail if they are missing
# (-include), since they will be missing in the first invocation!
ifneq ($(MAKECMDGOALS),clean)
-include ${Group0_DEP}
endif


