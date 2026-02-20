#ifndef generate_H
#define generate_H


#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>
#include <string>
#include <chrono>

// Функция для генерации случайного числа в заданном диапазоне
template<typename T>
T random_range(T min, T max, std::mt19937_64& gen) {
    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(gen);
    } else {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(gen);
    }
}

void generate() {
    // Создаём генератор случайных чисел
    std::random_device rd;
    std::mt19937_64 gen(rd());
    
    // Имя выходного файла
    std::string filename = "data.csv";
    
    // Открываем файл для записи
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Ошибка открытия файла для записи!" << std::endl;
        return;
    }
    
    // Устанавливаем точность для чисел с плавающей точкой
    file << std::fixed << std::setprecision(8);
    
    // Основной цикл для генерации строк
    const int NUM_ROWS = 150000;
    
    // Базовое время (как в примере - около 1716810808593627)
    long long base_time = 1716810808593627LL;
    
    // Возможные типы операций
    std::string types[] = {"bid", "ask"};
    
    for (int i = 0; i < NUM_ROWS; ++i) {
        // Генерируем первое время (с небольшими вариациями)
        long long time1 = base_time + random_range(-1000, 1000, gen);
        
        // Генерируем второе время (обычно немного меньше первого)
        long long time2 = time1 - random_range(0, 500, gen);
        
        // Генерируем цену (около 68480)
        double price = 68480.0 + random_range(-100.0, 100.0, gen);
        
        // Генерируем количество (обычно небольшое)
        double amount = random_range(0.001, 15.0, gen);
        
        // Выбираем тип операции
        std::string type = types[random_range(0, 1, gen)];
        
        // Генерируем флаг (0 или 1)
        int flag = random_range(0, 1, gen);
        
        // Записываем строку в файл
        file << time1 << ";"
             << time2 << ";"
             << std::setprecision(8) << price << ";"
             << std::setprecision(8) << amount << ";"
             << type << ";"
             << flag;
        
        // Добавляем перевод строки (кроме последней строки)
        if (i < NUM_ROWS - 1) {
            file << "\n";
        }
    }
    
    file.close();
    
    std::cout << "Файл " << filename << " успешно создан!" << std::endl;
    std::cout << "Сгенерировано " << NUM_ROWS << " строк." << std::endl;
}


#endif