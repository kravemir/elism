# Elism

This is a custom language focused on development of **e**asy, but **li**ght and **s**afe **m**emory **m**anagement. Language is being implemented as part my Bachelors work.
 
It's still work-in-progress, and implementation is highly experimental.

## Usage (Ubuntu 16.04)

Add LLVM 3.9 repository:

```bash
wget -O - http://apt.llvm.org/llvm-snapshot.gpg.key|sudo apt-key add -
sudo add-apt-repository 'deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-3.9 main'
```

Install needed packages:
```bash
sudo apt update && sudo apt install -y gcc g++ git cmake clang-3.9 llvm-3.9-dev llvm-3.9-runtime re2c lemon zlib1g-dev
```

Clone repository, and enter the project directory:
```bash
git clone https://github.com/kravemir/elism.git && cd elism
```

Ubuntu 16.04 has old lemon, build and install own lemon localle:

```bash
bash scripts/install_local_lemon.sh
export PATH="${HOME}/.local/bin:$PATH"
```

Prepare, build and test:

```bash
mkdir -p build
sh -c 'cd build; cmake ..; make -j4'
sh scripts/tests/test_fine_errors.sh
```