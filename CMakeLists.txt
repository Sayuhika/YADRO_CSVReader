# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.14)
if (CMAKE_VERSION VERSION_EQUAL "3.24" OR CMAKE_VERSION VERSION_GREATER "3.24")
    cmake_policy (SET CMP0135 NEW) 
    # При загрузке файлов из интернета через URL временная метка извлекаемых файлов из архива 
    # устанавливается равной времени извлечения, а не времени загрузки.
    # Устраняет Warning при конфигурации проекта.
endif()

project ("CSVReader")

# Включите подпроекты.
add_subdirectory ("CSVReader")
add_subdirectory ("CSVReaderTest")
