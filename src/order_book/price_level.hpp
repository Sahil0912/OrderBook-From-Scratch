#pragma once
#include "order.hpp"

class PriceLevel{
    private : 
        Price price;
        Order* head;
        Order* tail; // for appending the order at last (FIFO)
        int orderCount;
    public : 
        explicit PriceLevel(Price _price) : price(_price), head(nullptr), tail(nullptr), orderCount(0) {}
        Price GetPrice() const{ return price; }
        bool IsEmpty() const{ return (orderCount == 0); }
        void AddOrder(Order* order) {
            if(!head){
                head = tail = order;
                order->prev = order->next = nullptr;
            }
            else{
                tail->next = order;
                order->prev = tail;
                order->next = nullptr;
                tail = order;
            }
            orderCount++;
        }
        void RemoveTopOrder() {
            head = head->next;
            orderCount--;
            if(head) head->prev = nullptr;
            else tail = nullptr;
        }
        Order* GetHead() const { return head; }
        void PrintPriceLevel();
        

};