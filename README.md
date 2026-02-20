1.работа с git 
git init 
git add .gitignore 
git add . 
git commit -m "commit" 
git branch -M main 
git remote add origin https://github.com/spectraldv/cplus_console_cmake_median.git 
git push -u origin main 

git push 
git status 

2.работа с проектом 
mkdir build 
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release 
cmake --build build 