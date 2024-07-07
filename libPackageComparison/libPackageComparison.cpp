#include "libPackageComparison.hpp"
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <unordered_set>

using json = nlohmann::json;

// Структура для отбора нужной нам информации из json ответа сервера
struct PackageInfo {
    std::string name;
    std::string version;
    std::string release;

    // Конструктор по умолчанию
    PackageInfo() = default;
    // Конструктор с параметрами
    PackageInfo(const std::string& n, const std::string& v, const std::string& r)
        : name(n), version(v), release(r) {}

    // Переопределение оператора сравнения для использования в unordered_set
    bool operator==(const PackageInfo& other) const {
        return name == other.name;// && version == other.version && release == other.release;
    }   // Только по имени, чтобы find работал корректно
};

// Хэш-функция для PackageInfo, необходимая для использования в unordered_set
template<>
struct std::hash<PackageInfo> {
    std::size_t operator()(const PackageInfo& p) const {
        return std::hash<std::string>{}(p.name);// ^ (std::hash<std::string>{}(p.version) << 1) ^ (std::hash<std::string>{}(p.release) << 2);
    }
};

// Запись ответа от сервера
size_t writeCallback(void *ptr, size_t size, size_t nmemb, std::string *data) {
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}

// Получение пакетов
std::unordered_set<PackageInfo> fetchPackages(const std::string &branch, const std::string & arch) {
    std::string url = "https://rdb.altlinux.org/api/export/branch_binary_packages/" + branch + "?arch=" + arch;
    std::string response;

    std::cout << "Query the server" << std::endl;

    CURL *curl = curl_easy_init();                                          // Инициализируем сессию libcurl
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());                   // Устанавливаем URL для запроса
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);       // Устанавливаем функцию записи тела ответа сервера
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Добавление SSL версии
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        //
        long response_code;
        CURLcode res = curl_easy_perform(curl);                             // Выполняем запрос
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);    // Получаем код ответа сервера
        std::cerr << "Response code: " << response_code << ", ";

        switch (response_code) {                                            // Выводим описание кода
           case 200:
               std::cout << "Success" << std::endl;
               break;
           case 400:
               std::cout << "Request parameters validation error" << std::endl;
               break;
           case 404:
               std::cout << "Requested data not found in database" << std::endl;
               break;
           default:
               std::cout << "Some kind of mistake." << std::endl;
               break;
           }

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            exit(1);
        }

        curl_easy_cleanup(curl);  // Освобождаем ресурсы
    } else {
        std::cerr << "Failed to initialize curl." << std::endl;
        exit(1);
    }

    std::cout << "parse the json" << std::endl;

    // Парсинг JSON
    std::unordered_set<PackageInfo> packages;
    try {
        auto json_response = json::parse(response); // Парсим JSON

        if (json_response.contains("packages") && json_response["packages"].is_array()) { // Если json содержит packages
            // Извлекаем массив пакетов
            auto packages_array = json_response["packages"];

            for (auto& package_obj : packages_array) {
                PackageInfo packageStruct;
                packageStruct.name = package_obj["name"];
                packageStruct.version = package_obj["version"];
                packageStruct.release = package_obj["release"];
                packages.insert(packageStruct);
            }
        } else {
            std::cerr << "Invalid JSON format: 'packages' array not found or is not an array." << std::endl;
            exit(1);
        }
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        exit(1);
    }

    return packages;
}

ComparisonResult compareBranches(const std::string &branch1, const std::string &branch2, const std::string& arch) {
    ComparisonResult result;

    // Получаем списки пакетов для двух веток
    // Поскольку порядок нахождения пакетов в коллекции нам не важен(ведь мы их все переберём), можем воспользоваться unordered_set вместо обычного vector
    // для увеличения скорости поиска при сравнении

    std::unordered_set<PackageInfo> packages1 = fetchPackages(branch1, arch);   // Хранение пакетов
    std::unordered_set<PackageInfo> packages2 = fetchPackages(branch2, arch);

    std::cout << "Comparing packages" << std::endl;


    // Находим пакеты, которые есть только в первом списке
    for(const auto &package1 : packages1){
        if(packages2.find(package1) == packages2.end()) // Если find вернуло end(), значит пакета package в packages2 нет.
            result.onlyInBranch1.push_back(package1.name);
    }
    // Находим пакеты, которые есть только во втором списке
    for (const auto& package2 : packages2) {
        if (packages1.find(package2) == packages1.end())
            result.onlyInBranch2.push_back(package2.name);
    }

    // Находим пакеты, у которых версия-релиз больше в первом списке
        for (const auto& package1 : packages1) {
            auto it = packages2.find(package1); // Поиск пакета с таким же именем во второй коллекции
            if (it != packages2.end()) {        // Если найден
                const auto& package2 = *it;     // Получаем ссылку на найденный пакет package2 и сравниваем версии
                if (std::tie(package1.version, package1.release) > std::tie(package2.version, package2.release)) {
                    result.greaterInBranch1.push_back(package1.name);
                }
            }
        }

    return result;
}

// Финальный вывод в json формате
std::string toJsonString(const ComparisonResult &result) {
    json jsonResult;
    jsonResult["onlyInBranch1"] = result.onlyInBranch1;
    jsonResult["onlyInBranch2"] = result.onlyInBranch2;
    jsonResult["greaterInBranch1"] = result.greaterInBranch1;
    return jsonResult.dump(2);
}

