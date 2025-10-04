all:
	clang -target x86_64-pc-win32-coff -fno-stack-protector -fshort-wchar -mno-red-zone \
		-Iinclude/ -c src/main.c -o main.o
	lld-link -filealign:16 -subsystem:efi_application -nodefaultlib -dll -entry:efi_main main.o -out:BOOTX64.EFI
	bash mkiso.sh
	make clean

.PHONY: test
test:
	qemu-system-x86_64 -cdrom L5.iso   -drive if=pflash,format=raw,unit=0,file=ovmf.fd,readonly=on

clean:
	rm -r *.lib *.EFI *.o
