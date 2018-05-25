CC ?= gcc
CFLAGS ?= -Wall -std=gnu99 -g -O0

EXEC = gpio_mmio
.PHONY: all
all: $(EXEC)

gpio_mmio: mmio.c
	$(CC) $(CFLAGS) -o $@ mmio.c

.PHONY: run
run:
	sudo ./$(EXEC)

.PHONY: clean
clean:
	$(RM) $(EXEC)

