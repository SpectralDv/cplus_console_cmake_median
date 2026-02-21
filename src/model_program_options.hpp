#ifndef MODEL_PROGRAM_OPTIONS_H
#define MODEL_PROGRAM_OPTIONS_H

#include <boost/program_options.hpp>
#include <string>

namespace birga {
namespace po = boost::program_options;
class model_program_options {
public: 
    std::string name_config = "config.toml";
    
    model_program_options(){}
    ~model_program_options(){}

    int input(int argc, char* argv[],config_parser& _config_parser){
        try {
            po::options_description desc("Допустимые опции");

            desc.add_options()
                ("help,h", "Показать справку")
                ("config,c", po::value<std::string>(), 
                    "Путь к конфигурационному файлу");

            // Парсим аргументы
            po::variables_map vm;
            po::store(po::parse_command_line(argc, argv, desc), vm);

            // Проверяем, если запрошена справка
            if (vm.count("help")) {
                std::cout << desc << std::endl;
                std::cin.get();
                return 1;
            }

            // Уведомляем об ошибках (required опции)
            po::notify(vm);
            
            if (vm.count("config")) {
                _config_parser.name_config = vm["config"].as<std::string>();
                //std::cout << "Конфиг: " << vm["config"].as<std::string>() << std::endl;
            }
        } catch (const po::error& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
            std::cin.get();
            return 1;
        }
        return 0;
    }
};
}
#endif