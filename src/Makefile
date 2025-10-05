TARGET =
EFI_TARGET =
CFILES = $(shell find core/ -name "*.c")
CFILES += platform/$(TARGET)/*.c
OBJ = $(CFILES:.c=.o)

.PHONY: all
all: target $(OBJ)
	lld-link -filealign:16 -subsystem:efi_application -nodefaultlib -dll \
		-entry:efi_main $(OBJ) -out:../BOOTX64.EFI
	rm -rf include/machine

.PHONY: target
target:
	mkdir -p include/machine/
	cp -r include/platform/$(TARGET)/* include/machine/

%.o: %.c
	$(CC) -c -Iinclude/ $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ)
