#compile The UserApp1 and embed it into the kernel
aarch64-linux-gnu-gcc -S -O0 -o UserApps/outs/syscalls.s UserApps/stdLib/syscalls.c -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd
aarch64-linux-gnu-gcc -S -O0 -o UserApps/outs/heap.s UserApps/stdLib/heap.c -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd
aarch64-linux-gnu-gcc -S -O0 -o UserApps/outs/string.s UserApps/stdLib/string.c -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd
aarch64-linux-gnu-gcc -S -O0 -o UserApps/outs/bash.s UserApps/bash.c -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd
aarch64-linux-gnu-gcc -S -O0 -o UserApps/outs/testApp1.s UserApps/testApp1.c -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd

aarch64-linux-gnu-as -o UserApps/outs/testApp1.o UserApps/outs/testApp1.s
aarch64-linux-gnu-as -o UserApps/outs/syscalls.o UserApps/outs/syscalls.s
aarch64-linux-gnu-as -o UserApps/outs/heap.o UserApps/outs/heap.s
aarch64-linux-gnu-as -o UserApps/outs/string.o UserApps/outs/string.s
aarch64-linux-gnu-as -o UserApps/outs/bash.o UserApps/outs/bash.s

aarch64-linux-gnu-ld -T UserApps/linker1.ld -nostdlib -o UserApps/outs/testApp1.elf UserApps/outs/testApp1.o UserApps/outs/syscalls.o UserApps/outs/heap.o UserApps/outs/string.o UserApps/outs/bash.o 
aarch64-linux-gnu-objcopy -O binary  UserApps/outs/testApp1.elf  UserApps/outs/testApp1.bin

aarch64-linux-gnu-objcopy -I binary -O elf64-littleaarch64 -B aarch64 --add-section .userapp_image=UserApps/outs/testApp1.bin UserApps/outs/testApp1.o

#compile The UserApp2 and embed it into the kernel
aarch64-linux-gnu-gcc -S -O0 -o UserApps/outs/Userapp2.s UserApps/Userapp2.c -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd
aarch64-linux-gnu-as -o UserApps/outs/Userapp2.o UserApps/outs/Userapp2.s

aarch64-linux-gnu-ld -T UserApps/linker2.ld -nostdlib -o UserApps/outs/Userapp2.elf UserApps/outs/Userapp2.o UserApps/outs/syscalls.o
aarch64-linux-gnu-objcopy -O binary  UserApps/outs/Userapp2.elf  UserApps/outs/Userapp2.bin

aarch64-linux-gnu-objcopy -I binary -O elf64-littleaarch64 -B aarch64 --add-section .userapp_image2=UserApps/outs/Userapp2.bin UserApps/outs/Userapp2.o 


#compile the raw kernel image
aarch64-linux-gnu-gcc -S -O0 -o kernelim/outs/uart.s kernelim/utils/uart.c -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd 
aarch64-linux-gnu-gcc -S -O0 -o kernelim/outs/kernel.s kernelim/kernel.c -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd 
aarch64-linux-gnu-gcc -S -O0 -o kernelim/outs/mmu.s kernelim/mmu.c -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd 
aarch64-linux-gnu-gcc -S -O0 -o kernelim/outs/sprintf.s kernelim/utils/sprintf.c -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd 
aarch64-linux-gnu-gcc -S -O0 -o kernelim/outs/processes.s kernelim/processes.c -ffreestanding -mcpu=cortex-a53+nosimd 
aarch64-linux-gnu-gcc -S -O0 -o kernelim/outs/exeptions.s kernelim/exeptions.c -ffreestanding -mcpu=cortex-a53+nosimd 
aarch64-linux-gnu-gcc -S -O0 -o kernelim/outs/cpuContext.s kernelim/utils/cpuContext.c -ffreestanding -mcpu=cortex-a53+nosimd 
aarch64-linux-gnu-gcc -S -O0 -o kernelim/outs/syscalls.s kernelim/syscalls.c -ffreestanding -mcpu=cortex-a53+nosimd 
aarch64-linux-gnu-gcc -S -O0 -o kernelim/outs/memory.s kernelim/memory.c -ffreestanding -mcpu=cortex-a53+nosimd 
aarch64-linux-gnu-gcc -S -O0 -o kernelim/outs/heap.s kernelim/heap.c -ffreestanding -mcpu=cortex-a53+nosimd 
aarch64-linux-gnu-gcc -S -O0 -o kernelim/outs/filesys.s kernelim/filesys.c -ffreestanding -mcpu=cortex-a53+nosimd 
aarch64-linux-gnu-gcc -S -O0 -o kernelim/outs/fd.s kernelim/utils/fd.c -ffreestanding -mcpu=cortex-a53+nosimd 


aarch64-linux-gnu-as -o kernelim/outs/kernelstarter.o kernelim/kernelstarter.s
aarch64-linux-gnu-as -o kernelim/outs/uart.o kernelim/outs/uart.s
aarch64-linux-gnu-as -o kernelim/outs/mmu.o kernelim/outs/mmu.s
aarch64-linux-gnu-as -o kernelim/outs/sprintf.o kernelim/outs/sprintf.s
aarch64-linux-gnu-as -o kernelim/outs/kernel.o kernelim/outs/kernel.s 
aarch64-linux-gnu-as -o kernelim/outs/processes.o kernelim/outs/processes.s
aarch64-linux-gnu-as -o kernelim/outs/exeptions.o kernelim/outs/exeptions.s
aarch64-linux-gnu-as -o kernelim/outs/cpuContext.o kernelim/outs/cpuContext.s
aarch64-linux-gnu-as -o kernelim/outs/syscalls.o kernelim/outs/syscalls.s
aarch64-linux-gnu-as -o kernelim/outs/memory.o kernelim/outs/memory.s
aarch64-linux-gnu-as -o kernelim/outs/heap.o kernelim/outs/heap.s
aarch64-linux-gnu-as -o kernelim/outs/filesys.o kernelim/outs/filesys.s
aarch64-linux-gnu-as -o kernelim/outs/fd.o kernelim/outs/fd.s


aarch64-linux-gnu-ld -T kernelim/kernel.ld -nostdlib -o kernelim/outs/kernel.elf kernelim/outs/kernelstarter.o kernelim/outs/uart.o kernelim/outs/kernel.o kernelim/outs/mmu.o kernelim/outs/sprintf.o kernelim/outs/processes.o kernelim/outs/exeptions.o UserApps/outs/testApp1.o UserApps/outs/Userapp2.o kernelim/outs/cpuContext.o kernelim/outs/syscalls.o kernelim/outs/memory.o kernelim/outs/heap.o kernelim/outs/filesys.o kernelim/outs/fd.o 
aarch64-linux-gnu-objcopy -O binary kernelim/outs/kernel.elf kernelim/outs/kernel.bin

#compile the bootloeader
aarch64-linux-gnu-gcc -S -O0 -o bootloader/outs/bootloader.s bootloader/bootloader.c -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd
aarch64-linux-gnu-gcc -S -O0 -o bootloader/outs/bootUart.s bootloader/bootUart.c -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd
aarch64-linux-gnu-gcc -S -O0 -o bootloader/outs/mmu.s bootloader/mmu.c -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd

aarch64-linux-gnu-as -o bootloader/outs/stackinit.o bootloader/stackinit.s
aarch64-linux-gnu-as -o bootloader/outs/bootloader.o bootloader/outs/bootloader.s
aarch64-linux-gnu-as -o bootloader/outs/bootUart.o bootloader/outs/bootUart.s
aarch64-linux-gnu-as -o bootloader/outs/mmu.o bootloader/outs/mmu.s

#aarch64-linux-gnu-ld -T bootloader/linker.ld -nostdlib -o bootloader/outs/BootLoaderWithKernel.elf bootloader/outs/bootloader.o bootloader/outs/stackinit.o bootloader/outs/mmu.o bootloader/outs/bootUart.o

aarch64-linux-gnu-objcopy -I binary -O elf64-littleaarch64 -B aarch64 --add-section .kernel_image=kernelim/outs/kernel.bin kernelim/outs/kernel.o

#link the kernel image to bootloeader section
#aarch64-linux-gnu-objcopy  -I binary -B aarch64 --add-section kernel_image=kernelim/outs/kernel.bin bootloader/outs/BootLoaderWithKernel.elf

aarch64-linux-gnu-ld -T bootloader/linker.ld -nostdlib -o bootloader/outs/BootLoaderWithKernel.elf bootloader/outs/bootloader.o bootloader/outs/stackinit.o bootloader/outs/mmu.o bootloader/outs/bootUart.o kernelim/outs/kernel.o

#startUp
qemu-system-aarch64 -m 256 -M virt -cpu cortex-a53 -serial stdio -kernel bootloader/outs/BootLoaderWithKernel.elf -S -s 