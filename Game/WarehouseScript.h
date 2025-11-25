#ifndef WAREHOUSE_SCRIPT_H
#define WAREHOUSE_SCRIPT_H

class WarehouseScript: public StoreScript
{
	public:
		explicit WarehouseScript(Player* pPlayer): StoreScript(pPlayer, warehouse_size, warehouse_size) {}

                uint32 m_Zen;
                uint16 m_Password;
                bool m_Locked;
                uint8 m_Expanded;
                time_t m_ExpandedTime;

		void LoadDBData(PreparedQueryResult result);
		void LoadDBDataNew();
		void LoadDBItemListNew();
		void Open();

		void SaveDBData(SQLTransaction & trans);

		void Clear();

		uint8 RectCheck(uint8 x, uint8 y, uint8 width, uint8 height) const;
		void ItemSet(uint8 slot, uint8 set);

		void DeleteItem(uint8 slot);

		bool MoneyReachMaximum(uint32 ammount) const;
		void MoneyAdd(uint32 ammount);
		void MoneyReduce(uint32 ammount);
		bool MoneyHave(uint32 ammount);

		void AssignItem(Item const& add_item, uint8 slot);

		uint8 AddItem(Item const& item, uint8 slot) override;
		void WarehouseSaveItemDbBinary();
};

#endif