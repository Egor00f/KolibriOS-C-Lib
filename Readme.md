# KolibriOS C++ Lib



библиотека для (чуть более) удобной работы с KolibriOS, с похренитетом к оптимизации. Для всего и сразу.

Сия библиотека представляет из себя прослойку абстракций, переименованных функций и прочей фигни, что упростит разработку приложений


АХТУНГ! приложения использующие эту либу будут очень большими и медлеными.

## Зачем оно надо

+ Эта библиотека даёт упращенный доступ к системным функциям
+ чуть более удобно делать UI, чем просто системными API и/или C_Layer(но кому как)

## Сборка

По умолчанию предпологается что репозитрий kolibrios лежит в той же папке что и папка этого проекта, но вы можете отредактировать CMakeLists.txt, чтобы изменить путь до репозитория. Если у вас нет репозитория, то вот ссылка [https://github.com/KolibriOS/kolibrios](https://git.kolibrios.org/KolibriOS/kolibrios.git)

Для сборки требуются:
+ `cmake` https://cmake.org/
+ `make` https://gnuwin32.sourceforge.net/packages/make.htm
+ `kos32-gcc` http://wiki.kolibrios.org/wiki/%D0%9F%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5_%D0%BD%D0%B0_%D0%A1/C%2B%2B#GCC.2FG.2B.2B
+ `fasm` https://flatassembler.net/download.php

Сборка:
```
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Как использовать это

1. Добавте эту библиотеку как субмодуль в ваш проект
2. включите в проект с помощью cmake
   ```
   add_subdirectory(KolibriOS-Cpp-Lib)

   tareget_link_libraries(yourexeTarget PRIVATE KolibriLib)
   ```
4. Подключите заголовочный файл:
   ```
   #include <KolibriLib.hpp>
   ```
4. Прочитайте doxygen (https://egor00f.github.io/KolibriOS-Cpp-Lib)

Примеры использования лежат в папке `examples`

ВОЗМОЖНО что то полезное есть в wiki на github

Тема на форуме kolibrios: http://board.kolibrios.org/viewtopic.php?t=5621
