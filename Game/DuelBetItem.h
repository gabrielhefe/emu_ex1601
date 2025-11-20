#ifndef DUEL_BET_H
#define DUEL_BET_H


class CDuelBetIem {
	SingletonInstance(CDuelBetIem);

public:
	CDuelBetIem();
	virtual ~CDuelBetIem();

	void DuelBetRequest(Player* pPlayer, Player* playerTarget);
	void DuelBetAnswer(Player* pPlayer, uint8* Packet);

	void RewardItem(Player* pPlayerWin, Player* pPlayerLose, uint8 type);
	void DeleteItemFinish(Player* pPlayer, Item item);

	void DuelBegin(Player* pPlayer, Player* playerTarget);
};

#endif // !1

