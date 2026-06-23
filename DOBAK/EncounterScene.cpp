#include "EncounterScene.h"

void EncounterScene::Init(GameState& state)
{
    system("cls");

    rewardApplied = false;
    goldReward = 0;
    rewardItem = Item{};

    RollEncounter(state);
    ApplyReward(state);
}

void EncounterScene::Update(GameState& state)
{
    if (GetKeyDown(VK_SPACE) || GetKeyDown(VK_RETURN))
    {
        SceneManager::GetInst()->ChangeScene("ShopScene", state);
    }
}

void EncounterScene::Render(const GameState& state)
{
    COORD res = GetConsoleResolution();
    int cx = res.X / 2;
    int cy = res.Y / 2;

    GotoXY(cx - 20, cy - 6);
    SetColor(Color::LIGHT_VIOLET);
    cout << "+--------------------------------------+";

    GotoXY(cx - 20, cy - 5);
    cout << "|            만남 이벤트               |";

    GotoXY(cx - 20, cy - 4);
    cout << "+--------------------------------------+";

    GotoXY(cx - 18, cy - 2);
    SetColor(Color::LIGHT_YELLOW);
    cout << title;

    GotoXY(cx - 18, cy);
    SetColor(Color::WHITE);
    cout << desc;

    GotoXY(cx - 18, cy + 3);
    SetColor(Color::LIGHT_GRAY);
    cout << "[SPACE / ENTER] 상점으로";

    SetColor();
}

void EncounterScene::RollEncounter(GameState& state)
{
    int roll = rand() % 100;

    if (roll < 50)
    {
        encounterType = EncounterType::Gold;

        goldReward = 10 + rand() % 91; // 10~100G
        title = "수상한 행인이 동전을 건넸다.";
        desc = "+" + std::to_string(goldReward) + "G 획득";
    }
    else if (roll < 80)
    {
        encounterType = EncounterType::Item;

        if (!state.shopItems.empty())
        {
            int index = rand() % state.shopItems.size();
            rewardItem = state.shopItems[index];

            title = "떠돌이 상인이 물건을 두고 갔다.";
            desc = rewardItem.name + " 획득";
        }
        else
        {
            encounterType = EncounterType::Gold;
            goldReward = 30;
            title = "낡은 주머니를 발견했다.";
            desc = "+30G 획득";
        }
    }
    else
    {
        encounterType = EncounterType::Show;

        title = "기묘한 시선이 느껴진다.";
        desc = "아무 일도 일어나지 않았다.";
    }
}

void EncounterScene::ApplyReward(GameState& state)
{
    if (rewardApplied)
        return;

    rewardApplied = true;

    switch (encounterType)
    {
    case EncounterType::Gold:
        state.player.gold += goldReward;
        break;

    case EncounterType::Item:
        state.player.inventory.push_back(rewardItem);
        break;

    case EncounterType::Show:
        break;
    }
}