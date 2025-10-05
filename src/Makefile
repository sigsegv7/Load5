CFILES = $(shell find . -name "*.c")
OBJ = $(CFILES:.c=.o)

.PHONY: all
all: $(OBJ)
	lld-link -filealign:16 -subsystem:efi_application -nodefaultlib -dll \
		-entry:efi_main $(OBJ) -out:../BOOTX64.EFI

%.o: %.c
	$(CC) -c -I../include/ $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ)
