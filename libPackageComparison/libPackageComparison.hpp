#pragma once
#include <string>
#include <vector>

struct ComparisonResult {
    std::vector<std::string> onlyInBranch1;
    std::vector<std::string> onlyInBranch2;
    std::vector<std::string> greaterInBranch1;
};

ComparisonResult compareBranches(const std::string &branch1, const std::string &branch2, const std::string &arch);
std::string toJsonString(const ComparisonResult &result);
