#include "ShopItems.h"
#include "Commands.h"
void ShopItemInit(GameState& state)
{
	state.shopItems =
	{
		{1, "name", "description", 100, ItemType::CONSUME, new AddGoldCommand(10),
			{
				" ___ ",
				"($$$)",
				" --- "
			}
		},
		{6701, "67 부적", "왠지 익숙한 숫자가 골드를 끌어모은다.", 67, ItemType::EQUIP,
			new AddGoldCommand(6), { " ___ ", "(6=7)", " --- " }},

		{6702, "67의 속삭임", "어디선가 67이라고 외치는 소리가 들린다.", 67, ItemType::EQUIP,
			new ProbabilityCommand(7), { " ___ ", "(67!)", " --- " }},

		{6703, "육칠 동전 자루", "동전이 67개씩 묶여서 들어있다.", 134, ItemType::EQUIP,
			new AddGoldCommand(13), { " ___ ", "(_67)", " --- " }},   // 6+7=13

		{6704, "67 헌터의 인장", "패턴 하나마다 확률이 따라붙는다.", 168, ItemType::EQUIP,
			new ProbabilityCommand(13), { " ___ ", "(67x)", " --- " }},

		{6705, "전설의 6.7배율기", "전설로만 전해지던 그 배율.", 670, ItemType::EQUIP,
			new MultiplierGoldCommand(6.7f), { " ___ ", "(6.7)", " --- " }},   // 네타성 강력템

		{6706, "67을 외친 대가", "큰 골드가 들어오지만 운이 따라주지 않는다.", 167, ItemType::EQUIP,
			new CompositeCommand({ new AddGoldCommand(67), new ProbabilityCommand(-6) }),
			{ " ___ ", "(667)", " --- " }},
		{6707, "67의 위로상", "꽝이어도 67골드는 챙겨준다.", 134, ItemType::EQUIP,
			new ConsolationGoldCommand(67), { " ___ ", "(o67)", " --- " }},

		{6708, "초고속 릴 67", "릴이 0.67초 더 빨리 멈춘다.", 201, ItemType::EQUIP,
			new SpinSpeedCommand(670), { " ___ ", "(>67)", " --- " }},

		{6709, "67의 황금비", "골드도, 확률도 둘 다 챙긴다.", 469, ItemType::EQUIP,
			new CompositeCommand({ new AddGoldCommand(67), new ProbabilityCommand(6) }),
			{ " ___ ", "(G67)", " --- " }},

		{6710, "행운과 위로", "확률도 오르고 꽝이어도 위로금이 나온다.", 268, ItemType::EQUIP,
			new CompositeCommand({ new ProbabilityCommand(6), new ConsolationGoldCommand(7) }),
			{ " ___ ", "(6+7)", " --- " }},

		{6711, "초단타 67", "스핀이 빨라지고 골드도 조금 붙는다.", 335, ItemType::EQUIP,
			new CompositeCommand({ new SpinSpeedCommand(467), new AddGoldCommand(6) }),
			{ " ___ ", "(67>)", " --- " }},

		{6712, "67배의 도박", "배율은 미쳤는데 확률은 깎인다.", 603, ItemType::EQUIP,
			new CompositeCommand({ new MultiplierGoldCommand(6.7f), new ProbabilityCommand(-13) }),
			{ " ___ ", "(±67)", " --- " }},
		{6713, "왕초보의 67", "처음 만져보는 67의 기운.", 67, ItemType::EQUIP,
			new AddGoldCommand(6), { " ___ ", "(n67)", " --- " }},

		{6714, "67의 새싹", "아직 미약하지만 확률이 오른다.", 67, ItemType::EQUIP,
			new ProbabilityCommand(6), { " ___ ", "(.67)", " --- " }},

		{6715, "괜찮아 위로주", "꽝이어도 13골드는 위로해준다.", 100, ItemType::EQUIP,
			new ConsolationGoldCommand(13), { " ___ ", "(_13)", " --- " }},   // 6+7=13

		{6716, "총알 릴", "릴이 0.167초 빨리 멈춘다.", 100, ItemType::EQUIP,
			new SpinSpeedCommand(167), { " ___ ", "(=>>)", " --- " }},

		{6717, "쌍둥이 67", "골드와 확률이 사이좋게 같이 오른다.", 234, ItemType::EQUIP,
			new CompositeCommand({ new AddGoldCommand(13), new ProbabilityCommand(6) }),
			{ " ___ ", "(6|7)", " --- " }},

		{6718, "약한 배율기", "전설급은 아니지만 쓸만한 배율.", 167, ItemType::EQUIP,
			new MultiplierGoldCommand(1.67f), { " ___ ", "(x67)", " --- " }},

		{6719, "느긋한 위로", "스핀은 느려지지만 꽝이어도 든든하다.", 201, ItemType::EQUIP,
			new CompositeCommand({ new ConsolationGoldCommand(67), new SpinSpeedCommand(-67) }),
			{ " ___ ", "(z67)", " --- " }},   // 속도 페널티 버전 (음수도 가능)

		{6720, "67의 빚", "당장은 손해지만 한 방이 크다.", 402, ItemType::EQUIP,
			new CompositeCommand({ new AddGoldCommand(-20), new MultiplierGoldCommand(2.67f) }),
			{ " ___ ", "($-7)", " --- " }},

		{6721, "안정적인 67", "확률도 오르고 꽝이어도 든든한 만능형.", 268, ItemType::EQUIP,
			new CompositeCommand({ new ProbabilityCommand(13), new ConsolationGoldCommand(6) }),
			{ " ___ ", "(s67)", " --- " }},

		{6722, "67의 분신", "정신없이 빨라지는 대신 집중력이 흐트러진다.", 469, ItemType::EQUIP,
			new CompositeCommand({ new AddGoldCommand(67), new ProbabilityCommand(-6), new SpinSpeedCommand(134) }),
			{ " ___ ", "(67*)", " --- " }},
		{6723, "67의 저주", "공짜인 데는 이유가 있다.", 6, ItemType::EQUIP,
			new ProbabilityCommand(-67), { " ___ ", "(x67)", " --- " }},   // 트롤형: 극단적 마이너스 확률

		{6724, "순수한 67", "잡다한 거 없이 골드만 묵직하게.", 335, ItemType::EQUIP,
			new AddGoldCommand(67), { " ___ ", "(P67)", " --- " }},

		{6725, "확률 그 자체", "이걸 끼면 운이 다른 차원으로 간다.", 670, ItemType::EQUIP,
			new ProbabilityCommand(67), { " ___ ", "(@67)", " --- " }},

		{6726, "67배 위로", "꽝이어도 위로금이 쏠쏠하게 불어난다.", 402, ItemType::EQUIP,
			new CompositeCommand({ new ConsolationGoldCommand(67), new MultiplierGoldCommand(1.67f) }),
			{ " ___ ", "(c67)", " --- " }},

		{6727, "조용한 67", "티는 안 나지만 릴이 살짝 빨라진다.", 67, ItemType::EQUIP,
			new SpinSpeedCommand(67), { " ___ ", "(~67)", " --- " }},

		{6728, "광속 67", "스핀이 거의 즉시 끝난다.", 603, ItemType::EQUIP,
			new SpinSpeedCommand(1200), { " ___ ", "(!!!)", " --- " }},

		{6729, "67 도박사의 장갑", "배율은 극단적이지만 모든 게 같이 흔들린다.", 536, ItemType::EQUIP,
			new CompositeCommand({
				new MultiplierGoldCommand(3.67f),
				new ProbabilityCommand(-20),
				new ConsolationGoldCommand(-20)
			}), { " ___ ", "(G67)", " --- " }},   // 꽝나면 위로금도 마이너스라 더 아픔

		{6730, "겸손한 67", "뭐든 조금씩 골고루 챙긴다.", 201, ItemType::EQUIP,
			new CompositeCommand({ new AddGoldCommand(7), new ProbabilityCommand(7), new SpinSpeedCommand(7) }),
			{ " ___ ", "(=67)", " --- " }},

		{6731, "67 마스터키", "전설까진 아니지만 준수하게 다 챙긴다.", 670, ItemType::EQUIP,
			new CompositeCommand({
				new AddGoldCommand(34),
				new ProbabilityCommand(13),
				new MultiplierGoldCommand(1.34f),
				new SpinSpeedCommand(34)
			}), { " ___ ", "(M67)", " --- " }},

		{6732, "0원 67의 장난", "공짜인데 다 마이너스다. 왜 파는 거지?", 0, ItemType::EQUIP,
			new CompositeCommand({ new ProbabilityCommand(-7), new ConsolationGoldCommand(-7) }),
			{ " ___ ", "(0_0)", " --- " }},
		{ 6733, "67 콤보 부스터", "한 스핀에 여러 패턴이 터지면 터질수록 강해진다.", 268, ItemType::EQUIP,
		new ComboBonusCommand(67), { " ___ ", "(c+c)", " --- " } },

		{ 6734, "탐욕스런 67", "콤보 보너스에 배율까지 같이 노린다.", 469, ItemType::EQUIP,
			new CompositeCommand({ new ComboBonusCommand(34), new MultiplierGoldCommand(1.67f) }),
			{ " ___ ", "(g67)", " --- " } },
		{6767, "67의 만능 인장", "전설 속에서만 전해지던, 67의 모든 것.", 6767, ItemType::EQUIP,
			new CompositeCommand({
				new AddGoldCommand(67),
				new ProbabilityCommand(7),
				new MultiplierGoldCommand(1.67f),
				new ConsolationGoldCommand(67),
				new SpinSpeedCommand(67)
			}),
			{ " ___ ", "(67∞)", " --- " }},
	};
}