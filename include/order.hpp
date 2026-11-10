#pragma once

#include "core/input.hpp"
#include "core/net.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>

enum class ProductCategory : uint8_t { NA = 0, Makanan, Minuman, Dessert };

enum class PaymentStatus : uint8_t { UNPAID, PENDING, PAID, FAILED, REFUNDED };
class Payment {
private:
  PaymentStatus paymentStatus = PaymentStatus::UNPAID;

  void sync() {} // Just interface? idk

public:
  PaymentStatus getPaymentStatus() { return paymentStatus; }
  void update() { sync(); }
};

extern Payment payment;

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

enum class OrderStatus : uint8_t {
  INVALID = 0,
  DRAFT,
  SUBMITTED,
  PROCESSING,
  READY,
  COMPLETED,
  CANCELLED,
  LEN,
};

class Order {
private:
  int totalPrice = 0;
  int16_t id = 0;
  bool diff = false;
  bool ordered = false;
  bool cantOrder = false;
  bool initialized = false;

  OrderStatus orderStatus = OrderStatus::DRAFT;

  void updatePrice();
  void readOrder();

  void sync() {
    using namespace net;
    Response r = fetch();
    if (r.status == -1)
      return;
  }

  void resetState();
  void rmOrder();

public:
  std::unordered_map<uint16_t, OrderItem> items = {};

  void simulatedInput() {
    const char c = tui::Input.getBaseInput();
    int16_t h = (c == 'p') - (c == 'i');
    if (h != 0) {
      int b = static_cast<int>(orderStatus);
      b += h;
      if (b >= static_cast<int>(OrderStatus::LEN) && h == 1) {
        b = 1;
      }
      if (b == static_cast<int>(OrderStatus::INVALID) && h == -1) {
        b = static_cast<int>(OrderStatus::LEN) - 1;
      }
      orderStatus = static_cast<OrderStatus>(b);
    }
  }
  void init();
  void makeOrder();
  void updateOrder() {
    sync();
    updatePrice();
    payment.update();
    rmOrder();
  }
  void upPriceDiff(bool v = true) {
    if (diff == false)
      diff = v;
  }
  int8_t getOrderStatusIndex() { return static_cast<int8_t>(orderStatus); }
  OrderStatus getOrderStatus() { return orderStatus; }
  const int getId() { return id; }
  bool isOrdered() { return ordered; }
  const int &getTotalPrice() { return totalPrice; }
};

extern Order order;

extern int upsertOrder(const uint16_t &currentMenuId, int qty);
extern void setQuantity(int8_t bipolar);
extern MenuItem &getCurrentItem();
extern const int &getQuantity();
extern void updateOrderWarnMessage();
