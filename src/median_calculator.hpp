#ifndef MEDIAN_CALCULATOR_H
#define MEDIAN_CALCULATOR_H

#include <string>
#include <sstream>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include "csv_reader.hpp"


namespace birga {
using namespace boost::accumulators;
class median_calculator {
public: 
    accumulator_set<double, features<tag::median>> acc;
    int count_change = 0;
    double median_price = 0;

public:  
    median_calculator(){}
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


    void update(csv_reader _csv_reader)
    {

    }
};
}

#endif