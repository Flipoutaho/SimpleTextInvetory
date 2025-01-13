#pragma once
#include <iostream>
#include <vector>
#include <map>


using namespace std;

struct Slot
{
	int ID = 0;
	string name = "";
};

class Inventory
{
private: 
	Inventory();

public:
	static Inventory& GetInstance() {
		static Inventory instance;
		return instance;
	}

private:
	int m_curMaxSlot;		//현재 최대 슬롯수
	int m_limitMaxSlot;		//확장 가능한 최대 슬롯수
	int m_EquipmentSlot[2]; //장비 착용 슬롯
	vector<Slot> m_slot;	//
	string m_systemMsg;

public: 
	int ProcessInventory();//인벤토리 로직
	void MainMenuSelect();
	void PrintInventory(); //인벤토리 그리기
	void PrintSystemMsg(); //시스템 메시지 출력
	void PrintEquipment(); //착용중인 장비 그리기
public:
	void AddItem(int itemID); //아이템 획득
	void UseItem(char item); //아이템 사용, 장비 착용
	void SwapItem(char item1, char item2); //아이템 위치 변경
	void DeleteItem(char item); //아이템 버리기
	void ExpansionMaxSlot(); //슬롯 확장
	void TakeOffEquipment(int item); //장비 벗기	
};

