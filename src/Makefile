TARGET =
EFI_TARGET =
CFILES = $(shell find core/ -name "*.c")
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
	make -C platform/$(TARGET) EFI_TARGET=$(EFI_TARGET) TARGET=$(TARGET) \
		CFLAGS="$(CFLAGS)" CC=$(CC)

%.o: %.c
	$(CC) -c -Iinclude/ $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ)
