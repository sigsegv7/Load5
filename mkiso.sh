#!/bin/bash

mkdir -p iso_root/

dd if=/dev/zero of=iso_root/fat.img bs=1k count=1440
mformat -i iso_root/fat.img -f 1440 ::
mmd -i iso_root/fat.img ::/EFI
mmd -i iso_root/fat.img ::/EFI/BOOT
mcopy -i iso_root/fat.img BOOTX64.EFI ::/EFI/BOOT
mcopy -i iso_root/fat.img blobs/l5 ::

xorriso -as mkisofs -no-emul-boot -boot-load-size 4 \
    -boot-info-table --efi-boot fat.img -efi-boot-part \
    --efi-boot-image --protective-msdos-label iso_root -o L5.iso > /dev/null

rm -rf iso_root
