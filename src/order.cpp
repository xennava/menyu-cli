#include "order.hpp"
#include "app.hpp"
#include "core/terminal.hpp"
#include "item.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

Payment payment;
Order order;

MenuItem currentItem = {
    .nama = "", .description = "", .price = 0, .id = 0, .category = 0};
int qty = 1;

std::unordered_set<int> unavailableId;
int maxId = 22;

int getLeastId() {
  for (int i = 0; i < maxId; i++) {
    if (unavailableId.contains(i))
      continue;
    unavailableId.emplace(i);
    return i;
  }
  return -1;
}

int upsertOrder(const uint16_t &currentMenuId, int qty) {
  auto it = order.items.find(currentMenuId);

  if (it != order.items.end()) {
    const int bqty = it->second.quantity;
    it->second.quantity = qty;
    if (qty == 0) {
      order.items.erase(it);
    }
    return qty - bqty;
  } else {
    if (qty > 0)
      order.items[currentMenuId] =
          OrderItem{currentMenuId, static_cast<uint16_t>(qty)};
    return qty;
  }
  return 0;
}

void Order::updatePrice() {
  if (!initialized) {
    auto searchItemById = [&](int id) {
      for (auto &v : productItems) {
        if (v.id == id) {
          return v;
        }
      }
      return MenuItem{"", "", 0, 0, 0};
    };

    for (auto &[key, v] : order.items) {
      totalPrice += v.quantity * searchItemById(key).price;
    }
    return;
  }

  if (!diff)
    return;

  if (currentItem.category != 0)
    totalPrice += currentItem.price * upsertOrder(currentItem.id, qty);

  diff = false;
}

void setQuantity(int8_t bipolar) {
  qty -= bipolar;
  if (qty < 0)
    qty = 0;
}

const int &getQuantity() { return qty; }

MenuItem &getCurrentItem() { return currentItem; }

void Order::init() {
  readOrder();

  if (orderStatus == OrderStatus::SUBMITTED) {
    updatePrice();
    initialized = true;
    return;
  }

  id = getLeastId();
  if (id == -1) {
    cantOrder = true;
    // orderStatus = OrderStatus::CANCELLED;
  }
  initialized = true;
}

uint64_t orderWarnMessageDuration = 0;
bool orderWarnMessageTrigger = false;
std::vector<std::string> pesanManis = {"Belum pesan? Pesan dulu...",
                                       "Pesen dulu..", "Pesen gak!?",
                                       "Udah dipesan, ditunggu ya.."};

void updateOrderWarnMessage() {
  if (!orderWarnMessageTrigger)
    return;

  orderWarnMessageDuration += app::time::targetFrameTime;

  std::string cstr;
  static std::size_t i = 0;
  if (order.isOrdered()) {
    i = pesanManis.size() - 1;
    cstr.assign(pesanManis.at(i));
  }

  if (orderWarnMessageDuration < 1e6) {
    tui::puts((tui::screenExtent.width - pesanManis[i].size()) >> 1,
              (tui::screenExtent.height - 2) >> 1, pesanManis[i].c_str());
  } else {
    orderWarnMessageTrigger = false;
    orderWarnMessageDuration = 0;
    i++;
    if (i == pesanManis.size() - 1)
      i--;
  }
}

void Order::makeOrder() {
  if (order.items.size() < 1 || ordered == true) {
    if (orderWarnMessageDuration < 1) {
      orderWarnMessageTrigger = true;
    }
    return;
  }

  std::ofstream file((std::filesystem::temp_directory_path() / "fjkd").string(),
                     std::ios::out | std::ios::binary);
  if (!file)
    return;

  size_t n = order.items.size();

  file.write(reinterpret_cast<char *>(&order.id), sizeof(order.id));
  file.write(reinterpret_cast<const char *>(&n), sizeof(n));

  for (const auto &[key, v] : order.items) {
    file.write(reinterpret_cast<const char *>(&v), sizeof(v));
  }

  file.close();

  orderStatus = OrderStatus::SUBMITTED;
  ordered = true;
}

void Order::readOrder() {
  std::ifstream file((std::filesystem::temp_directory_path() / "fjkd").string(),
                     std::ios::binary);
  if (!file)
    return;

  file.read(reinterpret_cast<char *>(&order.id), sizeof(order.id));

  if (!file)
    return;

  size_t n = 0;
  file.read(reinterpret_cast<char *>(&n), sizeof n);

  if (!file)
    return;

  for (std::size_t i = 0; i < n; i++) {
    OrderItem v{};

    file.read(reinterpret_cast<char *>(&v), sizeof(OrderItem));

    if (!file)
      return;

    order.items.emplace(v.menuId, v);
  }

  file.close();
  orderStatus = OrderStatus::SUBMITTED;
}

void Order::resetState() {
  if (unavailableId.contains(id)) {
    unavailableId.erase(id);
  }
  id = getLeastId();
  totalPrice = 0;
}

void Order::rmOrder() {
  if (orderStatus != OrderStatus::COMPLETED)
    return;

  try {
    std::filesystem::remove(std::filesystem::temp_directory_path() / "fjkd");
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "err fs: rm order" << '\n';
    return;
  }

  items.clear();
  resetState();

  orderStatus = OrderStatus::DRAFT;
}
