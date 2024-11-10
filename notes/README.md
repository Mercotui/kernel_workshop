# Menno's class notes

These are the notes I wanted to take during the workshop.

<!-- TOC -->

* [Menno's class notes](#mennos-class-notes)
    * [Setup](#setup)
        * [Installing VM](#installing-vm)
    * [1. Build a kernel](#1-build-a-kernel)
    * [2. Build a module](#2-build-a-module)
    * [3. Start kernel in debugger](#3-start-kernel-in-debugger)
    * [4. Patch the kernel](#4-patch-the-kernel)
    * [5. Add a new syscall](#5-add-a-new-syscall)
    * [6, 7, 8, 9. Add page-cache hit/miss counters to the Linux kernel](#6-7-8-9-add-page-cache-hitmiss-counters-to-the-linux-kernel)

<!-- TOC -->

## Setup

* [x] [Workshop Goals](../README.md#workshop-goals)
* [x] [Workshop Description](../README.md#workshop-description)
* [x] [Configure your Virtual Machine (VM)](../README.md#configure-your-virtual-machine-vm)
* [x] [Update the kernel and download some useful packages](../README.md#update-the-kernel-and-download-some-useful-packages)

### Installing VM

It's probably wise to avoid EUFI and secure boot.

To get gnome boxes to work with proper file sharing and 3D acceleration,
I installed the whole virtualization group:

```bash
sudo yum group install --with-optional virtualization
```

In the guest os make sure the spice tools are installed:

```bash
$ yum list --installed | grep spice
...
spice-vdagent.x86_64
spice-webdavd.x86_64
```

Then the shared folders should be accessible in:

```
dav://localhost:9843
```

Additionally on Fedora 39, install `zstd`.

## 1. Build a kernel

After running the setup, the fresh upgraded fedora 39 installation has the following kernel:

```bash
$ uname -srm
Linux 6.7.7-200.fc39.x86_64 x86_64
```

The first compilation attempt actually failed after 20 minutes due to missing command `zstd`.

After building and installing the kernel, I see:

```bash
$ uname -rms
Linux 6.8.0-rc7+ x86_64
```

And after adding a `.0` to the `EXTRAVERSION` string, recompiling, etc,
I see:

```bash
$ uname -rms
Linux 6.8.0-rc7.0+ x86_64
```

* [x] [Build a vanilla upstream kernel](../README.md#build-a-vanilla-upstream-kernel)
    * [x] [Cloning (downloading) the upstream kernel tree from GitHub.com](../README.md#cloning-downloading-the-upstream-kernel-tree-from-githubcom)
* [x] [Configuring the kernel](../README.md#configuring-the-kernel)
* [x] [Building the kernel](../README.md#building-the-kernel)
* [x] [Installing the new kernel on your machine](../README.md#installing-the-new-kernel-on-your-machine)
* [x] [Change the EXTRAVERSION string in the kernel Makefile.](../README.md#change-the-extraversion-string-in-the-kernel-makefile)

## 2. Build a module

While building the dummy-kmod, I ran into the error:

```
make[1]: *** /lib/modules/6.8.0-rc7.0+build: No such file or directory.  Stop.
```

I thought this was due to the custom kernel version that we installed in step 1.
So I tried to switch back to the default fedora kernel and reboot:

```bash
$ sudo grubby --info=ALL
...
index=2
kernel="/boot/vmlinuz-6.7.7-200.fc39.x86_64"
...
$ sudo grubby --set-default-index=2
```

But then I realized the make file is simply missing a `/` on line 2:

```diff
diff --git a/dummy-kmod/Makefile b/dummy-kmod/Makefile
index 343636a..0be24f1 100644
--- a/dummy-kmod/Makefile
+++ b/dummy-kmod/Makefile
@@ -1,5 +1,5 @@
 obj-m += hello.o 
-KDIR:=/lib/modules/$(shell uname -r)build
+KDIR:=/lib/modules/$(shell uname -r)/build
 all:
        $(MAKE) -C $(KDIR) M=$(PWD) modules
```

Spice/webdav has messed up permission bits, so I copied the dummy-kmod to a local folder inside the vm:

```bash
cp -r kernel_workshop/dummy-kmod ./
sudo chmod -R 644 ./dummy-kmod
```

Then I could finally build the module:

```bash
$ make
make -C /lib/modules/6.8.0-rc7.0+/build M=/home/menno/Workspace/dummy-kmod modules
make[1]: Entering directory '/home/menno/Workspace/linux/build'
  CC [M]  /home/menno/Workspace/dummy-kmod/hello.o
  MODPOST /home/menno/Workspace/dummy-kmod/Module.symvers
  CC [M]  /home/menno/Workspace/dummy-kmod/hello.mod.o
  LD [M]  /home/menno/Workspace/dummy-kmod/hello.ko
  BTF [M] /home/menno/Workspace/dummy-kmod/hello.ko
make[1]: Leaving directory '/home/menno/Workspace/linux/build'
$ sudo insmod hello.ko
$ sudo dmesg
...
[ 2518.680182] hello: loading out-of-tree module taints kernel.
[ 2518.680187] hello: module verification failed: signature and/or required key missing - tainting kernel
[ 2518.680879] Hello, kernel world!
$ sudo rmmod hello.ko
$ sudo dmesg
...
[ 2975.023056] Goodbye, kernel world!
```

For bonus exercise #1 I made a kmod called [echo](../dummy-kmod/echo.c), and I implemented some features to print
variables using a template. I did not realize that the variables in bonus exercise #2 are preprocessor defines,
that can not be accessed by name at runtime. The kernel does have a list of (debug)symbols that you can access,
but to the best of my knowledge the preprocessor defines are lost/expanded during compilation.
I solved this by making a lookup function that converts returns the define values `get_variable_value()`.

Then I can run and compile echo like follows:

```bash
$ cp ../kernel_workshop/dummy-kmod/echo.c ./ && make
$ sudo insmod echo.ko template='"hey this is the HZ ${HZ} and the user HZ ${USER_HZ}"'
$ sudo rmmod echo.ko && sudo dmesg -c
[43659.632454] hey this is the HZ 1000 and the user HZ 100
[43660.897518] Goodbye, from echo!
```

* [x] [Building (out of the tree) kernel modules](../README.md#building-out-of-the-tree-kernel-modules)
* [x] [These are the bare-bones to start with building out of tree kernel modules:](../README.md#these-are-the-bare-bones-to-start-with-building-out-of-tree-kernel-modules)
    * [x] [Suggestions to expand the skeleton module, as bonus exercises:](../README.md#suggestions-to-expand-the-skeleton-module-as-bonus-exercises)

//TODO(Menno 2024.11.10) Saving the following tabs for further reference:

- https://www.kernel.org/doc/html/v6.8-rc7/
- https://tldp.org/LDP/lkmpg/2.6/html/x323.html
- https://github.com/torvalds/linux/blob/master/include/linux/moduleparam.h
- https://www.kernel.org/doc/html/v4.10/dev-tools/index.html
- https://lwn.net/Articles/948408/
- https://github.com/torvalds/linux/blob/master/include/linux/seq_buf.h
- https://github.com/torvalds/linux/blob/master/lib/seq_buf.c#L93
- https://docs.kernel.org/doc-guide/kernel-doc.html
- https://archive.kernel.org/oldlinux/htmldocs/kernel-api/libc.html#id-1.4.3
- https://return42.github.io/linuxdoc/kernel-doc-intro.html#kernel-doc-intro-example-out

## 3. Start kernel in debugger

* [ ] [Running the new kernel with QEMU.](../README.md#running-the-new-kernel-with-qemu)
    * [ ] [Preparing to run the kernel](../README.md#preparing-to-run-the-kernel)
* [ ] [Running the kernel with QEMU](../README.md#running-the-kernel-with-qemu)
* [ ] [Debugging the kernel with GDB](../README.md#debugging-the-kernel-with-gdb)
    * [ ] [Changing the QEMU command](../README.md#changing-the-qemu-command)
    * [ ] [Starting GDB](../README.md#starting-gdb)
* [ ] [Stop the kernel and find the corresponding source](../README.md#stop-the-kernel-and-find-the-corresponding-source)

## 4. Patch the kernel

* [ ] [Kernel Patching](../README.md#kernel-patching)
    * [ ] [Creating a kernel patch](../README.md#creating-a-kernel-patch)
    * [ ] [Applying and removing a patch to the unchanged kernel.](../README.md#applying-and-removing-a-patch-to-the-unchanged-kernel)

## 5. Add a new syscall

* [ ] [Defining a new System Call and creating a patch to distribute it.](../README.md#defining-a-new-system-call-and-creating-a-patch-to-distribute-it)
    * [ ] [Implementation](../README.md#implementation)
    * [ ] [Rebuilding the kernel to add the new system call.](../README.md#rebuilding-the-kernel-to-add-the-new-system-call)

## 6, 7, 8, 9. Add page-cache hit/miss counters to the Linux kernel

* [ ] [Adding page-cache hit/miss counters to the kernel](../README.md#adding-page-cache-hitmiss-counters-to-the-kernel)
* [ ] [Implementation](../README.md#implementation-1)
