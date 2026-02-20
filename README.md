1.добавить проект на git 

git init 

git add .gitignore 

git add . 

git commit -m "commit" 

git branch -M main 

git remote add origin https://github.com/spectraldv/cplus_console_cmake_median.git 

git push -u origin main 

git push 

git status 

2.скачать проект с git

git clone https://github.com/spectraldv/cplus_console_cmake_median.git 

git checkout -b ubuntu

git pull origin main


3.работа с проектом 

# 1. Удалите старую версию (опционально)
sudo apt remove --purge cmake

# 2. Добавьте ключ и репозиторий Kitware
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
echo "deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ $(lsb_release -sc) main" | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null

# 3. Обновите список пакетов и установите CMake
sudo apt update
sudo apt install cmake

# 4. Проверьте версию
cmake --version

mkdir build 

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release 

cmake --build build 

