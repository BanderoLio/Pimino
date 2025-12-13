# Инструкция по деплою на GitHub Releases

## Быстрый старт

### Автоматический деплой (рекомендуется)

1. **Подготовьте изменения:**
   ```bash
   git add .
   git commit -m "Описание изменений"
   git push origin main
   ```

2. **Создайте тег релиза:**
   ```bash
   git tag -a v1.0.0 -m "Release version 1.0.0"
   git push origin v1.0.0
   ```

3. **GitHub Actions автоматически:**
   - Соберет проект для Linux и Windows
   - Создаст архивы (tar.gz, zip)
   - Создаст установщик для Windows (exe)
   - Создаст архив с исходным кодом
   - Загрузит все в GitHub Release

Процесс займет 15-30 минут. Отслеживайте прогресс в разделе "Actions" на GitHub.

## Что будет создано

После успешного деплоя в релизе будут доступны:

### Для пользователей Linux:
- `pimino-{version}-linux-x64.tar.gz` - архив для Linux
- `pimino-{version}-linux-x64.zip` - ZIP архив для Linux

### Для пользователей Windows:
- `pimino-{version}-windows-x64.zip` - портативная версия
- `pimino-{version}-setup.exe` - установщик Windows

### Для разработчиков:
- `pimino-{version}-src.tar.gz` - исходный код (Linux/Mac)
- `pimino-{version}-src.zip` - исходный код (Windows)

## Структура архивов

### Linux архив содержит:
```
pimino-{version}-linux-x64/
├── bin/
│   └── pimino              # Исполняемый файл
├── lib/                    # Библиотеки (если есть)
├── qml/                    # QML модули Qt
├── plugins/                # Qt плагины
├── run.sh                  # Скрипт запуска
└── README.txt              # Инструкция
```

### Windows архив содержит:
```
pimino-{version}-windows-x64/
├── bin/
│   └── pimino.exe          # Исполняемый файл
├── qml/                    # QML модули Qt
├── plugins/                # Qt плагины
├── run.bat                 # Скрипт запуска
└── README.txt              # Инструкция
```

### Архив с исходниками содержит:
```
pimino-{version}-src/
├── src/                    # Исходный код
├── CMakeLists.txt          # CMake конфигурация
├── Makefile                # Упрощенная сборка
├── vcpkg-configuration.json
├── installer.nsi           # Скрипт установщика Windows
└── README.md               # Документация
```

## Ручной деплой

Если нужно создать релиз вручную:

### Linux

```bash
# 1. Соберите проект
make vcpkg-setup
make vcpkg-install
make
make install

# 2. Создайте архив с исходниками
git archive --format=tar.gz --prefix=pimino-1.0.0-src/ -o pimino-1.0.0-src.tar.gz HEAD

# 3. Создайте релизные архивы
bash .github/workflows/build-packages.sh 1.0.0 linux

# 4. Загрузите файлы в GitHub Release вручную
```

### Windows

```cmd
REM 1. Соберите проект
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build build --config Release
cmake --install build --prefix staging

REM 2. Скопируйте Qt зависимости (см. workflow)

REM 3. Создайте установщик (требуется NSIS)
makensis /DPRODUCT_VERSION=1.0.0 /DSTAGING_DIR=staging installer.nsi

REM 4. Создайте ZIP архив
powershell Compress-Archive -Path staging\* -DestinationPath pimino-1.0.0-windows-x64.zip
```

## Использование релизов

### Linux

1. Скачайте `pimino-{version}-linux-x64.tar.gz`
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

**Вариант 1: Установщик (рекомендуется)**
1. Скачайте `pimino-{version}-setup.exe`
2. Запустите установщик
3. Следуйте инструкциям

**Вариант 2: Портативная версия**
1. Скачайте `pimino-{version}-windows-x64.zip`
2. Распакуйте в любую директорию
3. Запустите `run.bat` или `bin\pimino.exe`

### Сборка из исходников

1. Скачайте архив с исходниками
2. Распакуйте:
   ```bash
   tar -xzf pimino-{version}-src.tar.gz
   cd pimino-{version}-src
   ```
3. Следуйте инструкциям в `BUILD.md` или `Makefile`:
   ```bash
   make help
   make vcpkg-setup
   make vcpkg-install
   make
   ```

## Настройка GitHub Actions

Workflow автоматически запускается при:
- Создании тега вида `v*` (например, `v1.0.0`)
- Ручном запуске через "Actions" → "Build and Release" → "Run workflow"

### Требования

- Репозиторий должен быть на GitHub
- GitHub Actions должны быть включены (включены по умолчанию)
- Для приватных репозиториев может потребоваться настройка прав

## Troubleshooting

### Workflow не запускается

1. Проверьте, что тег создан правильно: `git tag -l`
2. Убедитесь, что тег запушен: `git push origin v1.0.0`
3. Проверьте логи в разделе "Actions"

### Ошибки сборки

1. Проверьте логи в GitHub Actions
2. Убедитесь, что все зависимости указаны в workflow
3. Проверьте версии Qt и CMake

### Проблемы с установщиком Windows

1. Убедитесь, что NSIS установлен (в workflow используется action)
2. Проверьте пути в `installer.nsi`
3. Проверьте логи сборки

### Архивы не создаются

1. Проверьте, что staging директория содержит все необходимые файлы
2. Убедитесь, что Qt зависимости скопированы
3. Проверьте права доступа к файлам

## Дополнительные файлы

- `.github/workflows/release.yml` - основной workflow
- `.github/workflows/build-packages.sh` - скрипт упаковки
- `installer.nsi` - скрипт установщика Windows
- `Makefile` - упрощенная сборка
- `RELEASE.md` - подробная документация (английский)
- `BUILD.md` - инструкция по сборке

## Поддержка

При возникновении проблем:
1. Проверьте логи GitHub Actions
2. Изучите документацию в `RELEASE.md` и `BUILD.md`
3. Создайте issue в репозитории

