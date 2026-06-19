class OPA_MilkingSettingsData
{
	float MilkAmountPerMilking;
	int MilkingsPerCowRestart;

	void OPA_MilkingSettingsData()
	{
		MilkAmountPerMilking = OPA_DEFAULT_MILK_AMOUNT_PER_MILKING;
		MilkingsPerCowRestart = OPA_DEFAULT_MILKINGS_PER_COW_RESTART;
	}
}

class OPA_MilkingSettings
{
	protected static ref OPA_MilkingSettingsData m_Data;

	static const string SETTINGS_DIR = "$profile:OPAMilking";
	static const string SETTINGS_FILE = "$profile:OPAMilking/settings.json";

	static OPA_MilkingSettingsData Get()
	{
		if (!m_Data)
			Load();

		return m_Data;
	}

	static void Load()
	{
		m_Data = new OPA_MilkingSettingsData();

		if (!GetGame() || !GetGame().IsServer())
			return;

		MakeDirectory(SETTINGS_DIR);

		if (FileExist(SETTINGS_FILE))
		{
			string loadError;
			if (!JsonFileLoader<OPA_MilkingSettingsData>.LoadFile(SETTINGS_FILE, m_Data, loadError))
				m_Data = new OPA_MilkingSettingsData();
		}

		Validate();

		string saveError;
		JsonFileLoader<OPA_MilkingSettingsData>.SaveFile(SETTINGS_FILE, m_Data, saveError);
	}

	protected static void Validate()
	{
		if (m_Data.MilkAmountPerMilking < 1.0)
			m_Data.MilkAmountPerMilking = OPA_DEFAULT_MILK_AMOUNT_PER_MILKING;

		if (m_Data.MilkingsPerCowRestart < 1)
			m_Data.MilkingsPerCowRestart = OPA_DEFAULT_MILKINGS_PER_COW_RESTART;
	}
}
