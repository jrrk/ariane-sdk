diff -urN -X obj.txt original/linux-4.20-rc2/drivers/net/ethernet patched/linux-4.20-rc2/drivers/net/ethernet | sed -e 's=original/==' -e 's=patched/==' >configs/0099-lowrisc-ethernet.patch
rm -rf buildroot/output/build/linux-4.20-rc2
make -C buildroot
cd build && ../riscv-pk/configure --host=riscv64-unknown-elf --with-payload=../buildroot/output/images/vmlinux --enable-logo --with-logo=../configs/logo.txt && make
cd ..
riscv64-unknown-elf-objcopy -S -O binary --change-addresses -0x80000000 build/bbl bbl.bin
