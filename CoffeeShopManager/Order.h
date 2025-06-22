#pragma once
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include "Product.h"
#include "Member.h"

class Order {
private:
    int orderId;
    std::map<int, int> items;
    int memberId;
    int totalPrice;
    std::string orderTime;

public:
    Order(int orderId, int memberId = 0)
        : orderId(orderId), memberId(memberId), totalPrice(0) {
        time_t now = time(0);
        char buf[80];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        this->orderTime = buf;
    }

    void addItem(int productId, int quantity) { items[productId] += quantity; }
    int getOrderId() const { return orderId; }
    const std::map<int, int>& getItems() const { return items; }
    int getMemberId() const { return memberId; }
    int getTotalPrice() const { return totalPrice; }
    void setTotalPrice(int price) { totalPrice = price; }
};