#include "ShopScene.h"
#include <vector>
#include "UIAsciiObjs.h"
constexpr int ShopX = 10;
constexpr int ShopY = 3;
constexpr int ListY = ShopY + 6;
constexpr int ListMax = 6;
constexpr float SellPricePercent = 0.5f;
constexpr int ArtX = 55;

constexpr int ItemArtX = ShopX + 37;
constexpr int ItemArtY = ListY;               
constexpr int ItemArtWidth = ArtX - ItemArtX; 
constexpr int ItemArtHeight = ListMax;
static UIAsciiObjs objs;
std::vector<Item>& ShopScene::CurList(GameState& state)
{
    return (_curTab == ShopTab::BUY) ? _shopItems : state.player.inventory;
}

int ShopScene::CurPrice(const Item& item) const
{
    return (_curTab == ShopTab::SELL) ? (int)(item.price * SellPricePercent) : item.price;
}

void ShopScene::Init(GameState& state)
{
    AsciiInit(objs);
    _curTab = ShopTab::BUY;
    _cursor = 0;
    //SetConsoleSize(WIDTH, HEIGHT);
    _shopItems = state.shopItems;

    state.player.inventory =
    {
        { 6767, "676767676767", "676767", 6767 * 2, ItemType::EQUIP },
        { 67, "67676767", "676767",  67 * 2, ItemType::EQUIP },
    };
    PlayOpenTransition(state, 1);
}

void ShopScene::Update(GameState& state)
{
    auto& list = CurList(state);
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
            if (state.player.gold >= item.price)
            {
                ShakeConsoleWindow(20, 23, 1);
                state.player.gold -= item.price;
                state.player.inventory.push_back(item);
                _isSmiling = true;
                _smileTimer = state.curTime;
            }
        }
        else
        {
            state.player.gold += CurPrice(item);
            list.erase(list.begin() + _cursor);
            _isSelling = true;
            _juiceTimer = state.curTime;
            _cursor = std::min(_cursor, (int)list.size() - 1);
            _cursor = std::max(0, _cursor);
        }
    }

    if (GetKeyDown(VK_ESCAPE))
    {
        PlayCloseTransition(state, 1);
        SceneManager::GetInst()->ChangeScene("InGameScene", state);
    }
    if (_isSmiling && state.curTime - _smileTimer > 1000)
        _isSmiling = false;
    if (_isSelling && state.curTime - _juiceTimer > 1000)
        _isSelling = false;
    if (state.curTime - _sixSevenTimer > 100)
    {
        std::rotate(objs.shopSix.begin(), objs.shopSix.begin() + 1, objs.shopSix.end());
        std::rotate(objs.shopSeven.rbegin(), objs.shopSeven.rbegin() + 1, objs.shopSeven.rend());
        _sixSevenTimer = state.curTime;
    }
}

void ShopScene::Render(const GameState& state)
{
    COORD res = GetConsoleResolution();
    string esc = "[ESC]를 눌러 상점을 나갈 수 있습니다.";
    int x = (res.X - (int)esc.length()) / 2;
    SetColor();
    GotoXY(x, res.Y - 1);
    cout << esc;
    SetColor();
    auto noConststate = state;
    const auto& list = CurList(noConststate);

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
    if (!list.empty())
        RenderItemArt(list[_cursor]);
    GotoXY(ShopX, ListY + ListMax + 4);
    SetColor(Color::LIGHT_YELLOW);
    cout << "Gold: " << std::left << std::setw(10) << state.player.gold << "G";

    SetColor();
    
    int y = 1;
    const auto& man = _isSelling ? objs.shopjuicemen : (_isSmiling ? objs.shophappymen : objs.shopmen);
    SetColor();
    for (auto& i : man)
    {
        GotoXY(ArtX, y++);
        SetColor();
        cout << i;
    }
    //SetUniCodeMode();
    RenderSixSeven(state);
    ShakeConsoleWindow(100, 100, 100);
}
void ShopScene::RenderSixSeven(const GameState& state)
{
    COORD res = GetConsoleResolution();
    int x = res.X / 2 + 35;

    int sixWidth = 0;
    for (auto& i : objs.shopSix)
        sixWidth = std::max(sixWidth, (int)i.size());
    int sevenWidth = 0;
    for (auto& i : objs.shopSeven)
        sevenWidth = std::max(sevenWidth, (int)i.size());

    SetUniCodeMode();

    int y = 1;
    for (auto& i : objs.shopSix)
    {
        if (IsGotoXY(x, y++))
            wcout << i;
    }

    y = 1;
    for (auto& i : objs.shopSeven)
    {
        if (IsGotoXY(x + sixWidth, y++))
            wcout << i;
    }

    SetDefaultMode();
}
void ShopScene::PlayOpenTransition(const GameState& state, unsigned long  delaymilisecond)
{
    COORD res = GetConsoleResolution();
    int W = res.X;
    int H = res.Y;
    int half = W / 2;
    unsigned long delayMs = delaymilisecond;
    int colorCount = (int)Color::END;

    for (int row = 0; row < H; ++row)
    {
        Color c = (Color)std::min((int)(((float)row / H) * colorCount), colorCount - 1);
        SetColor(c, c);
        GotoXY(0, row);
        for (int x = 0; x < W; ++x)
            cout << " ";
        Sleep(delayMs);
    }
    SetColor();
    for (int x = 0; x < W; ++x)
    {
        int cx = (x < half) ? x : (W - 1 - x);
        Color c = (Color)std::min((int)(((float)cx / half) * colorCount), colorCount - 1);
        SetColor(c, c);
        for (int y = 0; y < H; ++y)
        {
            GotoXY(x, y);
            cout << " ";
        }
    }
    for (int step = 0; step < half; ++step)
    {
        int leftX = half - 1 - step; 
        int rightX = half + step;

        for (int y = 0; y < H; ++y)
        {
            GotoXY(leftX, y);
            cout << " ";
            GotoXY(rightX, y);
            cout << " ";
        }
        Sleep(delayMs);
    }
    SetColor();
    Render(state);
}

void ShopScene::PlayCloseTransition(const GameState& state, unsigned long  delayMilisecond)
{
    COORD res = GetConsoleResolution();
    int W = res.X;
    int H = res.Y;
    int half = W / 2;
    unsigned long  delayMs = delayMilisecond;
    int colorCount = (int)Color::END;

    for (int step = 1; step <= half; ++step)
    {
        Color c = (Color)std::min((int)(((float)step / half) * colorCount), colorCount - 1);
        SetColor(c, c);

        for (int y = 0; y < H; ++y)
        {
            GotoXY(step - 1, y);
            cout << " ";
            GotoXY(W - step, y);
            cout << " ";
        }
        Sleep(delayMs);
    }
    SetColor();
}
void ShopScene::RenderItemArt(const Item& item)
{
    SetColor();
    for (int i = 0; i < ItemArtHeight; ++i)
    {
        GotoXY(ItemArtX, ItemArtY + i);
        if (i < (int)item.art.size())
            cout << std::left << std::setw(ItemArtWidth) << item.art[i];
        else
            cout << std::setw(ItemArtWidth) << "";
    }
}