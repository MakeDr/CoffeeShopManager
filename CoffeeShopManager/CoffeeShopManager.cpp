#include <iostream>
#include "ShopManager.h"

int main() {
    // 이제부턴 모든 걸 사장님(ShopManager)을 통해서 처리함
    // getInstance()를 호출해서 유일한 사장님 객체를 불러옴
    ShopManager& manager = ShopManager::getInstance();

    std::cout << "프로그램 시작! 파일에서 데이터를 로드합니다..." << std::endl;
    // 이 한 줄이면 products.csv, members.csv에서 모든 정보를 읽어와서 객체로 싹 만들어줌
    manager.loadData();

    std::cout << "\n>> 데이터 로드 완료! <<\n";
    // 로드가 잘 됐는지 전체 데이터를 출력해서 확인해보자
    manager.displayAllData();

    // 프로그램이 종료되기 전에 데이터를 저장해야 하지만,
    // saveData()는 아직 구현 안 했으니 그냥 넘어감
    // manager.saveData(); 

    std::cout << "\n프로그램을 종료합니다." << std::endl;

    return 0;
}