#include <cstdint>
#include <iostream>
#include <string>
#include <terminal.hpp>
#include <ui.hpp>
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
  ui::header("Menu Restoran");
  // ui::box(0, 3, 16, 10);
  // tui::puts(2, 3, " Kategori ");
  // ui::box(16, 3, 24, 10);
  // tui::puts(19, 3, " Menu ");
  // ui::box(40, 3, 25, 10);
  // tui::puts(43, 3, " Detail ");
  ui::View viewKategori(0, 3, 16, 10, " Kategori ");
  viewKategori.render();

  ui::ListProperty listProp;
  listProp.margin.top = 2;
  listProp.margin.left = 1;
  listProp.marker.glyph = ">";
  listProp.marker.gap = 2;
  listProp.marker.selectedElement = 0;

  for (int i = 0; i < productCategoryStr.size(); i++)
    ui::list(viewKategori, i, productCategoryStr.at(i).c_str(), listProp);

  ui::View viewMenu(16, 3, 24, 10, " Menu ");
  viewMenu.render();

  listProp.margin.left = 3;
  listProp.margin.top = 2;
  for (int i = 0; i < 3; i++)
    ui::list(viewMenu, i, productItems[i].nama.c_str(), listProp);

  ui::View viewDetail(40, 3, 25, 10, " Detail ");
  viewDetail.render();

  ui::footer("Status: N/A | Total: Rp 15.000");

  tui::render();
  tui::show();
  return 0;
}
