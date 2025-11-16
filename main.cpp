#include <iostream>
#include <map>

#include "allocator.hpp"
#include "container.hpp"

// Функция для вычисления факториала
int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    // 1) создание экземпляра std::map<int, int>
    std::map<int, int> standard_map;
    
    // 2) заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    for (int i = 0; i < 10; ++i) {
        standard_map[i] = factorial(i);
    }
    
    // 3) создание экземпляра std::map<int, int> с новым аллокатором, ограниченным 10 элементами
    std::map<int, int, std::less<int>, custom_allocator<std::pair<const int, int>, 10>> custom_alloc_map;
    
    // 4) заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    for (int i = 0; i < 10; ++i) {
        custom_alloc_map[i] = factorial(i);
    }
    
    // 5) вывод на экран всех значений (ключ и значение разделены пробелом), хранящихся в контейнере

    std::cout << "Standard map with standart allocator:" << std::endl;
    for (const auto& pair : standard_map) {
        std::cout << pair.first << " " << pair.second << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Standard map with custom allocator:" << std::endl;
    for (const auto& pair : custom_alloc_map) {
        std::cout << pair.first << " " << pair.second << std::endl;
    }
    std::cout << std::endl;
    
    // 6) создание экземпляра своего контейнера для хранения значений типа int
    custom_container<int> standard_container;
    
    // 7) заполнение 10 элементами от 0 до 9
    for (int i = 0; i < 10; ++i) {
        standard_container.push_back(i);
    }
    
    // 8) создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором, ограниченным 10 элементами
    custom_container<int, custom_allocator<int, 10>> custom_alloc_container;
    
    // 9) заполнение 10 элементами от 0 до 9
    for (int i = 0; i < 10; ++i) {
        custom_alloc_container.push_back(i);
    }
    
    // 10) вывод на экран всех значений, хранящихся в контейнере
    std::cout << "Custom container with custom allocator:" << std::endl;
    for (const auto& value : custom_alloc_container) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
