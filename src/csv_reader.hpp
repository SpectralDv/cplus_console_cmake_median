#ifndef CSV_READER_H
#define CSV_READER_H

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <locale>

#include "config_parser.hpp"

namespace birga {
namespace fs = std::filesystem;
class csv_reader {
public: 
    std::string name_config = "config.toml";
    int count_files = 0;
    int count_write = 0;
    //<name_file,text_file=<receive_ts0,1716810808593627>>
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> umap_files;
public:
    csv_reader(){}
    ~csv_reader(){}
    void update(config_parser& _config_parser) {
        search_files(_config_parser.umap_config["input"]);
        count_files = static_cast<int>(umap_files.size());
        if(count_files == 0){
            search_files_mask(_config_parser.umap_config["filename_mask"]);
        }
        count_files = static_cast<int>(umap_files.size());
    }
    void search_files(std::string path) {
        try {
            fs::path input_path(path);
            //fs::path input_path = fs::current_path(); // /path 

            if (!fs::exists(input_path) || !fs::is_directory(input_path)) {
                std::cout << "Некорректный путь: " << input_path << std::endl;
                return;
            }

            spdlog::info("Файлы в текущей директории:");
            //std::cout << "Файлы в текущей директории:" << std::endl;
            
            // Только текущая директория (без поддиректорий)
            for (const auto& entry : fs::directory_iterator(input_path)) {
                if (fs::is_regular_file(entry.path())) {
                    //count_files ++;
                    //std::cout << "  - " << entry.path().filename().string() << std::endl;
                    read_files(entry.path().filename().string(),entry.path().string());
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }
    void search_files_mask(std::string mask) {
        std::stringstream ss(mask);
        std::string path;
        while (std::getline(ss, path, ',')) {
            search_files(path);
        }
    }
    std::string get_value(std::string name_file,std::string key){
        // Проверяем существование внешнего ключа (файла)
        auto outer_it = umap_files.find(name_file);
        if (outer_it != umap_files.end()) {
            // Получаем ссылку на внутреннюю map
            const auto& inner_map = outer_it->second;

            // Проверяем существование внутреннего ключа
            auto inner_it = inner_map.find(key);
            if (inner_it != inner_map.end()) {
                return inner_it->second;  
            }
        }
        return "";  
    }
    void init_write_file(const std::string& path) {
        std::ofstream file(path);
        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла: " << path << std::endl;
            return;
        }
        file << "receive_ts;price_median\n";
        file.close();
    }
    void remove_file(const std::string& path) {
        //std::ofstream file(path);
        if (std::filesystem::exists(path)) {
            if (!std::filesystem::remove(path)) {
                std::cerr << "Не удалось удалить файл " << path << std::endl;
                return;
            }
        }
    }

    void write_file(const std::string& path,std::string receive_ts,std::string price_median) {
        
        std::ofstream file(path, std::ios::app | std::ios::out);

        if (!file.is_open()) {
            std::cerr << "Ошибка открытия файла: " << path << std::endl;
            return;
        }

        file.seekp(0, std::ios::end);
        if (file.tellp() == 0) {
            // Файл пустой - записываем заголовок
            file << "receive_ts;price_median\n";
        }
        
        std::replace(receive_ts.begin(), receive_ts.end(), ',', '.');
        std::replace(price_median.begin(), price_median.end(), ',', '.');
        file << receive_ts+";"+price_median+"\n";
        file.close();
        //std::cout << "Файл успешно записан: " << path << std::endl;
    }

private:
    void read_files(const std::string& filename,const std::string& path) {
        spdlog::info("Чтение файла: " + filename);
        //std::cout << "Чтение файла: " << filename << std::endl;
        
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
            return;
        }
        
        std::string line;
        int row_count = 0;
        
        
        while (std::getline(file, line)) {
            // Пропускаем пустые строки
            if (line.empty()) continue;

            int col_count = 0;
            row_count++;
            //std::cout << "Строка " << row_count << ": " << line << std::endl;
            
            // Разбиваем строку на колонки
            std::stringstream ss(line);
            std::string column;
            
            while (std::getline(ss, column, ';')) {
                col_count++;
                std::string type = "type"+std::to_string(col_count);
                //std::cout << type << ": " << column << std::endl;
                if(row_count == 1){
                    update_umap(filename,type,column,umap_files);
                }else{
                    std::string type = umap_files[filename]["type"+std::to_string(col_count)]+std::to_string(row_count-1);
                    update_umap(filename,type,column,umap_files);
                }
            }
            
            //std::cout << std::endl;
        }
        count_write += row_count;
        
        //std::cout << "Всего строк: " << row_count << std::endl;
        file.close();
    }

    // Шаблонная функция для проверки существования ключа в любом map
    template<typename MapType>
    bool contains_key(const MapType& map, const typename MapType::key_type& key) {
        return map.find(key) != map.end();
    }
    void update_umap(
        std::string file,
        std::string key,
        std::string value,
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& umap)
    {
        // Проверка только внешнего ключа
        if (contains_key(umap, file)) {
            //std::cout << "Внешний ключ '" << file << "' существует\n";
            //std::cout << "   Вложенных ключей: " << umap.at(file).size() << "\n";
            update_umap(key,value,umap[file]);
        } else {
            //std::cout << "Внешний ключ '" << file << "' не существует\n";
            umap.insert({file, std::unordered_map<std::string, std::string>()});
            update_umap(key,value,umap[file]);
        }
    }
    void update_umap(std::string key,std::string value,std::unordered_map<std::string,std::string>& umap)
    {
        auto [it, inserted] = umap.insert({key, value});
        if (inserted){
            //std::cout << "Элемент key добавлен: " << it->first + " " + it->second << std::endl;
        }else{
            umap[key] = value;
            //std::cout << "Элемент key уже существует: " << it->first + " " + it->second << std::endl;
        }
    }
};
}

#endif


// level_csv
// receive_ts — временная метка получения данных (микросекунды)
// exchange_ts — временная метка биржи (микросекунды)
// price — цена
// quantity — объем
// side — сторона (`bid` или `ask`)
// rebuild — флаг перестроения стакана (0 или 1)

// trade_csv
// receive_ts — временная метка получения данных (микросекунды)
// exchange_ts — временная метка биржи (микросекунды)
// price — цена сделки
// quantity — объем сделки
// side — сторона (`bid` или `ask`)

// out.csv
// receive_ts — временная метка события, которое изменило медиану
// price_median — значение медианы, рассчитанное на основе всех цен, считанных до этого момента