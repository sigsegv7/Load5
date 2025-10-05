TARGET = amd64
CC = clang
EFI_TARGET = x86_64-pc-win32-coff
CFLAGS = -target $(EFI_TARGET) -fno-stack-protector -fshort-wchar -mno-red-zone

all:
	make -C src/ EFI_TARGET=$(EFI_TARGET) TARGET=$(TARGET) \
		CFLAGS="$(CFLAGS)" CC=$(CC)
	bash mkiso.sh
	make clean

.PHONY: test
test:
	qemu-system-x86_64 -cdrom L5.iso   -drive if=pflash,format=raw,unit=0,file=ovmf.fd,readonly=on

clean:
	make -C src/ clean
	rm -f *.EFI *.lib
