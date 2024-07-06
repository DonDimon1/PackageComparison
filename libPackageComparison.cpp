#include "libPackageComparison.hpp"
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <unordered_set>

using json = nlohmann::json;

// Структура нужной нам информации из json ответа сервера
struct PackageInfo {
    std::string name;
    std::string version;
    std::string release;
};

size_t writeCallback(void *ptr, size_t size, size_t nmemb, std::string *data) {
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}

std::vector<PackageInfo> fetchPackages(const std::string &branch, const std::string & arch) {
    std::string url = "https://rdb.altlinux.org/api/export/branch_binary_packages/" + branch + "?arch=" + arch;
    std::string response;

    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Добавление SSL версии
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return {};
        }

        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize curl." << std::endl;
        return {};
    }

    // Парсинг JSON
    std::vector<PackageInfo> packages;
    try {
        auto json_response = json::parse(response); // Парсим JSON

        if (json_response.contains("packages") && json_response["packages"].is_array()) {
            // Извлекаем массив пакетов
            auto packages_array = json_response["packages"];

            for (auto& package_obj : packages_array) {
                PackageInfo packageStruct;
                packageStruct.name = package_obj["name"];
                packageStruct.version = package_obj["version"];
                packageStruct.release = package_obj["release"];
                packages.push_back(packageStruct);
            }
        } else {
            std::cerr << "Invalid JSON format: 'packages' array not found or is not an array." << std::endl;
            return {};
        }
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return {};
    }

    return packages;
}

ComparisonResult compareBranches(const std::string &branch1, const std::string &branch2, const std::string& arch) {
    ComparisonResult result;

    // Получаем списки пакетов для двух веток
    // Поскольку порядок нахождения пакетов в коллекции нам не важен(ведь мы их все переберём), можем воспользоваться unordered_set вместо обычного vector
    // для увеличения скорости поиска при сравнении
    std::unordered_set<PackageInfo> packages1 = fetchPackages(branch1, arch);
    std::vector<PackageInfo> packages1 = fetchPackages(branch1, arch);
    std::vector<PackageInfo> packages2 = fetchPackages(branch2, arch);
    std::cout << "packages1.size(): " << packages1.size() << std::endl;
    std::cout << "packages2.size(): " << packages2.size() << std::endl;

    // Создаем set пакетов для быстрого поиска
//    std::set<PackageInfo> setPackages1(packages1.begin(), packages1.end());
//    std::set<PackageInfo> setPackages2(packages2.begin(), packages2.end());

//    std::cout << "setPackages1.size(): " << setPackages1.size() <<std::endl;
//    std::cout << "setPackages2.size(): " << setPackages2.size() <<std::endl;

//    // Находим пакеты, которые есть только в первом списке
//    for (const auto &package : packages1) {
//        if (setPackages2.find(package) == setPackages2.end()) {
//            result.onlyInBranch1.push_back(package);
//        }
//    }

//    // Находим пакеты, которые есть только во втором списке
//    for (const auto &package : packages2) {
//        if (setPackages1.find(package) == setPackages1.end()) {
//            result.onlyInBranch2.push_back(package);
//        }
//    }

//    // Находим пакеты, у которых версия-релиз больше в первом списке
//    for (const auto &package1 : packages1) {
//        for (const auto &package2 : packages2) {
//            if(package1 > package2)
//                result.greaterInBranch1.push_back(package1);
//        }
//    }

    return result;
}

std::string toJsonString(const ComparisonResult &result) {
    json jsonResult;
    jsonResult["onlyInBranch1"] = result.onlyInBranch1;
    jsonResult["onlyInBranch2"] = result.onlyInBranch2;
    jsonResult["greaterInBranch1"] = result.greaterInBranch1;
    return jsonResult.dump(4);
}

