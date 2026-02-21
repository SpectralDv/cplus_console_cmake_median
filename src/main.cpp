#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>
#include <spdlog/spdlog.h>

#include "generate.hpp"
#include "csv_reader.hpp"
#include "config_parser.hpp"
#include "median_calculator.hpp"
#include "model_program_options.hpp"

#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS 1
    #define PLATFORM_UNIX 0
#elif defined(__linux__) || defined(__unix__)
    #define PLATFORM_WINDOWS 0
    #define PLATFORM_UNIX 1
#else
    #define PLATFORM_WINDOWS 0
    #define PLATFORM_UNIX 0
    #warning "Unknown platform!"
#endif


int main(int argc, char* argv[]) {
    #if PLATFORM_WINDOWS
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    #endif

    birga::config_parser _config_parser; 
    birga::csv_reader _csv_reader;
    birga::median_calculator _median_calculator;
    birga::model_program_options _model_program_options;

    spdlog::set_level(spdlog::level::info); 
    spdlog::info("Добро пожаловать в программу!");

    std::cout << "Запуск приложения csv_median_calculator v1.0.0" << std::endl; 
    
    int result = _model_program_options.input(argc, argv, _config_parser);
    if(result == 1){return 0;}

    std::cout << "Чтение конфигурации: " + _config_parser.name_config << std::endl; 
    _config_parser.load_from_file();
    std::cout << "Входная директория: " + _config_parser.umap_config["input"] << std::endl; 
    std::cout << "Выходная директория: " + _config_parser.umap_config["output"] << std::endl; 
    _csv_reader.init_write_file(_config_parser.umap_config["output"]+"/result.csv");
    
    //_csv_reader.search_files(_config_parser.umap_config["input"]);
    _csv_reader.update(_config_parser);

    std::cout << "Найдено файлов: " << _csv_reader.count_files << std::endl; 
    for (const auto& [outer_key, inner_map] : _csv_reader.umap_files) {
        std::cout << "- " << outer_key << std::endl;
    }
    std::cout << "Прочитано записей: " << _csv_reader.count_write << std::endl; 

    _median_calculator.update(_config_parser,_csv_reader);

    std::cout << "Завершение работы" << std::endl; 
    std::cin.get();

    return 0;
}
