# Инструкция по сборке Pimino из исходников

Это руководство описывает процесс сборки Pimino из исходного кода.

## Быстрый старт

### Linux

```bash
# Клонируйте репозиторий
git clone https://github.com/BanderoLio/Pimino.git
cd Pimino

# Используйте Makefile для простой сборки
make vcpkg-setup      # Настроить vcpkg (только первый раз)
make vcpkg-install    # Установить зависимости
make                  # Собрать проект
make install          # Установить в директорию install/
```

### Windows

```cmd
REM Клонируйте репозиторий
git clone https://github.com/BanderoLio/Pimino.git
cd Pimino

REM Настройте vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install fluidsynth

REM Вернитесь в директорию проекта
cd ..

REM Соберите проект
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build build --config Release
```

## Требования

### Общие требования

- **CMake** 3.16 или новее
- **Git** для клонирования репозитория и vcpkg
- **C++ компилятор** с поддержкой C++17:
  - Linux: GCC 9+ или Clang 10+
  - Windows: MSVC 2019+ или MinGW-w64

### Linux

- **Qt6** (6.8 или новее):
  ```bash
  sudo apt-get install qt6-base-dev qt6-declarative-dev qt6-quickcontrols2-dev qt6-svg-dev
  ```

- **FluidSynth** (через vcpkg или системный пакет):
  ```bash
  sudo apt-get install libfluidsynth-dev
  ```

- **Ninja** (рекомендуется):
  ```bash
  sudo apt-get install ninja-build
  ```

### Windows

- **Visual Studio 2022** с компонентами:
  - Desktop development with C++
  - CMake tools for Windows

- **Qt6** (6.8 или новее):
  - Можно установить через Qt Installer
  - Или использовать через vcpkg (автоматически)

- **vcpkg** (будет установлен автоматически через Makefile)

## Детальная инструкция

### Шаг 1: Получение исходного кода

```bash
git clone https://github.com/BanderoLio/Pimino.git
cd Pimino
```

### Шаг 2: Настройка vcpkg

vcpkg используется для управления зависимостями (FluidSynth).

**Linux/Mac:**
```bash
make vcpkg-setup
```

**Windows:**
```cmd
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
cd ..
```

### Шаг 3: Установка зависимостей

**Linux/Mac:**
```bash
make vcpkg-install
```

**Windows:**
```cmd
vcpkg\vcpkg.exe install fluidsynth:x64-windows
```

### Шаг 4: Конфигурация CMake

**Linux/Mac:**
```bash
cmake -B build -S . \
  -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -G Ninja
```

**Windows:**
```cmd
cmake -B build -S . ^
  -DCMAKE_TOOLCHAIN_FILE=vcpkg\scripts\buildsystems\vcpkg.cmake ^
  -DCMAKE_BUILD_TYPE=Release ^
  -G "Visual Studio 17 2022" -A x64
```

### Шаг 5: Сборка

**Linux/Mac:**
```bash
cmake --build build --config Release
```

**Windows:**
```cmd
cmake --build build --config Release
```

Или используйте Makefile:
```bash
make
```

### Шаг 6: Установка

**Linux/Mac:**
```bash
cmake --install build --prefix install
```

Или через Makefile:
```bash
make install
```

**Windows:**
```cmd
cmake --install build --prefix install
```

## Использование Makefile

Makefile предоставляет удобные команды для сборки:

```bash
make help          # Показать справку
make               # Собрать проект
make clean         # Очистить директорию сборки
make install       # Установить в install/
make vcpkg-setup   # Настроить vcpkg
make vcpkg-install # Установить зависимости через vcpkg
make check-deps    # Проверить системные зависимости
make distclean     # Полная очистка (включая vcpkg)
```

## Переменные окружения

Вы можете настроить сборку через переменные окружения:

```bash
export VERSION=1.0.0
export BUILD_DIR=custom_build
make
```

## Решение проблем

### Ошибка: Qt6 не найден

**Linux:**
```bash
sudo apt-get install qt6-base-dev qt6-declarative-dev qt6-quickcontrols2-dev
```

**Windows:**
Убедитесь, что Qt6 установлен и переменная `Qt6_DIR` указывает на правильную директорию:
```cmd
set Qt6_DIR=C:\Qt\6.8.0\msvc2022_64
```

### Ошибка: FluidSynth не найден

Убедитесь, что vcpkg правильно настроен и зависимости установлены:
```bash
make vcpkg-install
```

Или вручную:
```bash
vcpkg/vcpkg install fluidsynth:x64-linux  # Linux
vcpkg/vcpkg install fluidsynth:x64-windows  # Windows
```

### Ошибка компиляции

1. Убедитесь, что все зависимости установлены
2. Проверьте версию компилятора:
   ```bash
   g++ --version  # Должна быть 9+
   ```
3. Очистите и пересоберите:
   ```bash
   make clean
   make
   ```

### Проблемы с vcpkg

Если vcpkg не работает:
1. Удалите директорию vcpkg
2. Запустите `make vcpkg-setup` заново
3. Убедитесь, что у вас есть интернет-соединение

## Структура проекта

```
Pimino/
├── src/              # Исходный код
│   ├── core/         # Основная логика
│   ├── external/     # Внешние библиотеки
│   ├── qml/          # QML интерфейс
│   └── main.cpp      # Точка входа
├── CMakeLists.txt    # CMake конфигурация
├── Makefile          # Упрощенная сборка
├── vcpkg-configuration.json  # Конфигурация vcpkg
└── build/            # Директория сборки (создается автоматически)
```

## Дополнительная информация

- [CMake документация](https://cmake.org/documentation/)
- [Qt6 документация](https://doc.qt.io/qt-6/)
- [FluidSynth документация](https://www.fluidsynth.org/)
- [vcpkg документация](https://vcpkg.io/)

