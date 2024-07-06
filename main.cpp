#include "libPackageComparison.hpp"
#include <iostream>
#include <fstream>

// Коллекции всех опций
const int branchCount = 13;
const int archCount = 6;

const std::string branchArr[branchCount] = {"sisyphus", "sisyphus_e2k", "sisyphus_riscv64",
                                   "sisyphus_loongarch64", "p11", "p10", "p10_e2k",
                                   "p9", "p9_e2k", "p8", "c10f1", "c9f2", "c7"};
const std::string archArr[archCount] = {"noarch", "x86_64", "i586", "aarch64", "ppc64le", "x86_64-i586"}; // "armv7hf", "e2k", "mipsel", "riscv64"

//Функция взаимодействия с пользователем
bool Start(std::string &branch1, std::string &branch2, std::string &archStr){
    int firstBranch, secondBranch, archInt;
    // Выбор опции
    std::cout << "Select a two branches:\n\n" << " 1 - sisyphus\n 2 - sisyphus_e2k\n 3 - sisyphus_riscv64\n" <<
                 " 4 - sisyphus_loongarch64\n 5 - p11\n 6 - p10\n 7 - p10_e2k\n 8 - p9\n" <<
                 " 9 - p9_e2k\n 10 - p8\n 11 - c10f1\n 12 - c9f2\n 13 - c7" << std::endl;

    std::cout << "\nfirst: ";
    std::cin >> firstBranch;
    std::cout << "second: ";
    std::cin >> secondBranch;

    --firstBranch; --secondBranch;
    if(firstBranch < 0 || firstBranch >= branchCount){
        std::cout << "First branch entry error" << std::endl;
        return false;
    }
    if(secondBranch < 0 || secondBranch >= branchCount){
        std::cout << "Second branch entry error" << std::endl;
        return false;
    }

    std::cout << "\nSelect architecture:\n" << " 1 - noarch\n 2 - x86_64\n 3 - i586\n 4 - aarch64\n 5 - ppc64le\n 6 - x86_64-i586" << std::endl;
    std::cout << "\narchitecture: ";
    std::cin >> archInt;

    --archInt;
    if(archInt < 0 || archInt >= archCount){
        std::cout << "Architecture entry error" << std::endl;
        return false;
    }

    // Присваиваем значения
    branch1 = branchArr[firstBranch];
    branch2 = branchArr[secondBranch];
    archStr = archArr[archInt];
    std::cout << std::endl;

    return true;
}


int main() {
    std::string branch1; //= "sisyphus";
    std::string branch2; //= "p10";
    std::string archStr;

    // Получаем параметры от пользователя
    if(!Start(branch1, branch2, archStr))
        return 1;

    // Получаем результат сравнения пакетов для двух веток
    auto comparisonResult = compareBranches(branch1, branch2, archStr);

    // Выводим результат в формате JSON
    //std::cout << toJsonString(comparisonResult) << std::endl;
    std::string jsonOutput = toJsonString(comparisonResult);

    // Записываем результат в файл
    std::ofstream outputFile("comparisonResult.json");
    if (outputFile.is_open()) {
        outputFile << jsonOutput;
        outputFile.close();
        std::cout << "\nComparison result saved to comparison_result.json in the program build folder\n" << std::endl;
    } else {
        std::cerr << "\nFailed to open comparison_result.json for writing.\n" << std::endl;
    }
    return 0;
}
