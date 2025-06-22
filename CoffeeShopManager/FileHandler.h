#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class FileHandler {
public:
    // CSV ������ �� �پ� �о 2���� ����(���̺� ���)�� ������ִ� �Լ�
    // const std::string& filename: ������ �������� �ʰ� ���� ��θ� �״�� �����ؼ� ȿ������
    std::vector<std::vector<std::string>> load(const std::string& filename) {
        std::vector<std::vector<std::string>> data;
        std::ifstream file(filename); // ���� �б� ���� ����

        // ������ �� ������ ���� �޽��� ���� �� ������ ��ȯ
        if (!file.is_open()) {
            std::cerr << "Error: " << filename << " ������ �� �� �����ϴ�." << std::endl;
            return data;
        }

        std::string line;
        // ���� ������ �� �پ� �б�
        while (std::getline(file, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line); // �� ���� stringstream�� �־ ��ǥ(,)�� �ڸ��� ���� ��
            std::string cell;

            // ��ǥ(,)�� �������� �ܾ�(cell)�� �߶� row ���Ϳ� �߰�
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            data.push_back(row);
        }

        file.close(); // ���� �� ������ �� �ݾ��ֱ�
        return data;
    }

    // 2���� ���� �����͸� �޾Ƽ� CSV ���Ϸ� �����ϴ� �Լ�
    void save(const std::string& filename, const std::vector<std::vector<std::string>>& data) {
        std::ofstream file(filename); // ���� ���� ���� ���� (���� ���� �� ����� ���� ��)

        if (!file.is_open()) {
            std::cerr << "Error: " << filename << " ���Ͽ� ������ �� �����ϴ�." << std::endl;
            return;
        }

        // ������ �� �پ� ���鼭 ���Ͽ� ����
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                // ������ �ܾ �ƴϸ� ��ǥ �߰�
                if (i < row.size() - 1) {
                    file << ",";
                }
            }
            file << "\n"; // �� �� �� ������ �ٹٲ�
        }

        file.close();
    }
};