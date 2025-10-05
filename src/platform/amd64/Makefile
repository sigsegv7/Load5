CFILES = $(shell find . -name "*.c")
OBJ = $(CFILES:.c=.o)

.PHONY: all
all: $(OBJ)

%.o: %.c
	$(CC) -c -I../../include/ $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ)
