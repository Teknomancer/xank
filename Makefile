OS=$(shell uname -s)

# The directories containing the source files, separated by ':'
# "VPATH" is a builtin by make, do not rename
VPATH=Source

# Output folder prefix, suffix
OUT_DIR_PREFIX=
OUT_DIR_SUFFIX=

# Output folders
OUT_DIR_DEPS_DEBUG=$(OUT_DIR_PREFIX)Debug/Dep$(OUT_DIR_SUFFIX)
OUT_DIR_DEPS_RELEASE=$(OUT_DIR_PREFIX)Release/Dep$(OUT_DIR_SUFFIX)
OUT_DIR_OBJS_DEBUG=$(OUT_DIR_PREFIX)Debug/Obj$(OUT_DIR_SUFFIX)
OUT_DIR_OBJS_RELEASE=$(OUT_DIR_PREFIX)Release/Obj$(OUT_DIR_SUFFIX)
OUT_DIR_BIN_DEBUG=$(OUT_DIR_PREFIX)Debug/Bin$(OUT_DIR_SUFFIX)
OUT_DIR_BIN_RELEASE=$(OUT_DIR_PREFIX)Release/Bin$(OUT_DIR_SUFFIX)
OUT_DIR_GEN_DEBUG=$(OUT_DIR_PREFIX)Debug/Gen$(OUT_DIR_SUFFIX)
OUT_DIR_GEN_RELEASE=$(OUT_DIR_PREFIX)Release/Gen$(OUT_DIR_SUFFIX)

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
	Atom.cpp \
    ConsoleIO.cpp \
	Errors.cpp \
	Evaluator.cpp \
	Function.cpp \
	Main.cpp \
	Operator.cpp \
    Variable.cpp


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

C_FLAGS_COMMON = -Wall -pedantic -Wshadow -Wunused-function -Wunused-label -Wunused-value -Wunused-variable

ifeq ($(BUILD_TYPE),debug)
C_FLAGS += -g $(C_FLAGS_COMMON) -D_DEBUG -DXANK_DEBUG ${INC_FLAGS}
else
C_FLAGS += -O2 -Wuninitialized $(C_FLAGS_COMMON) ${INC_FLAGS}
endif

# Common linker flags for all build types
LD_FLAGS += -ltermcap -lreadline -lgmp -lm

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
Source/Errors.cpp : $(OUT_DIR_GEN)/GenErrorData.h

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

$(OUT_DIR_GEN)/GenErrorData.h: Source/Errors.h
	@mkdir -p $(OUT_DIR_GEN)
	@echo "Generating error codes into $@"
	@sed -f Source/ErrorData.sed Source/Errors.h > $@

done:
	@echo "Done."

$(OUT_DIR_BIN)/${TARGET}: ${Group0_OBJ} | begin
	@mkdir -p $(dir $@)
	$(CC) -g -o $@ $^ ${LD_FLAGS}
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
	$(OUT_DIR_DEPS_DEBUG) $(OUT_DIR_OBJS_DEBUG) $(OUT_DIR_BIN_DEBUG) $(OUT_DIR_GEN_DEBUG) \
	$(OUT_DIR_DEPS_RELEASE) $(OUT_DIR_OBJS_RELEASE) $(OUT_DIR_BIN_RELEASE) $(OUT_DIR_GEN_RELEASE) \
	bin

# Unless "make clean" is called, include the dependency files
# which are auto-generated. Don't fail if they are missing
# (-include), since they will be missing in the first invocation!
ifneq ($(MAKECMDGOALS),clean)
-include ${Group0_DEP}
endif


