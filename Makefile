TARGET = ./main.out
HDRS = project/include
VALGRIND_LOG = "valgrind.log"

SRCS = \
	   project/src/main.c \
	   project/src/emlparse.c \
	   project/src/custom_string.c

.PHONY: all check build test memtest rebuild clean

all: clean check build test memtest

check:
	./run_linters.sh

build: $(TARGET)

test: $(TARGET)
	$(TARGET)

memtest: $(TARGET)
	./btests/run.sh $(TARGET) --memcheck

rebuild: clean build

$(TARGET): $(SRCS)
	$(CC) -g -Wall -Wextra -Werror $(addprefix -I,$(HDRS)) -o $(TARGET) $(CFLAGS) $(SRCS)

clean:
	rm -f $(TARGET) ${VALGRIND_LOG}