# Инструкция по созданию релиза на GitHub Releases

Это руководство описывает процесс создания и публикации релизов Pimino на GitHub Releases.

## Автоматический деплой (рекомендуется)

### Шаг 1: Подготовка

1. Убедитесь, что все изменения закоммичены и запушены:
   ```bash
   git add .
   git commit -m "Описание изменений"
   git push origin main
   ```

2. Обновите версию в `CMakeLists.txt` (если нужно):
   ```cmake
   project(Pimino VERSION 1.0.0 LANGUAGES CXX)
   ```

### Шаг 2: Создание тега

Создайте тег для релиза:
```bash
git tag -a v1.0.0 -m "Release version 1.0.0"
git push origin v1.0.0
```

Или через веб-интерфейс GitHub:
1. Перейдите в раздел Releases
2. Нажмите "Draft a new release"
3. Выберите или создайте тег (например, `v1.0.0`)
4. Заполните описание релиза
5. Нажмите "Publish release"

### Шаг 3: Автоматическая сборка

После создания тега GitHub Actions автоматически:
1. Соберет проект для Linux и Windows
2. Создаст архивы (tar.gz, zip)
3. Создаст установщик для Windows (exe)
4. Создаст архив с исходным кодом
5. Загрузит все файлы в GitHub Release

Процесс займет примерно 15-30 минут. Вы можете отслеживать прогресс в разделе "Actions" на GitHub.

## Ручная сборка и деплой

Если вы хотите собрать релиз вручную:

### Linux

1. Установите зависимости:
   ```bash
   sudo apt-get update
   sudo apt-get install -y build-essential cmake ninja-build \
     qt6-base-dev qt6-declarative-dev qt6-quickcontrols2-dev \
     libfluidsynth-dev git
   ```

2. Настройте vcpkg:
   ```bash
   make vcpkg-setup
   make vcpkg-install
   ```

3. Соберите проект:
   ```bash
   make
   ```

4. Установите в staging директорию:
   ```bash
   make install
   ```

5. Создайте архив с исходниками:
   ```bash
   git archive --format=tar.gz --prefix=pimino-1.0.0-src/ -o pimino-1.0.0-src.tar.gz HEAD
   ```

6. Создайте релизные архивы:
   ```bash
   bash .github/workflows/build-packages.sh 1.0.0 linux
   ```

### Windows

1. Установите зависимости:
   - Visual Studio 2022 (с компонентами C++ и CMake)
   - Qt 6.8.0 (или используйте Qt через vcpkg)
   - Git

2. Настройте vcpkg:
   ```cmd
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg install fluidsynth:x64-windows
   ```

3. Соберите проект:
   ```cmd
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg\scripts\buildsystems\vcpkg.cmake
   cmake --build build --config Release
   ```

4. Установите в staging директорию:
   ```cmd
   cmake --install build --prefix staging
   ```

5. Скопируйте Qt зависимости (см. workflow для деталей)

6. Создайте установщик (требуется NSIS):
   ```cmd
   makensis /DPRODUCT_VERSION=1.0.0 /DSTAGING_DIR=staging installer.nsi
   ```

## Структура релиза

Каждый релиз содержит:

1. **Исходный код**:
   - `pimino-{version}-src.tar.gz` (Linux/Mac)
   - `pimino-{version}-src.zip` (Windows)

2. **Готовые бинарники Linux**:
   - `pimino-{version}-linux-x64.tar.gz`
   - `pimino-{version}-linux-x64.zip`

3. **Готовые бинарники Windows**:
   - `pimino-{version}-windows-x64.zip`
   - `pimino-{version}-setup.exe` (установщик)

## Использование релизов

### Linux

1. Скачайте архив `pimino-{version}-linux-x64.tar.gz`
2. Распакуйте:
   ```bash
   tar -xzf pimino-{version}-linux-x64.tar.gz
   cd pimino-{version}-linux-x64
   ```
3. Запустите:
   ```bash
   ./run.sh
   ```

### Windows

1. Скачайте `pimino-{version}-setup.exe` для установки
   ИЛИ
2. Скачайте `pimino-{version}-windows-x64.zip` для портативной версии
3. Распакуйте и запустите `run.bat` или `bin\pimino.exe`

### Сборка из исходников

1. Скачайте архив с исходниками
2. Распакуйте:
   ```bash
   tar -xzf pimino-{version}-src.tar.gz
   cd pimino-{version}-src
   ```
3. Следуйте инструкциям в `Makefile`:
   ```bash
   make help
   make vcpkg-setup
   make vcpkg-install
   make
   ```

## Troubleshooting

### Проблемы с зависимостями Qt

Если приложение не запускается из-за отсутствующих Qt библиотек:
- Linux: Установите Qt6 runtime пакеты через пакетный менеджер
- Windows: Убедитесь, что все Qt DLL скопированы в директорию с исполняемым файлом

### Проблемы с FluidSynth

Если возникают ошибки с FluidSynth:
- Убедитесь, что библиотека установлена и доступна
- Проверьте переменные окружения для vcpkg

### Проблемы со сборкой

- Убедитесь, что все зависимости установлены
- Проверьте версию CMake (требуется 3.16+)
- Проверьте версию Qt (требуется 6.8+)

## Дополнительная информация

- GitHub Actions workflow: `.github/workflows/release.yml`
- Скрипт сборки пакетов: `.github/workflows/build-packages.sh`
- Makefile для сборки: `Makefile`
- NSIS скрипт установщика: `installer.nsi`

