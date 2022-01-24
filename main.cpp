#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

enum dishesName
{
    NONE,
    PIZZA,
    SOUP,
    STEAK,
    SALAD,
    SUSHI
};

std::mutex kitchen_access;
std::vector<int> dishes;
std::mutex cooking_access;

void Order()
{
    int countDelivery = 0;
    while(countDelivery < 10)
    {
        int orderInterval = 5 + rand() % (10 - 5);
        std::this_thread::sleep_for(std::chrono::seconds(orderInterval));
        int newOrder = PIZZA + rand() % (SUSHI - PIZZA);

        dishes.push_back(newOrder);
        kitchen_access.lock();
        std::cout << "New order: dishes# " << newOrder << std::endl;
        kitchen_access.unlock();
        countDelivery++;
    }
};


void Cooking()
{
    int countOrder = 0;
    while(countOrder < 10)
    {
        if (dishes.size() > countOrder && dishes[countOrder] != NONE)
        {
            int cookingInterval = 5 + rand() % (15 - 5);
            std::this_thread::sleep_for(std::chrono::seconds(cookingInterval));

            cooking_access.lock();
            std::cout << "Order# " << countOrder << " dishes# " << dishes[countOrder] << " is ready " << std::endl;
            cooking_access.unlock();

            countOrder++;
        }

    }
};

void Delivery()
{
    int countDelivery = 0;
    while(countDelivery < 10) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        kitchen_access.lock();
        std::cout << "Order# " << countDelivery << " has been delivered" << std::endl;
        kitchen_access.unlock();
        countDelivery++;
    }
};


int main() {
    std::thread order(Order);
    std::thread cooking(Cooking);
    cooking.detach();
    std::thread delivery(Delivery);

    order.join();
    if (cooking.joinable()) cooking.join();
    delivery.join();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

#if 0
Требуется реализовать упрощённую модель работы кухни ресторана, работающего в режиме онлайн-доставки.

Онлайн-заказ поступает каждые 5–10 секунд. Это случайное блюдо из пяти: пицца, суп, стейк, салат, суши. Официант, принявший заказ онлайн, оставляет его у кухни.
Если кухня свободна, она принимает заказ и спустя 5–15 секунд возвращает уже готовое блюдо на выдачу. Время определяется случайным образом с помощью функции std::rand() в указанных диапазонах.

Курьер приезжает каждые 30 секунд, забирает готовые блюда на выдаче и развозит их по заказчикам.

Программа завершается после 10 успешных доставок. На каждом из этапов, будь то заказ, или готовка, или доставка заказа, выводятся соответствующие сообщения в консоль.



Советы и рекомендации
        Для симуляции готовки, используйте отдельный мьютекс и запускайте отдельную нить ожидания с вызовом detach для неё.

#endif