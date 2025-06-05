#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class FileHandler {
public:
    // CSV 파일을 한 줄씩 읽어서 2차원 벡터(테이블 모양)로 만들어주는 함수
    // const std::string& filename: 파일을 복사하지 않고 원본 경로를 그대로 참조해서 효율적임
    std::vector<std::vector<std::string>> load(const std::string& filename) {
        std::vector<std::vector<std::string>> data;
        std::ifstream file(filename); // 파일 읽기 모드로 열기

        // 파일이 안 열리면 에러 메시지 띄우고 빈 데이터 반환
        if (!file.is_open()) {
            std::cerr << "Error: " << filename << " 파일을 열 수 없습니다." << std::endl;
            return data;
        }

        std::string line;
        // 파일 끝까지 한 줄씩 읽기
        while (std::getline(file, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line); // 한 줄을 stringstream에 넣어서 쉼표(,)로 자르기 쉽게 함
            std::string cell;

            // 쉼표(,)를 기준으로 단어(cell)를 잘라서 row 벡터에 추가
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            data.push_back(row);
        }

        file.close(); // 파일 다 썼으면 꼭 닫아주기
        return data;
    }

    // 2차원 벡터 데이터를 받아서 CSV 파일로 저장하는 함수
    void save(const std::string& filename, const std::vector<std::vector<std::string>>& data) {
        std::ofstream file(filename); // 파일 쓰기 모드로 열기 (기존 내용 싹 지우고 새로 씀)

        if (!file.is_open()) {
            std::cerr << "Error: " << filename << " 파일에 저장할 수 없습니다." << std::endl;
            return;
        }

        // 데이터 한 줄씩 돌면서 파일에 쓰기
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                // 마지막 단어가 아니면 쉼표 추가
                if (i < row.size() - 1) {
                    file << ",";
                }
            }
            file << "\n"; // 한 줄 다 썼으면 줄바꿈
        }

        file.close();
    }
};