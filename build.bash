# Сборка проекта
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Запуск с конфигурацией
./build/csv_median_calculator -config config.toml

# Запуск с конфигурацией по умолчанию (config.toml в той же директории)
./build/csv_median_calculator
