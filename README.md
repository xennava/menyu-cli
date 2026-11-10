# Pemesanan Menu Restoran

<!--toc:start-->
- [Pemesanan Menu Restoran](#pemesanan-menu-restoran)
  - [Features](#features)
  - [Requirements](#requirements)
    - [Development](#development)
    - [Runtime](#runtime)
  - [Build n Run](#build-n-run)
    - [Linux and MacOS](#linux-and-macos)
    - [Windows](#windows)
  - [Project Structure](#project-structure)
  - [Menu Overview](#menu-overview)
  - [Order Overview](#order-overview)
  - [TODO](#todo)
  - [Known Limitations](#known-limitations)
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

## Requirements

### Development

- CMake minimal 3.30
- GCC 11+ atau Clang 11+
- Ninja Build Generator
- C++20 atau lebih baru
- Editor C++

### Runtime

- Minimal Terminal Size: 64 columns × 18 rows

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
┌───────────────────────────────────────────────────────────────┐
│                         Menu Restoran                         │
└───────────────────────────────────────────────────────────────┘
┌ Kategori ────┐┌ Menu ────────────────┐┌ Detail ───────────────┐
│              ││                      ││                       │
│              ││                      ││        Mi Ayam        │
│ >  Makanan   ││   Mi Ayam            ││───────────────────────│
│    Minuman   ││   Bakso              ││        Rp.6000        │
│    Dessert   ││   Nasi Goreng        ││                       │
│              ││                      ││   Jumlah: [-] 1 [+]   │
│              ││                      ││                       │
│              ││                      ││ Tambah Pesanan [Enter]│
└──────────────┘└──────────────────────┘└───────────────────────┘
┌───────────────────────────────────────────────────────────────┐
│        Status: 0 Item | Total: Rp.0  Lihat Pesanan [x]        │
└───────────────────────────────────────────────────────────────┘
Checkout [x] Keluar [q]                                          
← →                                                              
```

## Order Overview

```text
┌──────────────────────────────────────────────────────────────┐ 
│Nomor Pesanan: 0 (Mohon diingat...)                           │ 
│                                                              │ 
│   Puding Coklat           Rp.6500                        x1  │ 
│   Teh Manis               Rp.2500                        x1  │ 
│   Mi Ayam                 Rp.6000                        x1  │ 
│                                                              │ 
│                                                              │ 
│                                                              │ 
│                                                              │ 
│                                                              │ 
│                                                              │ 
│                                                              │ 
│                                                              │ 
│Kembali [x]              3 | Rp.15000               Pesan! [w]│ 
└──────────────────────────────────────────────────────────────┘ 
> Menunggu pesanan..                                             

```

## TODO

- Bikin fungsi penyimpan struk dalam bentuk file data mentah dan
visualnya dalam bentuk text atau gambar.
- Tambahkan warna untuk kejelasan.
- Tambahin fitur settings.
- support multiple pages and multiple orders.

## Known Limitations

- **Single Order Only**: The application currently supports only one active order per session.
- Single Pages Only.

## Notes

Program dikembangkan menggunakan C++ dan CMake dengan target platform Linux,
macOS, dan Windows.

<!-- Dependency terminal UI digunakan secara khusus berdasarkan
platform. Linux dan macOS menggunakan `ncurses`, sedangkan
Windows menggunakan PDCurses. -->
