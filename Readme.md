# KolibriOS C++ Lib



библиотека для (чуть более) удобной работы с KolibriOS, с похренитетом к оптимизации. Для всего и сразу.

Сия библиотека представляет из себя прослойку абстракций, переименованных функций и прочей фигни, что упростит разработку приложений


АХТУНГ! приложения использующие эту либу будут очень большими и медлеными.

## Зачем оно надо

+ Эта библиотека даёт упращенный доступ к системным функциям
+ чуть более удобно делать UI, чем просто системными API и/или C_Layer(но кому как)

## Сборка

Клонирует репозиторий
```
git clone https://github.com/Egor00f/KooibriOS-Cpp-Lib.git
```

По умолчанию предполагается что репозиторий kolibrios лежит в той же папке что и папка этого проекта, но вы можете отредактировать CMakeLists.txt, чтобы изменить путь до репозитория. Если у вас нет репозитория, то вот ссылка [https://github.com/KolibriOS/kolibrios](https://git.kolibrios.org/KolibriOS/kolibrios.git). 
Короче просто запускаете в той же папке что и этот проект:
```
git clone https://git.kolibrios.org/KolibriOS/kolibrios.git
```


### Windows

Для сборки требуются:
+ `cmake` https://cmake.org/
+ `make` https://gnuwin32.sourceforge.net/packages/make.htm
+ `kos32-gcc` http://wiki.kolibrios.org/wiki/%D0%9F%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5_%D0%BD%D0%B0_%D0%A1/C%2B%2B#GCC.2FG.2B.2B
+ `fasm` https://flatassembler.net/download.php

### Linux

Устанавливаем тулчейн
```
git clone https://github.com/Egor00f/kolibrios-gcc-toolchain.git
kolibrios-gcc-toolchain/./install
```

Остальные зависимости:
```
apt update
apt install fasm cmake make
```

### Сама сборка

Сборка:
```
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../KolibriOS-toolchain-file/toolchain.cmake -DKOLIBRIOS_REPO=../kolibrios -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

`KOLIBRIOS_REPO` - это путь до репозитория kolibrios

## Как использовать это

1. Добавьте эту библиотеку как модуль в ваш проект
2. включите в проект с помощью cmake
   ```
   add_subdirectory(KolibriOS-Cpp-Lib)

   add_kolibri_executable(yourExeTarget)

   target_link_libraries(yourExeTarget PRIVATE KolibriLib)
   ```
   И При сборке вашего проекта используйте toolchain файл:
   ```
   cmake .. -DCMAKE_TOOLCHAIN_FILE=KolibriOS-Cpp-Lib/KolibriOS-toolchain-file/toolchain.cmake -DKOLIBRIOS_REPO=../kolibrios
   ```
4. Подключите заголовочный файл:
   ```
   #include <KolibriLib.hpp>
   ```
4. Прочитайте doxygen (https://egor00f.github.io/KolibriOS-Cpp-Lib)

### Логи

Логи выводятся на доску отладки в файл debug.log на рам-диске

### Флаги

Флаги влияют на уровни логов

+ `NO_LOGS` - Без логов.
+ `DEBUG` - Вывод отладочной информации. Включен в отладочной конфигурации cmake
+ `VERBOSE` - Подробности отладки. Включен в отладочной конфигурации cmake

## Возможно полезно

Примеры использования лежат в папке `examples`

ВОЗМОЖНО что то полезное есть в wiki на github

Тема на форуме kolibrios: http://board.kolibrios.org/viewtopic.php?t=5621
