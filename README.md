# Menu Pemesanan Restoran

<!--toc:start-->
- [Menu Pemesanan Restoran](#menu-pemesanan-restoran)
  - [Features](#features)
  - [Requirements](#requirements)
    - [Development](#development)
    - [Runtime](#runtime)
  - [Build n Run](#build-n-run)
    - [Linux and MacOS](#linux-and-macos)
    - [Windows](#windows)
  - [Project Structure](#project-structure)
  - [Menu Overview](#menu-overview)
  - [Notes](#notes)
<!--toc:end-->

Program sederhana untuk mensimulasikan proses pemesanan makanan
dan minuman pada restoran melalui antarmuka berbasis terminal.

## Features

- Menampilkan daftar menu restoran
- Memilih makanan atau minuman
- Menentukan jumlah pesanan
- Menampilkan daftar pesanan
- Menghitung subtotal dan total pembayaran
- Validasi input pengguna

## Requirements

### Development

- CMake minimal 3.30
- GCC 11+ atau Clang 11+
- Ninja Build Generator
- C++20 atau lebih baru
- Editor C++

### Runtime

- Minimal Terminal Size: 64 columns × 16 rows

## Build n Run

### Linux and MacOS

```bash
cmake -B build -S . -G Ninja
cmake --build build
./build/menu
```

### Windows

```powershell
cmake -B build -S . -G Ninja
cmake --build build
.\build\menu.exe
```

## Project Structure

```text
.
├── CMakeLists.txt
├── include/
├── src/
└── third_party/
```

## Menu Overview

```text
┌──────────────────────────────────────────────────────────────┐
│                 MENU PEMESANAN RESTORAN                      │
├──────────────┬───────────────────────┬───────────────────────┤
│              │                       │                       │
│  KATEGORI    │       MENU            │      DETAIL           │
│              │                       │                       │
│ > Makanan    │  Nasi Goreng          │  Nasi Goreng          │
│   Minuman    │  Ayam Geprek          │  ─────────────        │
│   Dessert    │  Mie Goreng           │  Rp15.000             │
│              │                       │                       │
│              │                       │  Jumlah: [-] 1 [+]    │
│              │                       │                       │
│              │                       │  [ Tambah Pesanan ]   │
├──────────────┴───────────────────────┴───────────────────────┤
│ Status: 1 item | Total: Rp15.000           [ Lihat Pesanan ] │
└──────────────────────────────────────────────────────────────┘
```

## Notes

Program dikembangkan menggunakan C++ dan CMake dengan target platform Linux,
macOS, dan Windows.

<!-- Dependency terminal UI digunakan secara khusus berdasarkan
platform. Linux dan macOS menggunakan `ncurses`, sedangkan
Windows menggunakan PDCurses. -->
