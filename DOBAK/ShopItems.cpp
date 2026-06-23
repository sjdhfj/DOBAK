#include "ShopItems.h"
#include "Commands.h"

void ShopItemInit(GameState& state)
{
    state.shopItems =
    {

          // ── 67 골드 계열 ─────────────────────────────────────────
          { 6701, "주식마녀의 부적",
            "스핀마다 1G가 보상에 더해진다. ㅂㅌㅇ의 기운이 깃든 낡은 부적.",
            50, ItemType::EQUIP, new AddGoldCommand(1),
            { " ___ ", "(6=7)", " --- " } },

          { 6703, "67 동전 자루",
            "스핀마다 2G가 보상에 더해진다. 67개씩 묶인 동전.",
            120, ItemType::EQUIP, new AddGoldCommand(2),
            { " ___ ", "(_67)", " --- " } },

          { 6706, "67을 외친 대가",
            "스핀당 +5G, 위로금 -2. 큰 소리엔 대가가 따른다.",
            200, ItemType::EQUIP,
            new CompositeCommand({ new AddGoldCommand(5), new ConsolationGoldCommand(-2) }),
            { " ___ ", "(667)", " --- " } },

          { 6709, "6과7의 기초",
            "스핀당 +6G, 위로금 +7G. 6과 7의 완벽한 황금 비율.",
            1000, ItemType::EQUIP,
            new CompositeCommand({ new AddGoldCommand(6), new ConsolationGoldCommand(7) }),
            { " ___ ", "(G67)", " --- " } },

          { 6713, "왕초보의 67",
            "스핀마다 1G가 더해진다. 67에 입문하는 첫 걸음.",
            67, ItemType::EQUIP, new AddGoldCommand(1),
            { " ___ ", "(n67)", " --- " } },

          { 6717, "쌍둥이 고블린의 팔찌",
            "위로금 + 3G. 이 고블린 2마리는 위로를 받고 싶어한다.",
            150, ItemType::EQUIP,
            new CompositeCommand({  new ConsolationGoldCommand(3) }),
            { " ___ ", "(6|7)", " --- " } },

          { 6720, "67의 빚",
            "스핀당 -20G지만 배율 x2.67. 지금 당장은 손해다.",
            400, ItemType::EQUIP,
            new CompositeCommand({ new AddGoldCommand(-20), new MultiplierGoldCommand(2.67f) }),
            { " ___ ", "($-7)", " --- " } },

          { 6722, "67의 중수",
            "스핀당 +67G, 위로금 -67G, 속도 +134ms. 67의 정수를 이해하기 시작하는 아이템",
            3000, ItemType::EQUIP,
            new CompositeCommand({
                new AddGoldCommand(67),
                new ConsolationGoldCommand(-67),
                new SpinSpeedCommand(134) }),
            { " ___ ", "(67*)", " --- " } },

          { 6724, "순수한 67",
            "스핀마다 67G가 묵직하게 더해진다. 잡기술 없이 순수하게.",
            5000, ItemType::EQUIP, new AddGoldCommand(67),
            { " ___ ", "(P67)", " --- " } },

          { 6730, "겸손한 67",
            "스핀당 +7G, 위로금 +7G, 속도 +7ms. 뭐든 조금씩 고루 챙긴다.",
            2000, ItemType::EQUIP,
            new CompositeCommand({
                new AddGoldCommand(7),
                new ConsolationGoldCommand(7),
                new SpinSpeedCommand(7) }),
            { " ___ ", "(=67)", " --- " } },

          { 6731, "67 마스터키",
            "스핀당 +34G, 배율 x1.34, 속도 +34ms. 전설까진 아니다.",
            5000, ItemType::EQUIP,
            new CompositeCommand({
                new AddGoldCommand(34),
                new MultiplierGoldCommand(1.34f),
                new SpinSpeedCommand(34) }),
            { " ___ ", "(M67)", " --- " } },

            { 6705, "전설의 6.7배율기",
              "보상 배율 x6.7. 전설로만 전해지던 그 배율이 실존했다.",
              676767, ItemType::EQUIP, new MultiplierGoldCommand(6.7f),
              { " ___ ", "(6.7)", " --- " } },

            { 6712, "67배의 도박",
              "배율 x6.7, 위로금 -6767G. 배율은 미쳤는데 망하면 끝까지 망한다.",
              6767, ItemType::EQUIP,
              new CompositeCommand({
                  new MultiplierGoldCommand(6.7f),
                  new ConsolationGoldCommand(-6767) }),
              { " ___ ", "(±67)", " --- " } },

            { 6718, "약한 배율기",
              "보상 배율 x1.67. 전설급은 아니지만 67의 배율을 품었다.",
              1670, ItemType::EQUIP, new MultiplierGoldCommand(1.67f),
              { " ___ ", "(x67)", " --- " } },

            { 6726, "67배 위로",
              "배율 x1.67, 꽝이어도 위로금 +67G. 지더라도 67이 챙겨준다.",
              4000, ItemType::EQUIP,
              new CompositeCommand({
                  new ConsolationGoldCommand(67),
                  new MultiplierGoldCommand(1.67f) }),
              { " ___ ", "(c67)", " --- " } },

            { 6729, "67 도박사의 장갑",
              "배율 x3.67, 위로금 -200G. 모든 게 극단으로 치닫는다.",
              4500, ItemType::EQUIP,
              new CompositeCommand({
                  new MultiplierGoldCommand(3.67f),
                  new ConsolationGoldCommand(-200) }),
              { " ___ ", "(G67)", " --- " } },

              { 6707, "67의 위로상",
                "꽝이어도 위로금 +67G. 67이 패배자에게도 손을 내민다.",
                1250, ItemType::EQUIP, new ConsolationGoldCommand(67),
                { " ___ ", "(o67)", " --- " } },

              { 6715, "괜찮아 위로주",
                "꽝이어도 위로금 +13G(=6+7). 67이 조용히 건네는 위로.",
                250, ItemType::EQUIP, new ConsolationGoldCommand(13),
                { " ___ ", "(_13)", " --- " } },

              { 6719, "느긋한 위로",
                "꽝이어도 위로금 +67G, 속도 -67ms. 느긋하게 챙긴다.",
                250, ItemType::EQUIP,
                new CompositeCommand({
                    new ConsolationGoldCommand(67),
                    new SpinSpeedCommand(-67) }),
                { " ___ ", "(z67)", " --- " } },

              { 6732, "0원 67의 장난",
                "공짜인데 위로금 -7G, 콤보 -7G. 왜 파는 건지 모르겠다.",
                0, ItemType::EQUIP,
                new CompositeCommand({
                    new ConsolationGoldCommand(-7),
                    new ComboBonusCommand(-7) }),
                { " ___ ", "(0_0)", " --- " } },

                { 6708, "초고속 릴 67",
                  "스핀 속도 +670ms 단축. 릴이 0.67초 더 빨리 멈춘다.",
                  200, ItemType::EQUIP, new SpinSpeedCommand(670),
                  { " ___ ", "(>67)", " --- " } },

                { 6711, "초단타 67",
                  "속도 +467ms, 스핀당 +6G. 빠르게 치고 빠지는 67 전략.",
                  400, ItemType::EQUIP,
                  new CompositeCommand({
                      new SpinSpeedCommand(467),
                      new AddGoldCommand(6) }),
                  { " ___ ", "(67>)", " --- " } },

                { 6716, "총알 릴",
                  "스핀 속도 +167ms 단축. 67의 기운을 실은 총알 같은 릴.",
                  100, ItemType::EQUIP, new SpinSpeedCommand(167),
                  { " ___ ", "(=>>)", " --- " } },

                { 6727, "조용한 67",
                  "스핀 속도 +67ms 단축. 티는 안 나지만 67이 살짝 빠르다.",
                  67, ItemType::EQUIP, new SpinSpeedCommand(67),
                  { " ___ ", "(~67)", " --- " } },

                { 6728, "광속 67",
                  "스핀 속도 +1200ms 단축. 거의 즉시 끝난다. 67의 극한.",
                  670, ItemType::EQUIP, new SpinSpeedCommand(1200),
                  { " ___ ", "(!67)", " --- " } },

                  { 6733, "67 콤보 부스터",
                    "패턴 하나 추가될 때마다 +67G 콤보 보너스.",
                    1670, ItemType::EQUIP, new ComboBonusCommand(67),
                    { " ___ ", "(c+c)", " --- " } },

                  { 6734, "탐욕스런 67",
                    "콤보 보너스 +34G에 배율 x1.67. 67이 욕심을 부린다.",
                    3670, ItemType::EQUIP,
                    new CompositeCommand({
                        new ComboBonusCommand(34),
                        new MultiplierGoldCommand(1.67f) }),
                    { " ___ ", "(g67)", " --- " } },

                    { 7010, "67의 크기욕심",
                      "패턴 한 칸당 6G 추가. 넓을수록 67이 더 좋아한다.",
                      500, ItemType::EQUIP, new PatternSizeBonusCommand(6),
                      { " ___ ", "(S67)", " --- " } },

                    { 7011, "67의 대식가",
                      "패턴 한 칸당 13G 추가, 속도 -67ms. 크게 먹고 느리게 간다.",
                      500, ItemType::EQUIP,
                      new CompositeCommand({
                          new PatternSizeBonusCommand(13),
                          new SpinSpeedCommand(-67) }),
                      { " ___ ", "(B67)", " --- " } },

                    { 7012, "67 면적 배율기",
                      "패턴 한 칸당 7G에 배율 x1.67. 넓이와 배율, 둘 다 욕심낸다.",
                      670, ItemType::EQUIP,
                      new CompositeCommand({
                          new PatternSizeBonusCommand(7),
                          new MultiplierGoldCommand(1.67f) }),
                      { " ___ ", "(A67)", " --- " } },

                    { 7013, "67 면적왕",
                      "패턴 한 칸당 67G. 5x3이 터지면 무슨 일이 벌어질지.",
                      6700, ItemType::EQUIP, new PatternSizeBonusCommand(67),
                      { " ___ ", "(W67)", " --- " } },

                      { 6767, "67의 만능 인장",
                        "+67G, 배율 x1.67, 위로금 +67G, 속도 +67ms, 칸당 +6G. 67의 모든 것.",
                        6767, ItemType::EQUIP,
                        new CompositeCommand({
                            new AddGoldCommand(67),
                            new MultiplierGoldCommand(1.67f),
                            new ConsolationGoldCommand(67),
                            new SpinSpeedCommand(67),
                            new PatternSizeBonusCommand(6) }),
                        { " ___ ", "(67∞)", " --- " } },

                        { 7001, "67의 가로신",
                          "3x1 가로 패턴 보상 x2. 67이 가로줄에 강림했다.",
                          250, ItemType::EQUIP,
                          new PatternBonusCommand(3, 1, 2.0f),
                          { " ___ ", "(-67)", " --- " } },

                        { 7002, "황금 육칠줄",
                          "3x1 가로 패턴에 +200G 추가. 67이 황금 가로줄을 그었다.",
                          300, ItemType::EQUIP,
                          new PatternBonusCommand(3, 1, 1.0f, 200),
                          { " ___ ", "(=67)", " --- " } },

                        { 7003, "세로 67령",
                          "1x3 세로 패턴 보상 x2. 67의 정령이 세로줄을 타고 내린다.",
                          250, ItemType::EQUIP,
                          new PatternBonusCommand(1, 3, 2.0f),
                          { " ___ ", "(|67)", " --- " } },

                        { 7004, "67 사각진",
                          "2x2 정사각 패턴에 +300G 추가. 67이 사각형 진지를 쳤다.",
                          400, ItemType::EQUIP,
                          new PatternBonusCommand(2, 2, 1.0f, 300),
                          { " ___ ", "([67)", " --- " } },

                        { 7005, "67의 사각왕",
                          "2x2 정사각 패턴 보상 x3. 67이 사각형의 왕좌에 올랐다.",
                          600, ItemType::EQUIP,
                          new PatternBonusCommand(2, 2, 3.0f),
                          { " ___ ", "([6])", " --- " } },

                        { 7006, "만관의 67",
                          "5x1 풀 라인 패턴 보상 x5. 67이 모든 릴을 지배한다.",
                          900, ItemType::EQUIP,
                          new PatternBonusCommand(5, 1, 5.0f),
                          { " ___ ", "(567)", " --- " } },

                        { 7007, "67의 줄콤보",
                          "3x1 가로 패턴 x2에 콤보 보너스 +50G. 67이 콤보까지 탐낸다.",
                          500, ItemType::EQUIP,
                          new CompositeCommand({
                              new PatternBonusCommand(3, 1, 2.0f),
                              new ComboBonusCommand(50) }),
                          { " ___ ", "(-6-)", " --- " } },
    };
}