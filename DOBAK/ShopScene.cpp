#include "ShopScene.h"
#include "UIAsciiObjs.h"
#include "SoundManager.h"
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

static int DisplayWidth(const string& s)
{
    int w = 0;
    for (int i = 0; i < (int)s.size(); )
    {
        unsigned char c = (unsigned char)s[i];
        if (c >= 0x81 && c <= 0xFE && i + 1 < (int)s.size())
        {
            w += 2; i += 2;
        }
        else
        {
            w += 1; i += 1;
        }
    }
    return w;
}

static vector<string> WrapText(const string& text, int maxWidth)
{
    vector<string> lines;
    string cur;
    int curW = 0;
    int i = 0;
    while (i < (int)text.size())
    {
        unsigned char c = (unsigned char)text[i];
        int charW;
        string ch;
        if (c >= 0x81 && c <= 0xFE && i + 1 < (int)text.size())
        {
            charW = 2; ch = text.substr(i, 2); i += 2;
        }
        else
        {
            charW = 1; ch = text.substr(i, 1); i += 1;
        }

        if (curW + charW > maxWidth)
        {
            lines.push_back(cur); cur = ch; curW = charW;
        }
        else
        {
            cur += ch; curW += charW;
        }
    }
    if (!cur.empty())
        lines.push_back(cur);
    return lines;
}

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
    SOUND->PlayBGM("Sound/candyland.mp3");
    SetConsoleFont(L"NSimSun", { 8, 16 });
    SetConsoleSize(WIDTH, HEIGHT);
    AsciiInit(objs);
    _curTab = ShopTab::BUY;
    _cursor = 0;
    _scrollOffset = 0;
    _savedCursor[0] = _savedCursor[1] = 0;
    _savedScroll[0] = _savedScroll[1] = 0;
    _shopItems = state.shopItems;
    PlayOpenTransition(state, 1);
}

void ShopScene::Update(GameState& state)
{
    auto& list = CurList(state);
    int size = (int)list.size();

    if (GetKeyDown(VK_LEFT) && _curTab == ShopTab::SELL)
    {
        _savedCursor[1] = _cursor;
        _savedScroll[1] = _scrollOffset;
        _curTab = ShopTab::BUY;
        _cursor = _savedCursor[0];
        _scrollOffset = _savedScroll[0];
        ClampCursorAndScroll((int)CurList(state).size());
        system("cls");
        return;
    }
    if (GetKeyDown(VK_RIGHT) && _curTab == ShopTab::BUY)
    {
        _savedCursor[0] = _cursor;
        _savedScroll[0] = _scrollOffset;
        _curTab = ShopTab::SELL;
        _cursor = _savedCursor[1];
        _scrollOffset = _savedScroll[1];
        ClampCursorAndScroll((int)CurList(state).size());
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

    if (GetKey(VK_RETURN))
    {
        if (size == 0) return;
        Item& item = list[_cursor];

        if (_curTab == ShopTab::BUY)
        {
            if (state.player.gold >= item.price)
            {
                SOUND->Play("Yeah");
                state.player.gold -= item.price;
                state.player.inventory.push_back(item);
                _isSmiling = true;
                _smileTimer = state.curTime;
                ShakeConsoleWindow(2, 50, 10);
            }
        }
        else
        {
            state.player.gold += CurPrice(item);
            list.erase(list.begin() + _cursor);
            _isSelling = true;
            SOUND->Play("Drink");
            _juiceTimer = state.curTime;
            size = (int)list.size();
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

    ClampCursorAndScroll(size);
    UpdateShakeConsoleWindow();
}

void ShopScene::Render(const GameState& state)
{
    COORD res = GetConsoleResolution();
    string esc = "[ESC]를 눌러 상점을 나갈 수 있습니다.";
    int x = (res.X - (int)esc.length()) / 2;
    SetColor();
    GotoXY(x, res.Y - 1);
    cout << esc;

    auto noConstState = state;
    const auto& list = CurList(noConstState);

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
        int idx = _scrollOffset + i;
        GotoXY(ShopX, ListY + i);
        if (idx < (int)list.size())
        {
            bool sel = (idx == _cursor);
            SetColor(sel ? Color::WHITE : Color::LIGHT_GRAY);
            cout << (sel ? "> " : "  ");
            cout << std::left << std::setw(20) << list[idx].name;
            cout << std::right << std::setw(6) << CurPrice(list[idx]) << "G";
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

        vector<string> descLines;
        if (!list.empty())
            descLines = WrapText(list[_cursor].description, DescWidth);

        for (int i = 0; i < DescLines; ++i)
        {
            GotoXY(ShopX, ListY + ListMax + 2 + i);
            SetColor(Color::CYAN);
            if (i < (int)descLines.size())
            {
                const string& line = descLines[i];
                cout << line;
                int remain = DescWidth - DisplayWidth(line);
                if (remain > 0) cout << string(remain, ' ');
            }
            else
            {
                cout << string(DescWidth, ' ');
            }
        }

    if (!list.empty())
        RenderItemArt(list[_cursor]);

    GotoXY(ShopX, ListY + ListMax + 2 + DescLines + 1);
    SetColor(Color::LIGHT_YELLOW);
    cout << "Gold: " << std::left << std::setw(10) << state.player.gold << "G";

    SetColor();
    int y = 1;
    const auto& man = _isSelling ? objs.shopjuicemen
        : (_isSmiling ? objs.shophappymen : objs.shopmen);
    for (auto& row : man)
    {
        GotoXY(ArtX, y++);
        SetColor();
        cout << row;
    }

    RenderSixSeven(state);
}

void ShopScene::RenderSixSeven(const GameState& state)
{
    COORD res = GetConsoleResolution();
    int x = res.X / 2 + 35;

    int sixWidth = 0;
    for (auto& i : objs.shopSix)
        sixWidth = std::max(sixWidth, (int)i.size());

    SetUniCodeMode();
    int y = 1;
    for (auto& i : objs.shopSix)
    {
        if (IsGotoXY(x, y++)) wcout << i;
    }
    y = 1;
    for (auto& i : objs.shopSeven)
    {
        if (IsGotoXY(x + sixWidth, y++)) wcout << i;
    }
    SetDefaultMode();
}

void ShopScene::PlayOpenTransition(const GameState& state, unsigned long delaymilisecond)
{
    COORD res = GetConsoleResolution();
    int W = res.X;
    int H = res.Y;
    int half = W / 2;
    int colorCount = (int)Color::END;

    for (int row = 0; row < H; ++row)
    {
        Color c = (Color)std::min((int)(((float)row / H) * colorCount), colorCount - 1);
        SetColor(c, c);
        GotoXY(0, row);
        for (int x = 0; x < W; ++x) cout << " ";
        Sleep(delaymilisecond);
    }

    SetColor();
    for (int x = 0; x < W; ++x)
    {
        int cx = (x < half) ? x : (W - 1 - x);
        Color c = (Color)std::min((int)(((float)cx / half) * colorCount), colorCount - 1);
        SetColor(c, c);
        for (int y = 0; y < H; ++y)
        {
            GotoXY(x, y); cout << " ";
        }
    }

    for (int step = 0; step < half; ++step)
    {
        int leftX = half - 1 - step;
        int rightX = half + step;
        for (int y = 0; y < H; ++y)
        {
            GotoXY(leftX, y); cout << " ";
            GotoXY(rightX, y); cout << " ";
        }
        Sleep(delaymilisecond);
    }

    SetColor();
    Render(state);
}

void ShopScene::PlayCloseTransition(const GameState& state, unsigned long delayMilisecond)
{
    COORD res = GetConsoleResolution();
    int W = res.X;
    int H = res.Y;
    int half = W / 2;
    int colorCount = (int)Color::END;

    for (int step = 1; step <= half; ++step)
    {
        Color c = (Color)std::min((int)(((float)step / half) * colorCount), colorCount - 1);
        SetColor(c, c);
        for (int y = 0; y < H; ++y)
        {
            GotoXY(step - 1, y); cout << " ";
            GotoXY(W - step, y); cout << " ";
        }
        Sleep(delayMilisecond);
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

void ShopScene::ClampCursorAndScroll(int size)
{
    if (size <= 0)
    {
        _cursor = 0;
        _scrollOffset = 0;
        return;
    }
    _cursor = std::max(0, std::min(_cursor, size - 1));
    if (_cursor < _scrollOffset)
        _scrollOffset = _cursor;
    if (_cursor >= _scrollOffset + ListMax)
        _scrollOffset = _cursor - ListMax + 1;
    _scrollOffset = std::max(0, std::min(_scrollOffset, std::max(0, size - ListMax)));
}