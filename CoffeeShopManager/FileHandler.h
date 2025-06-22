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
            // ������ �� ���� �� �ɰ��� ������ �ƴ� �� ���� (orders.csv ó�� ó���� ���� �� ������)
            // std::cerr << "Warning: " << filename << " ������ �� �� �����ϴ�." << std::endl;
            return data;
        }
        std::string line;
        while (std::getline(file, line)) {
            // �����쿡�� ���� CSV ������ ���� \r�� �ٴ� ��찡 �־ ��������
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            if (line.empty()) continue; // �� ���� �ǳʶٱ�

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
            std::cerr << "Error: " << filename << " ���Ͽ� ������ �� �����ϴ�." << std::endl;
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