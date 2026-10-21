#pragma once

#include "ui.hpp"
#include <cstdint>
#include <string>
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
  int totalPrice = 0;
};

extern void mainMenuInput();
extern void mainMenu();
extern void mainMenuInit();
extern Order order;

extern ui::ListProperty listProp;

extern std::vector<MenuItem> productItems;
