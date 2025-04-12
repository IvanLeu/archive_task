# Compressor

Compressor - приложение для архивации и распоковки одного файла.
---

## Возможности

- Сжатие одного файла в архив
- Распаковка архива в файл
- Кроссплатформенность (Linux и Windows)

---

## Сборка

### Зависимости

- **C++ компилятор**, поддерживающий C++20
- **CMake** версии 3.12 или выше
- **zlib**

---

### Инструкция по сборке

1. Склонируйте репозиторий или скачайте исходный код:

   ```bash
   git clone https://github.com/IvanLeu/archive_task.git
   cd archive_task
   ```

2. Установите `zlib`, если он ещё не установлен:

   - **Linux (APT-пакет):**

     ```bash
     sudo apt-get install zlib1g-dev
     ```

   - **Windows:**  
     Скачайте и соберите zlib вручную, или используйте [vcpkg](https://github.com/microsoft/vcpkg):

     ```bash
     vcpkg install zlib
     ```

3. Создайте папку для сборки:

   ```bash
   mkdir build
   cd build
   ```

4. Сгенерируйте файлы сборки CMake:

   - **Linux/macOS:**

     ```bash
     cmake ..
     ```

   - **Windows (с vcpkg):**

     ```bash
     cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
     ```

5. Соберите проект:

   - **Linux/macOS:**

     ```bash
     cmake --build .
     ```

   - **Windows (Visual Studio):**
     
     Откройте `Compressor.sln` в Visual Studio и соберите проект, либо:

     ```bash
     cmake --build . --config Release
     ```

---

## Использование

После сборки приложение будет доступно как исполняемый файл `Compressor`.

```bash
Compressor <команда> <входной_файл> <выходной_файл>
```

### Команды:

- `a` — архивировать файл
- `e` — распаковать файл

### Примеры:

- Архивировать:

  ```bash
  ./Compressor a original.txt archive.dat
  ```

- Распаковать:

  ```bash
  ./Compressor e archive.dat restored.txt
  ```

---

## Прерывание операций

Во время архивирования или распаковки можно нажать **Ctrl+C**, чтобы прервать процесс.

---
