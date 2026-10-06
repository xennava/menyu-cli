#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <terminal.hpp>
#include <ui.hpp>
#include <unistd.h>
#include <vector>

enum class ProductCategory : uint8_t { NA = 0, Makanan, Minuman, Dessert };

struct MenuItem {
  std::string nama;
  std::string description;
  uint32_t price;
  uint16_t id;
  uint8_t category;
};

struct OrderItem {
  uint16_t menuId;
  uint16_t quantity;
};

struct Order {
  std::vector<OrderItem> items;
};

std::vector<std::string> productCategoryStr = {"Makanan", "Minuman", "Dessert"};

std::vector<MenuItem> productItems = {
    {"Mi Ayam",
     "Mi dengan ayam suwir, telur, sayuran, dan bumbu berkuah hangat.", 6000, 0,
     static_cast<uint8_t>(ProductCategory::Makanan)},
    {"Bakso", "Bakso daging sapi dan telur di dalamnya.", 16000, 1,
     static_cast<uint8_t>(ProductCategory::Makanan)},
    {"Nasi Goreng", "Nasi goreng dengan telur dan ayam suwir.", 12000, 2,
     static_cast<uint8_t>(ProductCategory::Makanan)},
    {"Teh Manis", "Teh hangat atau es yang manis dan segar.", 2500, 1000,
     static_cast<uint8_t>(ProductCategory::Minuman)},
    {"Jeruk Manis", "Jeruk hangat atau es tidak kecut yang manis dan segar.",
     3000, 1001, static_cast<uint8_t>(ProductCategory::Minuman)},
    {"Kopi ABC", "Kopi panas sachet ABC", 3000, 1002,
     static_cast<uint8_t>(ProductCategory::Minuman)},
    {"Pisang Goreng", "Tidak lembek tidak keras, pas, 1 porsi 7 potong pisang.",
     5000, 3000, static_cast<uint8_t>(ProductCategory::Dessert)},
    {"Puding Coklat", "Puding kenyal dan manis.", 6500, 3001,
     static_cast<uint8_t>(ProductCategory::Dessert)},
    {"Es Krim", "Es Krim dengan varian coklat, vanilla, durian, dll.", 3000,
     3002, static_cast<uint8_t>(ProductCategory::Dessert)}};

int main(int argc, char *argv[]) {
  // for (auto &item : productItems) {
  //   if (item.id >= 1000)
  //     continue;
  //   printf("%s\n", item.nama.c_str());
  // }

  if (tui::init()) {
    std::cerr << "error init\n";
  }

  ui::View viewKategori(0, 3, 16, 10, " Kategori ");
  ui::View viewMenu(16, 3, 24, 10, " Menu ");
  ui::View viewDetail(40, 3, 25, 10, " Detail ");

  ui::ListProperty listProp;

  int ch;
  bool isRunning = true;
  while (isRunning) {
    tui::clearScreen();

    ch = tui::getch();
    if (ch != -1) {
      if (ch == 'q') {
        isRunning = false;
      }
      if (ch == '\t') {
        ui::view::stepActiveView(1);
      }
    }

    ui::header("Menu Restoran");
    viewKategori.render();

    listProp.margin.top = 2;
    listProp.margin.left = 1;
    listProp.marker.glyph = ">";
    listProp.marker.gap = 2;
    listProp.marker.selectedElement = 0;

    for (int i = 0; i < productCategoryStr.size(); i++)
      ui::list(viewKategori, i, productCategoryStr.at(i).c_str(), listProp);

    viewMenu.render();

    listProp.margin.left = 3;
    listProp.margin.top = 2;
    listProp.marker.gap = 3;
    for (int i = 0; i < 3; i++)
      ui::list(viewMenu, i, productItems[i].nama.c_str(), listProp);

    viewDetail.render();

    ui::footer("Status: N/A | Total: Rp 15.000");

    tui::render();
    tui::show();
  }
  for (auto &c : ui::view::viewRef) {
    printf("#%d\n", c.second->getID());
  }
  tui::end();
  return 0;
}
