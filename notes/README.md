# Menno's class notes

These are the notes I wanted to take during the workshop.

<!-- TOC -->
* [Menno's class notes](#mennos-class-notes)
  * [Setup](#setup)
    * [Installing VM](#installing-vm)
  * [1. Build a kernel](#1-build-a-kernel)
  * [2. Build a module](#2-build-a-module)
  * [3. Start kernel in GDB](#3-start-kernel-in-gdb)
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

* [ ] [Building (out of the tree) kernel modules](../README.md#building-out-of-the-tree-kernel-modules)
* [ ] [These are the bare-bones to start with building out of tree kernel modules:](../README.md#these-are-the-bare-bones-to-start-with-building-out-of-tree-kernel-modules)
    * [ ] [Suggestions to expand the skeleton module, as bonus exercises:](../README.md#suggestions-to-expand-the-skeleton-module-as-bonus-exercises)
* [ ] [Running the new kernel with QEMU.](../README.md#running-the-new-kernel-with-qemu)
    * [ ] [Preparing to run the kernel](../README.md#preparing-to-run-the-kernel)
* [ ] [Running the kernel with QEMU](../README.md#running-the-kernel-with-qemu)

## 3. Start kernel in GDB

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
