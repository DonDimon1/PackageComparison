#!/bin/bash

# Переменная с именем исполняемого файла
EXECUTABLE="PackageComparison"

# Переходим в директорию проекта
cd "$PWD" || exit

# Создаем директорию для сборки, если её нет
mkdir -p build
cd build || exit

# Запускаем cmake для генерации файлов сборки
cmake ..

# Проверяем успешность выполнения cmake
if [ $? -ne 0 ]; then
    echo "Error while executing cmake. Check the output above."
    exit 1
fi

# Собираем приложение с помощью make
make

# Проверяем успешность компиляции
if [ $? -ne 0 ]; then
    echo "Error during compilation. Check the output above."
    exit 1
fi

echo -e "\n"
echo -e "Running program\n"

# Запускаем исполняемый файл
"./${EXECUTABLE}"

# Проверяем успешность запуска
if [ $? -ne 0 ]; then
    echo "Error while running the executable file. Check the output above."
    exit 1
fi

# Переходим к JSON файлу
cd "$PWD" || exit
RESULT_FILE="comparisonResult.json"

# Проверяем, существует ли файл
if [ -f "$RESULT_FILE" ]; then
    echo "Open JSON file: $RESULT_FILE"
    # Открываем файл в соответствующем приложении по умолчанию
    xdg-open "$RESULT_FILE" || open "$RESULT_FILE"
else
    echo "File $RESULT_FILE not found."
fi

echo -e "\nProcess completed successfully. Exit."

# Возвращаемся в исходную директорию
cd "$PWD" || exit

