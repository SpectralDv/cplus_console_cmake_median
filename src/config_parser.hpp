#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <unordered_map>
#include <fstream>
#include <cpptoml.h>

namespace birga {
class config_parser {
public: 
    std::string name_config = "config.toml";
    std::string path_input = "config.toml";
    std::string path_output = "config.toml";
    std::string filename_mask = "config.toml";
    std::unordered_map<std::string, std::string> umap_config;
    std::unordered_map<std::string, std::string> umap_mask;

public: 
    config_parser()
    {
        umap_config = {
            {"name_config", "config.toml"},
            {"input", "/path/to/input_dir"},
            {"output", "/path/to/output_dir"},
        };
    }
    ~config_parser(){}
    bool load_from_file(){
        try {
            // Парсим TOML файл
            auto config = cpptoml::parse_file(name_config);
            
            // Читаем input
            auto input = config->get_as<std::string>("input");
            if (input) {
                std::string input_path = *input;
                update_umap(static_cast<std::string>("input"),*input,umap_config);
                //std::cout << "input: " << *input << std::endl;
            }
            
            // Читаем output
            auto output = config->get_as<std::string>("output");
            if (output) {
                std::string output_path = *output;
                update_umap("output",*output,umap_config);
                //std::cout << "output: " << *output << std::endl;
            }
            
            // Читаем filename_mask как массив
            auto masks = config->get_array_of<std::string>("filename_mask");
            if (masks) {
                std::vector<std::string> filename_masks = *masks;
                
                // Сохраняем в config_map как строку
                std::string masks_str;
                for (size_t i = 0; i < filename_masks.size(); ++i) {
                    if (i > 0) masks_str += ",";
                    masks_str += filename_masks[i];
                }
                update_umap(static_cast<std::string>("filename_mask"),masks_str,umap_config);
                
                //std::cout << "filename_mask: ";
                for (const auto& mask : filename_masks) {
                    //std::cout << mask << " ";
                    update_umap(mask,mask,umap_mask);
                }
                //std::cout << std::endl;
            }

            //parser(key, value)
            return true;
            
        } catch (const cpptoml::parse_exception& e) {
            std::cerr << "Ошибка парсинга TOML: " << e.what() << std::endl;
            return false;
        }
    }

private:
    void update_umap(std::string key,std::string value,std::unordered_map<std::string,std::string>& umap)
    {
        auto [it, inserted] = umap.insert({key, value});
        if (inserted){
            // std::cout << "Элемент key добавлен: " << it->second << std::endl;
        }else{
            umap[key] = value;
            // std::cout << "Элемент key уже существует: " << it->second << std::endl;
        }
    }

//     void parser(const std::string& key, const std::string& value){
//         if(key == "filename_mask"){
//             parser_filename_mask(key,value);
//             return;
//         }
        
//     } 
//     void parser_filename_mask(const std::string& key, const std::string& value)
//     {
//         std::string trimmed = value;
//         // Удаляем квадратные скобки
//         if (trimmed.front() == '[') trimmed = trimmed.substr(1);
//         if (trimmed.back() == ']') trimmed.pop_back();
        
//         // Разделяем по запятым
//         std::vector<std::string> items;
//         boost::split(items, trimmed, boost::is_any_of(","));

//         // Очищаем каждый элемент от кавычек и пробелов
//         for (auto& item : items) {
//             boost::trim(item);
//             boost::erase_all(item, "'");
//             boost::erase_all(item, "\"");
//             if (!item.empty()) {
//                 // insert возвращает пару (итератор, bool)
//                 // bool = true если элемент был вставлен, false если уже существовал
//                 auto [it, inserted] = umap_mask.insert({key, item});
//                 if (inserted) {
//                     //std::cout << "Элемент key3 добавлен: " << it->second << std::endl;
//                 } else {
//                     umap_mask[key] = item;
//                     //std::cout << "Элемент key3 уже существует: " << it->second << std::endl;
//                 }
//             }
//         }
//     }
};
}

#endif