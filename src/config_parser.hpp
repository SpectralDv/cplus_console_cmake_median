#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <unordered_map>
#include <fstream>
#include <cpptoml.h>

namespace birga {
class config_parser {
public: 
    std::string name_config = "";
    std::string path_input = "";
    std::string path_output = "";
    std::string filename_mask = "";
    std::unordered_map<std::string, std::string> umap_config;
    std::unordered_map<std::string, std::string> umap_mask;

public: 
    config_parser()
    {
        umap_config = {
            {"name_config", "config.toml"},
            {"input", "path/to/input_dir"},
            {"output", "path/to/output_dir"},
        };
    }
    ~config_parser(){}
    bool load_from_file(){
        try {
            // Парсим TOML файл
            auto config = cpptoml::parse_file(name_config);
            update_umap("name_config",name_config,umap_config);
            
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
};
}

#endif