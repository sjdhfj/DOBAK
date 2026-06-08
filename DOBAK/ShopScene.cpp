#include "ShopScene.h"
#include <vector>
constexpr int ShopX = 10;
constexpr int ShopY = 3;
constexpr int ListY = ShopY + 6;
constexpr int ListMax = 6;
constexpr float SellPricePercent = 0.5f;
constexpr int ArtX = 55;
constexpr int ArtY = 1;
std::vector<Item>& ShopScene::CurList()
{
    return (_curTab == ShopTab::BUY) ? _shopItems : _inventory;
}

//const std::vector<Item>& ShopScene::CurList() const
//{
//    return (_curTab == ShopTab::BUY) ? _shopItems : _inventory;
//}

int ShopScene::CurPrice(const Item& item) const
{
    return (_curTab == ShopTab::SELL) ? (int)(item.price * SellPricePercent) : item.price;
}

void ShopScene::Init(GameState& state)
{
    _curTab = ShopTab::BUY;
    _cursor = 0;
    _gold = 500;
    SetConsoleSize(WIDTH, HEIGHT);
    _shopItems = state.shopItems;

    _inventory =
    {
        { 6767, "676767676767", "676767", 6767 * 2, ItemType::EQUIP },
        { 67, "67676767", "676767",  67 * 2, ItemType::EQUIP },
    };
}

void ShopScene::Update(GameState& state)
{
    auto& list = CurList();
    int size = (int)list.size();

    if (GetKeyDown(VK_LEFT) && _curTab == ShopTab::SELL)
    {
        _curTab = ShopTab::BUY;
        _cursor = 0;
        system("cls");
        return;
    }
    if (GetKeyDown(VK_RIGHT) && _curTab == ShopTab::BUY)
    {
        _curTab = ShopTab::SELL;
        _cursor = 0;
        system("cls");
        return;
    }

    if (size > 0)
    {
        if (GetKeyDown(VK_UP))
            _cursor = std::max(0, _cursor - 1);
        if (GetKeyDown(VK_DOWN))
            _cursor = std::min(size - 1, _cursor + 1);
    }

    if (GetKeyDown(VK_RETURN))
    {
        if (size == 0) return;
        Item& item = list[_cursor];

        if (_curTab == ShopTab::BUY)
        {
            if (_gold >= item.price)
            {
                _gold -= item.price;
                _inventory.push_back(item);
                _isSmiling = true;
                _smileTimer = state.curTime;
            }
        }
        else
        {
            _gold += CurPrice(item);
            list.erase(list.begin() + _cursor);
            _cursor = std::min(_cursor, (int)list.size() - 1);
            _cursor = std::max(0, _cursor);
        }
    }

    if (GetKeyDown(VK_ESCAPE))
        SceneManager::GetInst()->ChangeScene("TitleScene", state);
    if (_isSmiling && state.curTime - _smileTimer > 1000)
        _isSmiling = false;
}

void ShopScene::Render(const GameState& state)
{
    const auto& list = CurList();

    GotoXY(ShopX, ShopY);
    SetColor(Color::LIGHT_YELLOW);
    cout << "상점";

    GotoXY(ShopX, ShopY + 2);
    SetColor(_curTab == ShopTab::BUY ? Color::WHITE : Color::GRAY);
    cout << (_curTab == ShopTab::BUY ? "> " : "  ") << "사기";
    cout << "    ";
    SetColor(_curTab == ShopTab::SELL ? Color::WHITE : Color::GRAY);
    cout << (_curTab == ShopTab::SELL ? "> " : "  ") << "팔기";

    GotoXY(ShopX, ShopY + 4);
    SetColor();
    DrawLine('-', 35);

    for (int i = 0; i < ListMax; ++i)
    {
        GotoXY(ShopX, ListY + i);
        if (i < (int)list.size())
        {
            bool sel = (i == _cursor);
            SetColor(sel ? Color::WHITE : Color::LIGHT_GRAY);
            cout << (sel ? "> " : "  ");
            cout << std::left << std::setw(20) << list[i].name;
            cout << std::right << std::setw(6) << CurPrice(list[i]) << "G";
        }
        else
        {
            SetColor();
            cout << std::setw(30) << "";
        }
    }

    GotoXY(ShopX, ListY + ListMax + 1);
    SetColor();
    DrawLine('-', 35);

    GotoXY(ShopX, ListY + ListMax + 2);
    SetColor(Color::CYAN);
    if (!list.empty())
        cout << std::left << std::setw(35) << list[_cursor].description;
    else
        cout << std::setw(35) << "";

    GotoXY(ShopX, ListY + ListMax + 4);
    SetColor(Color::LIGHT_YELLOW);
    cout << "Gold: " << std::left << std::setw(10) << _gold << "G";

    SetColor();

    const std::vector<string> noSmileman =
    {
        R"(      ////^\\\\       )",
        R"(      | ^   ^ |       )",
        R"(     @ (o) (o) @      )",
        R"(      |   <   |       )",
        R"(      |  ___  |       )",
        R"(       \_____/        )",
        R"(     ____|  |____     )",
        R"(    /    \__/    \    )",
        R"(   /              \   )",
        R"(  /\_/|        |\_/\  )",
        R"( / /  |        |  \ \ )",
        R"(( <   |        |   > ))",
        R"( \ \  |        |  / / )",
        R"(  \ \ |________| / /  )",
        R"(   \ \|<I_D_I__|/ /   )",
        R"(    \ \ / I  \ / /    )",
        R"(     \ /  I   \ /     )",
        R"(     |         |      )",
        R"(     |    |    |      )",
        R"(     |    |    |      )",
        R"(     |    |    |      )",
        R"(     |    |    |      )",
        R"(     | ## | ## |      )",
        R"(     |    |    |      )",
        R"(     |    |    |      )",
        R"(     |____|____|      )",
        R"(     (____(____)      )",
        R"(      _| | _| |       )",
        R"(  cccC__Cccc___)      )"
    };
    const std::vector<string> Smileman =
    {
        R"(      ////^\\\\       )",
        R"(      | ^   ^ |       )",
        R"(     @ (O) (O) @      )",
        R"(      |   <   |       )",
        R"(      |  \_/  |       )",
        R"(       \_____/        )",
        R"(     ____|  |____     )",
        R"(    /    \__/    \    )",
        R"(   /              \   )",
        R"(  /\_/|        |\_/\  )",
        R"( / /  |        |  \ \ )",
        R"(( <   |        |   > ))",
        R"( \ \  |        |  / / )",
        R"(  \ \ |________| / /  )",
        R"(   \ \|<I_D_I__|/ /   )",
        R"(    \ \ / I  \ / /    )",
        R"(     \ /  I   \ /     )",
        R"(     |         |      )",
        R"(     |    |    |      )",
        R"(     |    |    |      )",
        R"(     |    |    |      )",
        R"(     |    |    |      )",
        R"(     | ## | ## |      )",
        R"(     |    |    |      )",
        R"(     |    |    |      )",
        R"(     |____|____|      )",
        R"(     (____(____)      )",
        R"(      _| | _| |       )",
        R"(  cccC__Cccc___)      )"
    };
    int y = ArtY;
    const auto& man = _isSmiling ? Smileman : noSmileman;
    SetColor();
    for (auto& i : man)
    {
        GotoXY(ArtX, y++);
        cout << i;
    }
}