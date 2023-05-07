dd if=/dev/zero of=$(pwd)/disk.img bs=1M count=1024

losetup /dev/loop30 $(pwd)/disk.img

mkfs.ext4 $(pwd)/disk.img


# mount -o rw,loop /dev/loop0 $(pwd)/mount



# touch $(pwd)/mount/myfile.txt


# umount /dev/loop0
# losetup -d /dev/loop0