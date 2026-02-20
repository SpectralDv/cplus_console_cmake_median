# 0.добавить проект на git (только для разработчика)

git init 

git add .gitignore 

git add . 

git commit -m "commit" 

git branch -M main 

git remote add origin https://github.com/spectraldv/cplus_console_cmake_median.git 

git push -u origin main 

git push 

git status 

# 1.скачать проект с git

git clone https://github.com/spectraldv/cplus_console_cmake_median.git 

(если потребуется)
1.1.обновить проект с git(удалив все свои изменения)

git fetch --all

git reset --hard

git pull origin main

git pull

1.2.добавить свою ветку с git(если потребуется)

git checkout -b ubuntu


# 2.cmake

#  Удалите старую версию (опционально)
sudo apt remove --purge cmake

#  Добавьте ключ и репозиторий Kitware
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
echo "deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ $(lsb_release -sc) main" | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null

#  Обновите список пакетов и установите CMake
sudo apt update
sudo apt install cmake

#  Проверьте версию
cmake --version

# 3.boost

# Debian/Ubuntu
sudo apt update
sudo apt install build-essential autoconf libtool pkg-config
# Fedora/CentOS/RHEL
sudo dnf groupinstall "Development Tools"
sudo dnf install autoconf libtool
# Обновление списка пакетов
sudo apt update

# Установка всех библиотек Boost (рекомендуется для начинающих)
sudo apt install libboost-all-dev -y

# Если нужны только определенные компоненты (например, только thread и system)
sudo apt install libboost-thread-dev libboost-system-dev -y

# 4.логирование
sudo apt install libspdlog-dev -y

# 5.cpptoml
sudo apt install libcpptoml-dev -y

mkdir build 

# 6.сборка
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release 

cmake --build build 

# 7.редактирование конфига config.toml
в поле input = записать путь до path/to/input_dir
в поле output = записать путь до path/to/output_dir

# 8.запуск приложение
./app.exe
./app.exe --help
./app.exe --config config.toml