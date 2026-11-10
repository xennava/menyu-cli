#include "app.hpp"
#include "core/input.hpp"
#include "core/terminal.hpp"
#include "core/type.hpp"
#include "input.hpp"
#include "item.hpp"
#include "order.hpp"
#include "ui.hpp"
#include <array>
#include <cstdint>
#include <pages.hpp>
#include <string>
#include <string_view>
#include <vector>

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

ui::ListProperty listProp;

ui::View viewKategori(0, 3, 16, 10, " Kategori ");
ui::View viewMenu(16, 3, 24, 10, " Menu ");
ui::View viewDetail(40, 3, 25, 10, " Detail ");
ui::Text detailText(viewDetail);

ui::View orderView(0, 0, tui::screenExtent.width - 1, tui::screenExtent.height);

void mainMenuInit() {
  viewKategori.init();
  viewMenu.init();
  viewDetail.init();
}

void mainMenuInput() {
  if (tui::Input.getBaseInput() == 'x') {
    app::orderPage = true;
  }
  int bipolar = appInput.getKey().ArrowRight - appInput.getKey().ArrowLeft;
  ui::view::stepActiveView(bipolar);

  bipolar = appInput.getKey().plus - appInput.getKey().minus +
            appInput.getKey().ArrowDown - appInput.getKey().ArrowUp;
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
  if (viewDetail.state.activeView && bipolar != 0) {
    setQuantity(bipolar);
  }
  if (viewDetail.state.activeView) {
    if (appInput.getKey().enter &&
        order.getOrderStatus() == OrderStatus::DRAFT) {
      order.upPriceDiff();
    }
  }
}

void mainMenuPage() {
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

  MenuItem &currentItem = getCurrentItem();
  currentItem =
      *(searchItemByCategory(categorySelectedIndex) + menuSelectedIndex);

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
      .add(std::to_string(getQuantity()))
      .add(" [+]")
      .center()
      .setPadding({5, 0, 0, 0})
      .render();

  if (order.getOrderStatus() == OrderStatus::DRAFT) {
    detailText.setOriginPos(viewDetail.getContentOrigin())
        .assign("Tambah Pesanan [Enter]")
        .center()
        .setPadding({7, 0, 0, 0})
        .render();
  }
  viewDetail.render();

  std::string footer("Status: ");
  footer += std::to_string(order.items.size());
  footer += " Item | Total: Rp.";
  footer += std::to_string(order.getTotalPrice());
  footer += "  Lihat Pesanan [x]";

  ui::footer(footer);
  tui::puts(0, 16, "Checkout [x] ");
  tui::puts("Keluar [q]");

  tui::puts(0, 17, "←");
  tui::puts(4, 17, "→");
}

void orderInput() {
  if (tui::Input.getKeys()['x']) {
    app::orderPage = false;
  }
  if (appInput.getKey().w) {
    order.makeOrder();
  }
  order.simulatedInput();
}

std::string topOrderMessage[2] = {
    "(Mohon diingat...)",
    "Diingat-ingat ya..",
};
std::array<std::string_view, 6> orderStatusses = {
    "Menunggu pesanan",   "Pesanan diserahkan", "Pesanan sedang diproses",
    "Pesanan sudah siap", "Pesanan selesai",    "Pesanan dibatalkan"};

std::array<std::string_view, 2> subcaption = {
    "Bayar secara online",
    "Silakan ambil pesanan dan bayar di kasir atau online",
};
void updateOrderMessage() {
  updateOrderWarnMessage();
  std::string_view dots = "....";

  static int elapsed = 0;
  static int dotsElapsed = 0;
  static int8_t idx = 0;
  static int8_t didx = 1;

  elapsed += app::time::targetFrameTime;
  dotsElapsed += app::time::targetFrameTime;

  if (elapsed >= 17e5) {
    elapsed = 0;
    idx++;
    if (idx == 2)
      idx = 0;
  }
  if (dotsElapsed >= 25e4) {
    dotsElapsed = 0;
    didx++;
    if (didx == dots.size() + 1) {
      didx = 1;
    }
  }

  std::string_view s(topOrderMessage[idx]);

  tui::puts(orderView.getContentOrigin().x, orderView.getContentOrigin().y,
            "Nomor Pesanan: ");
  tui::puts(std::to_string(order.getId()));
  tui::puts(" ");
  tui::puts(s);

  int8_t status = order.getOrderStatusIndex() - 1;
  if (status > 5)
    status = 5;
  if (status < 0)
    status = 0;
  tui::puts(0, 16, "> ");
  tui::puts(orderStatusses[status]);
  tui::puts(dots.substr(0, didx));

  tui::moveCursor(0, 17);
  if (order.getOrderStatusIndex() > 1 &&
      order.getOrderStatusIndex() <= static_cast<int>(OrderStatus::READY)) {
    if (order.getOrderStatus() == OrderStatus::READY) {
      tui::puts(" > ");
      auto &str = subcaption.at(1);
      auto pos = str.find("atau");

      tui::puts(pos != std::string::npos ? str.substr(0, pos) : str);
    }
    // else {
    //   tui::puts("> ");
    //   tui::puts(subcaption.at(0));
    // }
    // tui::puts(" [r]");
  }
}

void orderPage() {
  updateOrderMessage();

  auto searchItem = [&](int i) {
    for (auto &v : productItems)
      if (v.id == order.items[i].menuId)
        return v;
    return MenuItem();
  };

  listProp.margin.left = 3;
  listProp.margin.top = 2;
  listProp.marker.gap = 3;
  listProp.maxElement = order.items.size();
  listProp.marker.selectedElement = 0;
  int i = 0;

  std::string orderList("");
  for (const auto &[key, orderItem] : order.items) {
    auto item = searchItem(key);

    const uint8_t gapLen = 24;
    orderList.assign(item.nama);

    int sisaRuang = gapLen - orderList.size();
    orderList.append(sisaRuang, ' ');

    orderList += "Rp." + std::to_string(item.price);
    std::string c = "x" + std::to_string(orderItem.quantity);
    orderList.append(orderView.getContentExtent().width - c.size() -
                         orderList.size() - 6,
                     ' ');
    orderList += c;

    ui::list(orderView, i, orderList, listProp);
    ++i;
  }

  orderView.render();

  {
    auto bottom = [&](int x, std::string_view s) {
      tui::puts(orderView.getContentOrigin().x + x,
                orderView.getContentExtent().height - 1, s);
    };
    std::string a = std::to_string(order.items.size()) + " | Rp." +
                    std::to_string(order.getTotalPrice());

    bottom(0, "Kembali [x]");
    bottom((orderView.getContentExtent().width - a.size()) >> 1, a.c_str());

    OrderStatus ostat = order.getOrderStatus();

    std::string s = "";
    if (ostat == OrderStatus::DRAFT)
      s = "Pesan!";
    if (ostat == OrderStatus::SUBMITTED)
      s = "Edit Pesanan?";
    if (ostat == OrderStatus::COMPLETED)
      s = "Pesan Lagi?";

    if (s.size() > 0) {
      s += " [w]";
      bottom(orderView.getContentExtent().width - s.size() - 1, s.c_str());
    }
  }
}

// [Pages Space]
void pagesInput() {
  if (app::orderPage)
    orderInput();
  else
    mainMenuInput();
}

void pagesUpdate() {
  order.updateOrder();

  if (app::orderPage) {
    orderPage();
  } else {
    mainMenuPage();
  }
}

void pagesInit() {
  order.init();
  mainMenuInit();
  orderView.init();
}
