#ifndef MEDIAN_CALCULATOR_H
#define MEDIAN_CALCULATOR_H

#include <string>
#include <sstream>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>



namespace birga {
// Атомарный флаг для безопасной работы с сигналом
std::atomic<bool> running{true};

// Обработчик сигнала
void signal_handler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nПолучен сигнал Ctrl+C. Завершение работы..." << std::endl;
        running = false;
    }
}

using namespace boost::accumulators;
class median_calculator {
public: 
    accumulator_set<double, features<tag::median>> acc;
    int count_change = 0;
    double median_price = 0;

public:  
    median_calculator()
    {
        // Установка обработчика сигнала
        std::signal(SIGINT, signal_handler); //SIGTSTP
    }
    ~median_calculator(){}

    void calculate(double prew_median,double next_price)
    {
        median_price = (prew_median + next_price) / 2.0f;
        //std::cout << "      calculate = "<< std::to_string(prew_median);
        //std::cout << " "<< std::to_string(next_price) <<std::endl;
    }
    void accumulators(double next_price)
    {
        acc(next_price);
        
        size_t n = count(acc);
        if (n >= 3) {
            median_price = median(acc);
        } else if (n >= 2) {
            // Если есть минимум 2 элемента, медиана осмысленна
            median_price = (median_price + next_price) / 2.0f;
        } else {
            // Для первого элемента медиана равна самому значению
            median_price = next_price;
        }

        //std::cout << "      accumulators = "<< std::to_string(median_price);
        //std::cout << " next_price = "<< std::to_string(next_price) << std::endl;
    }


    void update(config_parser& _config_parser,csv_reader& _csv_reader)
    {
        auto it = _csv_reader.umap_files.begin();
        int counter_file = 0;
        int counter_step = 0;
        while (running) {

            double prew_price = 0;
            double next_price = 0;
            
            if (it == _csv_reader.umap_files.end()) {break;}

            const auto& outer_key = (*it).first;

            if(counter_file != 0){prew_price = median_price;}

            counter_file++;
            
            std::string key = "price"+std::to_string(counter_file);//+1
            std::string str_next_price =  _csv_reader.get_value(outer_key,key);
            //std::cout << "str_next_price: " << str_next_price << std::endl; 

            //std::setlocale(LC_NUMERIC, "POSIX");
            std::replace(str_next_price.begin(), str_next_price.end(), '.', ',');

            if(str_next_price == ""){++it;counter_file=0;continue;}
            else{next_price = std::stod(str_next_price);}

            //calculate(prew_price,next_price);
            accumulators(next_price);

            counter_step++;
            std::cout << "Шаг "<< counter_step;
            std::replace(str_next_price.begin(), str_next_price.end(), ',', '.');
            std::cout << " price = "<< str_next_price;
            std::string median = std::to_string(median_price);
            std::replace(median.begin(), median.end(), ',', '.');
            std::cout << " медиана = "<< median;

            
            if(median_price == prew_price){
                std::cout << " медиана не изменилась, запись НЕ производится "<< std::endl;
            }else{
                count_change++;
                _csv_reader.write_file(
                    _config_parser.umap_config["output"]+"/result.csv",
                    std::to_string(counter_step),
                    std::to_string(median_price));
                std::cout << " запись в файл "<< std::endl;
            }
            // Имитация работы
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cout << "Записано изменений медианы: " << count_change << std::endl; 

        std::cout << "Результат сохранен: " << _config_parser.umap_config["output"]+"/result.csv" << std::endl; 

    }
};
}

#endif