#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/plpos.kernel isodir/boot/plpos.kernel

cat > isodir/boot/grub/grub.cfg << EOF
menuentry "PLP OS" {
	multiboot /boot/plpos.kernel
}
EOF
grub-mkrescue -o plpos.iso isodir
