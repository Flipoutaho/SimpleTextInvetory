#include "Inventory.h"
#include <windows.h>
#include <string>

map<int, string> map_ItemDB = { { 0,     "없음"},
								{ 10001, "포션" },
								{ 10002, "고급 포션" },
								{ 10003, "최고급 포션" },

								{ 20001, "기본칼" },
								{ 20002, "고급 칼" },
								{ 20003, "최고급 칼" },

								{ 30001, "기본 갑옷" },
								{ 30002, "튼튼한 갑옷" },
								{ 30003, "아주 튼튼한 갑옷" }
};

Inventory::Inventory()
{
	m_curMaxSlot = 10;
	m_limitMaxSlot = 16;
	m_systemMsg = "";
	memset(m_EquipmentSlot, 0, sizeof(int)*2); 
}

int Inventory::ProcessInventory()
{
	int menu = -1;
	while (1)
	{
		PrintInventory();
		PrintSystemMsg();
		PrintEquipment();
		printf("메뉴를 입력후 엔터\n");
		printf("1. 아이템 획득 \n2. 아이템 사용 \n3. 아이템 위치 변경 \n4. 아이템 버리기 \
		\n5. 슬롯 확장\n6. 장비해제\n7. 종료 \n");
		scanf_s("%d", &menu);
		system("cls");
		switch (menu)
		{
		case 1: //아이템 획득
			printf("[아이템 획득] 메뉴를 입력\n");
			printf("1. 임의의 아이템 랜덤 획득 \n2. 특정 아이템 획득\n ");
			scanf_s("%d", &menu);
			switch (menu)
			{
			case 1:
				printf("[아이템 획득 > 랜덤 획득] ID를 입력\n");
				AddItem(((rand()%3)+1) * 10000 + (rand()%3)+1);
				break;
			case 2:
				printf("[아이템 획득 > 특정 아이템 획득] ID를 입력\n");
				int tempID = 10001;
				scanf_s("%d", &tempID);
				AddItem(tempID);
				break;
			}
			break;
		case 2: //아이템 사용
			PrintInventory();
			printf("[아이템 사용] 메뉴를 입력(a-p)\n");
			cin.ignore();
			UseItem(getchar());
			break;

		case 3: // 아이템 위치 변경
			PrintInventory();
			printf("[아이템 위치 변경] 원하는 아이템 두개 입력(a-p, a-p)\n");
			char tempC[3];
			cin.ignore();
			cin >> tempC[0] >> tempC[1] >> tempC[2];
			SwapItem(tempC[0], tempC[2]);
			break;

		case 4: //아이템 버리기
			PrintInventory();
			printf("[아이템 버리기] 메뉴를 입력(a-p)\n");
			cin.ignore();
			DeleteItem(getchar());
			break;
		case 5:
			ExpansionMaxSlot();
			break;
		case 6:
			printf("[장비 벗기] 메뉴를 입력(1.모두 벗기 2. 무기 3. 갑옷)\n");
			scanf_s("%d", &menu);
			TakeOffEquipment(menu);
			break;
		case 7:
			return 0; // 루프 종료
			break;
		}

		
	}
}
void Inventory::MainMenuSelect()
{

}
void Inventory::PrintInventory()
{
	system("cls");
	printf("◎◎◎◎◎◎◎◎ INVENTORY◎◎◎◎◎◎◎◎\n");
	for (int i = 0; i < m_limitMaxSlot; ++i)
	{
		if( m_slot.size() > i )
			printf("◎%10c. : %-23s ◎\n", i+97, m_slot[i].name.c_str());
		else
			printf("◎\t\t\t\t\t◎\n");
	}
	printf("◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎◎\n\n");
	
}
void Inventory::PrintSystemMsg()
{
	COORD backupPos = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO buf;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buf);
	backupPos.X = buf.dwCursorPosition.X;
	backupPos.Y = buf.dwCursorPosition.Y;
	
	COORD setPos = { 50, 8 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), setPos);
	printf("[시스템 메시지]");

	setPos = { 50, 9 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), setPos);
	printf(">%s", m_systemMsg.c_str() );
	m_systemMsg = "";

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), backupPos);
}
void Inventory::PrintEquipment()
{
	COORD backupPos = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO buf;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buf);
	backupPos.X = buf.dwCursorPosition.X;
	backupPos.Y = buf.dwCursorPosition.Y;

	COORD setPos = { 50, 0 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), setPos);
	printf("[착용중인 장비]");

	setPos = { 50, 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), setPos);
	printf("무기 : %s", map_ItemDB[m_EquipmentSlot[0]].c_str());

	setPos = { 50, 2 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), setPos);
	printf("장비 : %s", map_ItemDB[m_EquipmentSlot[1]].c_str());

	
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), backupPos);
}
void Inventory::AddItem(int itemID)
{
	if (m_slot.size() >= m_curMaxSlot)
	{
		m_systemMsg = "[아이템 추가 실패] 아이템이 가득 찼습니다";
		return;
	}
	if ((itemID < 10001 || itemID > 10003) && \
		(itemID < 20001 || itemID > 20003) && \
		(itemID < 30001 || itemID > 30003))
	{
		m_systemMsg = "[아이템 추가 실패] 존재하지 않는 아이템입니다.";
		return;
	}
	Slot temp;
	temp.ID = itemID;
	temp.name = map_ItemDB[itemID];
	m_slot.push_back(temp);
	m_systemMsg = "아이템{" + temp.name + "}을 획득했습니다.";
}

void Inventory::UseItem(char item)
{
	if (m_slot.empty()) //아이템이 없을때
	{
		m_systemMsg = "[아이템 사용 실패] 사용 가능한 아이템이 없습니다.";
		return;
	}
	if (item < 97 && item > 112) // a-p 범위외 입력 시
	{
		m_systemMsg = "[아이템 사용 실패] a~p 사이로 입력해주십시오";
		return;
	}
	if (m_slot.size() <= item - 97) //가지고 있지 않은 아이템 슬롯 시도
	{
		m_systemMsg = "[아이템 사용 실패] 존재하는 아이템을 입력해주십시오";
		return;
	}

	int whichItem = item - 97;
	if (m_slot[whichItem].ID / 10000 == 1)
	{
		m_systemMsg = "[아이템 사용 성공] {" + m_slot[whichItem].name + "}을 사용 했습니다";
		m_slot.erase(m_slot.begin() + whichItem);
	}
	else if (m_slot[whichItem].ID / 10000 == 2 )
	{
		if (m_EquipmentSlot[0] != 0) //이미 착용중인 무기가 있다면
		{
			m_systemMsg = "[아이템 사용 성공] {" + m_slot[whichItem].name + "}으로 교체했습니다.";
			int temp = m_slot[whichItem].ID;
			m_slot.erase(m_slot.begin() + whichItem);
			AddItem(m_EquipmentSlot[0]);
			m_EquipmentSlot[0] = temp;
			return;
		}
		else
		{
			m_systemMsg = "[아이템 사용 성공] {" + m_slot[whichItem].name + "}을 장착했습니다.";
			m_EquipmentSlot[0] = m_slot[whichItem].ID;
			m_slot.erase(m_slot.begin() + whichItem);
		}
	}
	else if (m_slot[whichItem].ID / 10000 == 3)
	{
		if (m_EquipmentSlot[1] != 0) //이미 착용중인 갑옷이 있다면
		{
			m_systemMsg = "[아이템 사용 성공] {" + m_slot[whichItem].name + "}으로 교체했습니다.";
			int temp = m_slot[whichItem].ID;
			m_slot.erase(m_slot.begin() + whichItem);
			AddItem(m_EquipmentSlot[1]);
			m_EquipmentSlot[1] = temp;
			return;
		}
		else
		{
			m_systemMsg = "[아이템 사용 성공] {" + m_slot[whichItem].name + "}을 장착했습니다.";
			m_EquipmentSlot[1] = m_slot[whichItem].ID;
			m_slot.erase(m_slot.begin() + whichItem);
		}
	}
	
}
void Inventory::SwapItem(char item1, char item2)
{
	if (m_slot.empty() || m_slot.size() == 1) //아이템이 없거나 한개만 있을때
	{
		m_systemMsg = "[아이템 변경 실패] 위치 변경 가능한 아이템이 부족합니다.";
		return;
	}
	if ((item1 < 97 && item1 > 112) || (item2 < 97 && item2 > 112)) // a-p 범위외 입력 시
	{
		m_systemMsg = "[아이템 변경 실패] a~p 사이로 입력해주십시오";
		return;
	}
	if (m_slot.size() <= item1 - 97 || m_slot.size() <= item2 - 97) //가지고 있지 않은 아이템 슬롯 시도
	{
		m_systemMsg = "[아이템 변경 실패] 존재하는 아이템을 입력해주십시오";
		return;
	}
	swap(m_slot[item1 - 97], m_slot[item2 - 97]);
	m_systemMsg = "[아이템 변경 성공] 위치 변경을 성공하였습니다.";
}

void Inventory::DeleteItem(char item)
{
	if (m_slot.empty()) //아이템이 없을때
	{
		m_systemMsg = "[아이템 버리기 실패] 버리기 가능한 아이템이 없습니다.";
		return;
	}
	if (item < 97 && item > 112) // a-p 범위외 입력 시
	{
		m_systemMsg = "[아이템 버리기 실패] a~p 사이로 입력해주십시오";
		return;
	}
	if (m_slot.size() <= item - 97) //가지고 있지 않은 아이템 슬롯 시도
	{
		m_systemMsg = "[아이템 버리기 실패] 존재하는 아이템을 입력해주십시오";
		return;
	}
	int whichItem = item - 97;
	m_systemMsg = "[아이템 버리기 성공] {" + m_slot[whichItem].name + "}을 버렸습니다";
	m_slot.erase(m_slot.begin() + whichItem);
}

void Inventory::ExpansionMaxSlot()
{
	if (m_curMaxSlot < m_limitMaxSlot)
	{
		m_curMaxSlot++;
		m_systemMsg = "[슬롯 확장 성공] 슬롯이 확장되었습니다(현재 : " + to_string(m_curMaxSlot) + " / 최대 : " + to_string(m_limitMaxSlot) + ")";
	}
	else
	{
		m_systemMsg = "[슬롯 확장 실패] 더 이상 확장할 수 없습니다";
	}
}
void Inventory::TakeOffEquipment(int item)
{
	if (m_EquipmentSlot[0] == 0 && m_EquipmentSlot[1] == 0 ) //아이템이 없을때
	{
		m_systemMsg = "[장비 벗기 실패] 착용한 장비가 없습니다.";
		return;
	}
	if (item < 0 && item > 2) // 1~3 범위외 입력 시
	{
		m_systemMsg = "[장비 벗기 실패] 1~3 사이로 입력해주십시오";
		return;
	}
	
	if (m_slot.size() >= m_curMaxSlot)
	{
		m_systemMsg = "[장비 벗기 실패] 아이템 창이 가득찼습니다";
		return;
	}
	if (item == 1)
	{
		if (m_slot.size() >= m_curMaxSlot - 1)
		{
			m_systemMsg = "[장비 벗기 실패] 아이템 창이 가득찼습니다";
			return;
		}
		if( m_EquipmentSlot[0] != 0)
			AddItem(m_EquipmentSlot[0]);
		if (m_EquipmentSlot[1] != 0)
			AddItem(m_EquipmentSlot[1]);
		m_systemMsg = "[장비 벗기 성공] 장비를 모두 벗었습니다";
		memset(m_EquipmentSlot, 0, sizeof(int) * 2);
		return;
	}
	else if (item == 2)
	{
		AddItem(m_EquipmentSlot[0]);
		m_systemMsg = "[장비 벗기 성공] 무기 {" + map_ItemDB[m_EquipmentSlot[0]] + "}를 벗었습니다";
		m_EquipmentSlot[0] = 0; 
		return;
	}
	else if (item == 3)
	{
		AddItem(m_EquipmentSlot[1]);
		m_systemMsg = "[장비 벗기 성공] 갑옷 {" + map_ItemDB[m_EquipmentSlot[1]] + "}를 벗었습니다";
		m_EquipmentSlot[1] = 0;
		return;
	}
}