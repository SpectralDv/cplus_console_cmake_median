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
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    #define PLATFORM_WINDOWS 0
    #define PLATFORM_UNIX 1
#else
    #define PLATFORM_WINDOWS 0
    #define PLATFORM_UNIX 0
    #warning "Unknown platform!"
#endif

//./app.exe --help
//./app.exe --config config.toml

// Атомарный флаг для безопасной работы с сигналом
std::atomic<bool> running{true};

// Обработчик сигнала
void signal_handler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nПолучен сигнал Ctrl+C. Завершение работы..." << std::endl;
        running = false;
    }
}


int main(int argc, char* argv[]) {
    // Установка обработчика сигнала
    std::signal(SIGINT, signal_handler); //SIGTSTP

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
    
    _csv_reader.search_files(_config_parser.umap_config["input"]);

    std::cout << "Найдено файлов: " << _csv_reader.count_files << std::endl; 
    for (const auto& [outer_key, inner_map] : _csv_reader.umap_files) {
        std::cout << "- " << outer_key << std::endl;
    }
    std::cout << "Прочитано записей: " << _csv_reader.count_write << std::endl; 

    auto it = _csv_reader.umap_files.begin();
    int counter_file = 0;
    int counter_step = 0;
    while (running) {

        double prew_price = 0;
        double next_price = 0;
        
        if (it == _csv_reader.umap_files.end()) {break;}

        const auto& outer_key = (*it).first;

        if(counter_file == 0){
            //std::string str_prew_price = _csv_reader.get_value(outer_key,"price1");
            //if(str_prew_price != "")
            //{prew_price = std::stod(str_prew_price);_median_calculator.accumulators(prew_price);}
        }else{
            prew_price = _median_calculator.median_price;
        }

        counter_file++;
        
        std::string key = "price"+std::to_string(counter_file);//+1
        std::string str_next_price =  _csv_reader.get_value(outer_key,key);
        //std::cout << "str_next_price: " << str_next_price << std::endl; 

        //std::setlocale(LC_NUMERIC, "POSIX");
        std::replace(str_next_price.begin(), str_next_price.end(), '.', ',');

        if(str_next_price == ""){++it;counter_file=0;continue;}
        else{next_price = std::stod(str_next_price);}

        //_median_calculator.calculate(prew_price,next_price);
        _median_calculator.accumulators(next_price);

        counter_step++;
        std::cout << "Шаг "<< counter_step;
        std::replace(str_next_price.begin(), str_next_price.end(), ',', '.');
        std::cout << " price = "<< str_next_price;
        std::string median = std::to_string(_median_calculator.median_price);
        std::replace(median.begin(), median.end(), ',', '.');
        std::cout << " медиана = "<< median;

        
        if(_median_calculator.median_price == prew_price){
            std::cout << " медиана не изменилась, запись НЕ производится "<< std::endl;
        }else{
            _median_calculator.count_change++;
            _csv_reader.write_file(
                _config_parser.umap_config["output"]+"/result.csv",
                std::to_string(counter_step),
                std::to_string(_median_calculator.median_price));
            std::cout << " запись в файл "<< std::endl;
        }
        // Имитация работы
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Записано изменений медианы: " << _median_calculator.count_change << std::endl; 

    std::cout << "Результат сохранен: " << _config_parser.umap_config["output"]+"/result.csv" << std::endl; 

    std::cout << "Завершение работы" << std::endl; 
    std::cin.get();

    return 0;
}
