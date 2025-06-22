#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

class FileHandler {
public:
    std::vector<std::vector<std::string>> load(const std::string& filename) {
        std::vector<std::vector<std::string>> data;
        std::ifstream file(filename);
        if (!file.is_open()) {
            // 파일을 못 여는 건 심각한 에러가 아닐 수 있음 (orders.csv 처럼 처음엔 없을 수 있으니)
            // std::cerr << "Warning: " << filename << " 파일을 열 수 없습니다." << std::endl;
            return data;
        }
        std::string line;
        while (std::getline(file, line)) {
            // 윈도우에서 만든 CSV 파일은 끝에 \r이 붙는 경우가 있어서 제거해줌
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            if (line.empty()) continue; // 빈 줄은 건너뛰기

            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string cell;
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            data.push_back(row);
        }
        file.close();
        return data;
    }

    void save(const std::string& filename, const std::vector<std::vector<std::string>>& data) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: " << filename << " 파일에 저장할 수 없습니다." << std::endl;
            return;
        }
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                if (i < row.size() - 1) file << ",";
            }
            file << "\n";
        }
        file.close();
    }
};