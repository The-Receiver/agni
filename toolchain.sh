#!/bin/sh
cd ~

set -e
set -x

# Thanks to bauen1 for lending me this script and letting me modify it to fit my own needs

BINUTILS_VERSION=2.30
GCC_VERSION=7.3.0
GRUB_VERSION=2.02
TINYCC_TAG=release_0_9_27
MUSL_VERSION=v1.1.19

mkdir -p toolchain
export PREFIX="$PWD/toolchain"

BINUTILS_VERSION=2.30
GCC_VERSION=8.1.0
TINYCC_TAG=release_0_9_27

export TARGET_ARCH=i386
export TARGET=$TARGET_ARCH-elf
export PATH="$PREFIX/bin:$PATH"
export JOBS=4

cd toolchain
mkdir -p src
cd src

if [ ! -f .downloaded_binutils ]; then
	echo "Downloading binutils-$BINUTILS_VERSION"
	wget ftp://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.xz
	wget ftp://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.xz.sig
	tar -xvf binutils-$BINUTILS_VERSION.tar.xz
	touch .downloaded_binutils
fi

if [ ! -f .downloaded_gcc ]; then
	echo "Downloading gcc-$GCC_VERSION"
	wget ftp://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.xz
	wget ftp://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.xz.sig
	tar -xvf gcc-$GCC_VERSION.tar.xz
	touch .downloaded_gcc
fi

if [ ! -f .downloaded_tinycc ]; then
	echo "Cloning tinycc"
	test -d tinycc || git clone "http://repo.or.cz/tinycc.git"
	git -C tinycc checkout $TINYCC_TAG
	touch .downloaded_tinycc
fi


if [ ! -f .built_binutils ]; then
	echo "Building Binutils"
	rm -rf binutils-build
	mkdir -p binutils-build
	(cd binutils-build
		../binutils-$BINUTILS_VERSION/configure \
			--target="$TARGET" \
			--prefix="$PREFIX" \
			--with-sysroot \
			--disable-nls \
			--disable-werror

		make -j$JOBS
		echo "Installing binutils"
		make -j$JOBS install
	)
	echo "Cleaning up"
	rm -rf binutils-build
	#"binutils-$BINUTILS_VERSION"
	touch .built_binutils
fi

if [ ! -f .built_gcc ]; then
	echo "Building gcc"
	rm -rf gcc-build
	mkdir -p gcc-build
	(cd gcc-build
		../gcc-$GCC_VERSION/configure \
			--target="$TARGET" \
			--prefix="$PREFIX" \
			--disable-nls \
			--enable-languages=c \
			--without-headers
		make -j$JOBS all-gcc all-target-libgcc
		make -j$JOBS install-gcc install-target-libgcc
	)

	echo "Cleaning up build directory"
	rm -rf gcc-build
	touch .built_gcc
fi


if [ ! -f .built_tinycc ]; then
	echo "Building Tiny C Compiler"
	rm -rf tinycc-build
	mkdir -p tinycc-build
	(cd tinycc-build
		../tinycc/configure \
			--prefix="$PREFIX/opt" \
			--strip-binaries \
			--sysroot='$PREFIX/sysroot' \
			--cpu="$TARGET_ARCH" \
			--triplet="$TARGET" \
		#	--config-musl
		#	--sysincludepaths='' \
		#	--libpaths='' \
		#	--crtprefix=''
		make -j$JOBS cross-i386
		# cross-x86_64 cross-arm cross-arm64
		make -j$JOBS install
	)

	echo "Cleaning up build directory"
	rm -rf tinycc-build
	touch .built_tinycc
fi
