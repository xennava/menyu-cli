
#include "core/terminal.hpp"
#include "input.hpp"
#include "ui.hpp"
#include <mainMenu.hpp>

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

int categorySelectedIndex = 0;
int menuSelectedIndex = 0;
int qty = 0;
MenuItem currentItem = {.category = 0};

ui::ListProperty listProp;

ui::View viewKategori(0, 3, 16, 10, " Kategori ");
ui::View viewMenu(16, 3, 24, 10, " Menu ");
ui::View viewDetail(40, 3, 25, 10, " Detail ");
ui::Text detailText(viewDetail);

void mainMenuInit() {
  viewKategori.init();
  viewMenu.init();
  viewDetail.init();
}

Order order;

void mainMenuInput() {
  if (appInput.getKey().e) {
    ui::view::stepActiveView(1);
  }
  if (appInput.getKey().enter)
    tui::puts(0, 17, "Enter");
  int bipolar = appInput.getKey().ArrowRight - appInput.getKey().ArrowLeft;
  ui::view::stepActiveView(bipolar);

  bipolar = appInput.getKey().ArrowDown - appInput.getKey().ArrowUp;
  if (viewKategori.state.activeView) {
    categorySelectedIndex += bipolar;
    if (categorySelectedIndex < 0)
      categorySelectedIndex = 0;
    if (categorySelectedIndex > 2)
      categorySelectedIndex = 2;
    menuSelectedIndex = 0;
  }
  if (viewMenu.state.activeView) {
    menuSelectedIndex += bipolar;
    if (menuSelectedIndex < 0)
      menuSelectedIndex = 0;
    if (menuSelectedIndex > 2)
      menuSelectedIndex = 2;
  }
  bipolar = appInput.getKey().plus - appInput.getKey().minus;
  if (viewDetail.state.activeView && bipolar != 0) {
    qty += bipolar;
    if (qty < 1)
      qty = 1;
  }
  if (viewDetail.state.activeView) {
    if (appInput.getKey().enter && currentItem.category != 0) {

      uint16_t menuId = currentItem.id;
      order.items.push_back(
          {.menuId = menuId, .quantity = static_cast<uint16_t>(qty)});
      order.totalPrice += currentItem.price * qty;
    }
  }
}

void mainMenu() {
  ui::header("Menu Restoran");
  viewKategori.render();

  listProp.margin.top = 2;
  listProp.margin.left = 1;
  listProp.marker.glyph = ">";
  listProp.marker.gap = 2;
  listProp.maxElement = productCategoryStr.size();

  listProp.marker.selectedElement = categorySelectedIndex;
  for (int i = 0; i < productCategoryStr.size(); i++)
    ui::list(viewKategori, i, productCategoryStr.at(i).c_str(), listProp);

  viewMenu.render();

  listProp.margin.left = 3;
  listProp.margin.top = 2;
  listProp.marker.gap = 3;
  listProp.maxElement = 3;
  listProp.marker.selectedElement = menuSelectedIndex;

  for (int i = 0; i < 3; i++)
    ui::list(viewMenu, i,
             productItems[i + categorySelectedIndex * 3].nama.c_str(),
             listProp);

  currentItem = productItems[categorySelectedIndex * 3 + menuSelectedIndex];

  detailText.setOriginPos(viewDetail.getContentOrigin())
      .assign(currentItem.nama.c_str())
      .setPadding({1, 0, 0, 0})
      .center()
      .render();

  ui::lineSeparator(detailText.getOriginPos(), detailText.getExtent(),
                    viewDetail.getContentExtent().width);

  detailText.setOriginPos(viewDetail.getContentOrigin())
      .assign("Rp.")
      .add(std::to_string(currentItem.price))
      .setPadding({3, 0, 0, 0})
      .center()
      .render();

  detailText.setOriginPos(viewDetail.getContentOrigin())
      .assign("Jumlah: [-] ")
      .add(std::to_string(qty))
      .add(" [+]")
      .center()
      .setPadding({5, 0, 0, 0})
      .render();

  detailText.setOriginPos(viewDetail.getContentOrigin())
      .assign("Tambah Pesanan [Enter]")
      .center()
      .setPadding({7, 0, 0, 0})
      .render();
  viewDetail.render();

  std::string footer("Status: ");
  footer += std::to_string(order.items.size());
  footer += " Item | Total: Rp.";
  footer += std::to_string(order.totalPrice);
  footer += "  Lihat Pesanan [s]";

  ui::footer(footer);

  tui::puts(0, 17, "←");
  tui::puts(4, 17, "→");
}
