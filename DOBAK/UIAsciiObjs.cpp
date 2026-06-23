#include "AsciiArt.h"
#include "Console.h"
#include "GameState.h"
#include "UIAsciiObjs.h"

void AsciiInit(UIAsciiObjs& objs)
{
    objs.six =
    {
        "            @@@@,       ",
        "          :@@ ~@@       ",
        "         ~@@   @@       ",
        "         @@-  @@@       ",
        "        ;@@             ",
        "        @@@             ",
        "        @@$@@@$         ",
        "       -@@@=-@@@:       ",
        "       @@@@  .@@@       ",
        "       @@@:   @@@       ",
        "       @@@~   @@@       ",
        "       !@@:  .@@@       ",
        "        @@!  @@@,       ",
        "        @@@  @@@        ",
        "         @@@@@$         "
    };
    objs.seven =
    {
        "                       ",
        "       @@@@@@@@@       ",
        "       @@~...:@=       ",
        "        @     @@       ",
        "             @@*       ",
        "          =@@          ",
        "          @@@          ",
        "         @@@           ",
        "        -@@            ",
        "        @@@            ",
        "       .@@-            ",
        "        @@@.           ",
        "        @@@            ",
        "         @@-           ",
        "                       "
    };

    objs.shopSix =
    {
        L"   ⠀⠀⢀⠤⣂⣤⣬⣭⣭⣭⣔⡠⡀⠀⠀⠀⠀",
        L"   ⠀⠔⣵⣾⣿⣿⣿⢿⣿⣿⣿⣿⣎⢂⠀  ",
        L"   ⠐⣾⣿⣿⣿⡏⣾⡿⢎⣛⣫⣭⣴⣾⠆ ",
        L"   ⡇⣿⣿⣿⣿⣟⡿⢀⣐⣻⣛⡩⢁⠀⠀  ",
        L"   ⡇⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿⣿⣶⡕⠄ ",
        L"   ⠃⣿⣿⣿⣿⣿⢋⣥⠭⡻⣿⣿⣿⣿⡌⡄",
        L"   ⠨⢻⣿⣿⣿⣧⢻⠁⠀⠘⢸⣿⣿⣿⡇⣿",
        L"   ⠀⢦⢻⣿⣿⣿⣦⣐⣀⣊⣼⣿⣿⡿⢱⡿",
        L"   ⠀⠀⠣⣙⠿⣿⣿⣿⣿⣿⣿⠿⢛⣵⡿⠃",
        L"   ⠀⠀⠀⠈⠛⠶⣮⣭⣭⣴⣶⡿⠿⠋⠀⠀ ",
        L"                         "
        /*L"   ⠀⠀⢀⠔⠒⠂⠠⠤⠭⡀⠀⠀⠀⠀⠀⠀⠀",
        L"   ⢀⠆⠁⠀⡄⠀⠀⠀⠀⠈⢂⠀⠀⠀⠀⠀⠀ ",
        L"   ⠣⠤⢤⠞⠂⠀⣀⠰⠃⠀⠘⣆⢀⣀⠀⠀⠀",
        L"   ⠀⠀⢸⠀⠈⠭⡀⢈⣡⠔⢶⠁⣹⢩⠃  ",
        L"   ⠀⠀⠀⠣⠀⢂⠞⠱⠴⣈⡸⠰⢇⠘⠀  ",
        L"   ⠀⠀⠀⠀⠱⠄⣀⢜⢁⡠⠥⠊⠀⠀⠀   ",
        L"   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀   "*/
    };
    objs.shopSeven =
    {
        L"   ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀     ",
        L" ⢲⣤⣤⣤⣤⣀⣒⣒⣒⣒⣂⡠⠤⠤⣄  ",
        L" ⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢼  ",
        L" ⣘⣛⣛⡛⠿⠿⠿⢿⣿⣿⣿⣿⣿⢟⣾  ",
        L" ⠉⠛⠛⠛⠛⡻⣣⣾⣿⣿⣿⢟⣵⣿⠛  ",
        L"⠀   ⠀⠀⡐⣼⣿⣿⣿⡿⣣⣾⠏⠀⠀  ",
        L"⠀⠀   ⠌⣼⣿⣿⣿⡿⢱⣿⠃⠀⠀⠀  ",
        L"⠀   ⠰⣸⣿⣿⣿⣿⢣⣿⠃⠀⠀⠀⠀  ",
        L"  ⢀⢃⣿⣿⣿⣿⡟⣾⡇⠀⠀⠀⠀⠀   ",
        L"  ⢨⣘⣿⡻⠿⠿⢇⣿⠀⠀⠀⠀⠀⠀   ",
        L"   ⠙⠛⠛⠛⠛⠻⠃⠀⠀⠀⠀⠀⠀   "
        /*L"  ⠀  ⠀⢀⡤⠒⠁⠀⠀⠒⢤⡀⠀⠀  ",
        L"   ⠀⢀⠎⠀⢠⡀⠀⠀⠀⢀⠀⠙⡀   ",
        L" ⠀⢀⠀⢸⠀⠀⠀⣑⣠⣤⠀⠙⡦⣀⠜  ",
        L" ⠰⡭⠷⢝⡤⣂⣄⠒⢤⡐⠀⠀⡇⠀⠀  ",
        L"⠀⠡⡘⡄⠐⡂⠘⢌⡀⠉⠂⡸⠀⠀⠀   ",
        L" ⠀⠈⠙⠄⠹⢅⣀⠹⠒⠊⠀⠀⠀⠠   "*/
    };
    objs.shopmen =
    {
        R"(         ////^\\\\               )",
        R"(         | _   _ |               )",
        R"(        @ (o) (o) @              )",
        R"(         |   <   |               )",
        R"(         |  ___  |               )",
        R"(          \_____/                )",
        R"(        ____|  |____             )",
        R"(       /    \__/    \            )",
        R"(      /              \           )",
        R"(     /\_/|        |\_/\          )",
        R"(    / /  |        |  \ \         )",
        R"(   ( <   |        |   > )        )",
        R"(    \ \  |        |  / /         )",
        R"(     \ \ |________| / /          )",
        R"(      \ \|<I_D_I__|/ /           )",
        R"(       \ \ / I  \ / /            )",
        R"(        \ /  I   \ /             )",
        R"(         |        |              )",
        R"(         |   |    |              )",
        R"(         |   |    |              )",
        R"(         |   |    |              )",
        R"(         |   |    |              )",
        R"(         |## | ## |              )",
        R"(         |   |    |              )",
        R"(         |   |    |              )",
        R"(         |___|____|              )",
        R"(         (___(____)              )",
        R"(         _| | _| |               )",
        R"(     cccC__Cccc___)              )"
    };
    objs.shopangrymen =
    {
        R"(         ////^\\\\               )",
        R"(         | \   / |               )",
        R"(        @ (o) (o) @              )",
        R"(         |   <   |               )",
        R"(         |  .-.  |               )",
        R"(          \_____/                )",
        R"(        ____|  |____             )",
        R"(       /    \__/    \            )",
        R"(      /              \           )",
        R"(     /\_/|        |\_/\          )",
        R"(    / /  |        |  \ \         )",
        R"(   ( <   |        |   > )        )",
        R"(    \ \  |        |  / /         )",
        R"(     \ \ |________| / /          )",
        R"(      \ \|<I_D_I__|/ /           )",
        R"(       \ \ / I  \ / /            )",
        R"(        \ /  I   \ /             )",
        R"(         |        |              )",
        R"(         |   |    |              )",
        R"(         |   |    |              )",
        R"(         |   |    |              )",
        R"(         |   |    |              )",
        R"(         |## | ## |              )",
        R"(         |   |    |              )",
        R"(         |   |    |              )",
        R"(         |___|____|              )",
        R"(         (___(____)              )",
        R"(         _| | _| |               )",
        R"(     cccC__Cccc___)              )"
    };
    objs.shophappymen =
    {
        R"( Ccc      ////^\\\\       Ccc    )",
        R"(c( )D     | ^   ^ |      C( )D   )",
        R"( \  \    @ (O) (O) @     /  /    )",
        R"(  \  \    |   <   |     /  /     )",
        R"(   \  \   |  \_/  |    /  /      )",
        R"(    \  \   \_____/    /  /       )",
        R"(     \  \____|  |____/  /        )",
        R"(      \ /    \__/    \ /         )",
        R"(       \__          __/          )",
        R"(          |        |             )",
        R"(          |        |             )",
        R"(          |        |             )",
        R"(          |        |             )",
        R"(          |________|             )",
        R"(          |_I_D_I__|             )",
        R"(          | / I  \ |             )",
        R"(          |/  I   \|             )",
        R"(          |        |             )",
        R"(          |   |    |             )",
        R"(          |   |    |             )",
        R"(          |   |    |             )",
        R"(          |   |    |             )",
        R"(          |## | ## |             )",
        R"(          |   |    |             )",
        R"(          |   |    |             )",
        R"(          |___|____|             )",
        R"(         (____(____)             )",
        R"(          _| | _| |              )",
        R"(      cccC__Cccc___)             )"
    };
    objs.shopjuicemen =
    {
        R"(          ////^\\\\              )",
        R"(          | ^   ^ |              )",
        R"(         @ (O) (O) @             )",
        R"(          |   <   |              )",
        R"(          |  \_/  |          \__ )",
        R"(           \_____/           |\ |)",
        R"(         ____|  |____        | \|)",
        R"(        /    \__/    \      /|__|)",
        R"(       /              \    / /   )",
        R"(      /__/|        |\__\  / /    )",
        R"(     /  / |        | \  \/ /     )",
        R"(    (  (  |        |  \   /      )",
        R"(     \  \ |        |   \_/       )",
        R"(      \  \|________|             )",
        R"(       \__|_I_D_I__|             )",
        R"(          | / I  \ |             )",
        R"(          |/  I   \|             )",
        R"(          |   |    |             )",
        R"(          |   |    |             )",
        R"(          |___|____|             )",
        R"(          (___(____)             )",
        R"(           | |  | |              )",
        R"(           | |  | |              )",
        R"(           ( |  ( |              )",
        R"(           | |  | |              )",
        R"(           | |  | |              )",
        R"(           | |  | |              )",
        R"(          _| | _| |              )",
        R"(      cccC__Cccc___)             )"
    };
    

    objs.titleascii =
    {
        L"██████╗  ██████╗ ██████╗  █████╗ ██╗  ██╗",
        L"██╔══██╗██╔═══██╗██╔══██╗██╔══██╗██║ ██╔╝",
        L"██║  ██║██║   ██║██████╔╝███████║█████╔╝ ",
        L"██║  ██║██║   ██║██╔══██╗██╔══██║██╔═██╗ ",
        L"██████╔╝╚██████╔╝██████╔╝██║  ██║██║  ██╗",
        L"╚═════╝  ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝"
    };
    objs.endingascii =
    {
        L"████████╗██╗  ██╗ █████╗ ███╗   ██╗██╗  ██╗███████╗    ██████╗ ██╗      █████╗ ██╗   ██╗██╗███╗   ██╗ ██████╗  ",
        L"╚══██╔══╝██║  ██║██╔══██╗████╗  ██║██║ ██╔╝██╔════╝    ██╔══██╗██║     ██╔══██╗╚██╗ ██╔╝██║████╗  ██║██╔════╝  ",
        L"   ██║   ███████║███████║██╔██╗ ██║█████╔╝ ███████╗    ██████╔╝██║     ███████║ ╚████╔╝ ██║██╔██╗ ██║██║  ███╗ ",
        L"   ██║   ██╔══██║██╔══██║██║╚██╗██║██╔═██╗ ╚════██║    ██╔═══╝ ██║     ██╔══██║  ╚██╔╝  ██║██║╚██╗██║██║   ██║ ",
        L"   ██║   ██║  ██║██║  ██║██║ ╚████║██║  ██╗███████║    ██║     ███████╗██║  ██║   ██║   ██║██║ ╚████║╚██████╔╝ ",
        L"   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝    ╚═╝     ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝╚═╝  ╚═══╝ ╚═════╝  "                                                                                                                      
    };
    objs.gameoverascii =
    {
        L" ██████╗  █████╗ ███╗   ███╗███████╗ ██████╗ ██╗   ██╗███████╗██████╗ ",
        L"██╔════╝ ██╔══██╗████╗ ████║██╔════╝██╔═══██╗██║   ██║██╔════╝██╔══██╗",
        L"██║  ███╗███████║██╔████╔██║█████╗  ██║   ██║██║   ██║█████╗  ██████╔╝",
        L"██║   ██║██╔══██║██║╚██╔╝██║██╔══╝  ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗",
        L"╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗╚██████╔╝ ╚████╔╝ ███████╗██║  ██║",
        L" ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝ ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝",
    };
    /*COORD res = GetConsoleResolution();
    for (string& line : objs.six)
        line.resize(res.X, ' ');
    for (string& line : objs.seven)
        line.resize(res.X, ' ');*/
    objs.sixOffset = 0;
    objs.sevenOffset = 0;
}

void AsciiUpdate(UIAsciiObjs& objs)
{
    /*for (string& line : objs.six)
        std::rotate(line.begin(), line.begin() + 1, line.end());
    for (string& line : objs.seven)
        std::rotate(line.rbegin(), line.rbegin() + 1, line.rend());*/
    ++objs.sixOffset;
    --objs.sevenOffset;
}


void AsciiRender(const UIAsciiObjs& objs, const vector<ExcludeRect>& excludeRects)
{
    int sixCount = (int)objs.six.size();
    int sevenCount = (int)objs.seven.size();

    COORD res = GetConsoleResolution();

    SetColor(Color::LIGHT_YELLOW);
    for (int y = 0; y < sixCount; ++y)
    {
        if (objs.six[y].empty()) continue;
        string fullLine = BuildTiledLine(objs.six[y], objs.sixOffset, res.X);
        DrawRowExcluding(fullLine, y, res.X, excludeRects);
    }

    SetColor(Color::CYAN);
    for (int i = 0; i < sevenCount; ++i)
    {
        int y = sevenCount + i;
        if (objs.seven[i].empty()) continue;
        string fullLine = BuildTiledLine(objs.seven[i], objs.sevenOffset, res.X);
        DrawRowExcluding(fullLine, y, res.X, excludeRects);
    }

    SetColor();
}

void DrawTitle(const UIAsciiObjs& objs)
{
    COORD res = GetConsoleResolution();
    int titleX = (res.X - 70) / 2;
    int titleY = res.Y / 3;

    SetUniCodeMode();
    for (int i = 0; i < (int)objs.titleascii.size(); ++i)
    {
        GotoXY(titleX, titleY + i);
        wcout << objs.titleascii[i];
    }
    SetDefaultMode();
}
string BuildTiledLine(const string& pattern, int offset, int width)
{
    int patternLen = (int)pattern.size();
    if (patternLen == 0)
        return string(width, ' ');

    string out;
    out.reserve(width);
    for (int x = 0; x < width; ++x)
    {
        int idx = ((x + offset) % patternLen + patternLen) % patternLen;
        out += pattern[idx];
    }
    return out;
}

void DrawRowExcluding(const string& fullLine, int y, int width,
    const vector<ExcludeRect>& excludeRects)
{
    int cursor = 0;
    while (cursor < width)
    {
        int skipTo = -1;
        for (const ExcludeRect& rect : excludeRects)
        {
            if (y >= rect.y && y < rect.y + rect.height
                && cursor >= rect.x && cursor < rect.x + rect.width)
            {
                skipTo = rect.x + rect.width;
                break;
            }
        }

        if (skipTo >= 0)
        {
            cursor = skipTo;
            continue;
        }

        int nextStart = width;
        for (const ExcludeRect& rect : excludeRects)
        {
            if (y >= rect.y && y < rect.y + rect.height
                && rect.x > cursor && rect.x < nextStart)
            {
                nextStart = rect.x;
            }
        }

        int len = nextStart - cursor;
        if (len > 0)
        {
            GotoXY(cursor, y);
            cout.write(fullLine.data() + cursor, len);
        }
        cursor = nextStart;
    }
}
