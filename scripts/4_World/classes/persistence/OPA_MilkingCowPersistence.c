class OPA_MilkingCowSaveEntry
{
	float Position[3];
	float Orientation[3];
}

class OPA_MilkingCowSaveData
{
	ref array<ref OPA_MilkingCowSaveEntry> Cows;

	void OPA_MilkingCowSaveData()
	{
		Cows = new array<ref OPA_MilkingCowSaveEntry>();
	}
}

class OPA_MilkingCowPersistence
{
	protected static ref OPA_MilkingCowPersistence m_Instance;

	protected ref array<OPA_MilkingCow> m_Cows;
	protected bool m_Initialized;
	protected bool m_Restoring;
	protected bool m_SaveQueued;

	static const string SAVE_DIR = "$profile:OPAMilking";
	static const string SAVE_FILE = "$profile:OPAMilking/cows.json";
	static const int SAVE_INTERVAL_MS = 30000;
	static const int SAVE_DELAY_MS = 5000;

	void OPA_MilkingCowPersistence()
	{
		m_Cows = new array<OPA_MilkingCow>();
	}

	static OPA_MilkingCowPersistence Get()
	{
		if (!m_Instance)
			m_Instance = new OPA_MilkingCowPersistence();

		return m_Instance;
	}

	bool IsRestoring()
	{
		return m_Restoring;
	}

	void Init()
	{
		if (!GetGame() || !GetGame().IsServer() || m_Initialized)
			return;

		m_Initialized = true;
		MakeDirectory(SAVE_DIR);
		OPA_MilkingSettings.Get();
		LoadCows();
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SaveCows, SAVE_INTERVAL_MS, true);
	}

	void RegisterCow(OPA_MilkingCow cow)
	{
		if (!cow || !GetGame() || !GetGame().IsServer())
			return;

		if (m_Cows.Find(cow) == -1)
		{
			m_Cows.Insert(cow);
			QueueSave();
		}
	}

	void UnregisterCow(OPA_MilkingCow cow)
	{
		if (!cow || !GetGame() || !GetGame().IsServer())
			return;

		int index = m_Cows.Find(cow);
		if (index != -1)
			m_Cows.Remove(index);
	}

	protected void LoadCows()
	{
		OPA_MilkingCowSaveData data = new OPA_MilkingCowSaveData();

		if (!FileExist(SAVE_FILE))
		{
			SaveCows();
			return;
		}

		string errorMessage;
		if (!JsonFileLoader<OPA_MilkingCowSaveData>.LoadFile(SAVE_FILE, data, errorMessage))
		{
			Print("[OPAMilking] Cow persistence load failed: " + errorMessage);
			return;
		}

		if (!data || !data.Cows)
			return;

		m_Restoring = true;

		foreach (OPA_MilkingCowSaveEntry entry: data.Cows)
		{
			vector position = Vector(entry.Position[0], entry.Position[1], entry.Position[2]);
			vector orientation = Vector(entry.Orientation[0], entry.Orientation[1], entry.Orientation[2]);

			OPA_MilkingCow cow = OPA_MilkingCow.Cast(GetGame().CreateObjectEx("OPA_MilkingCow", position, ECE_SETUP | ECE_CREATEPHYSICS | ECE_UPDATEPATHGRAPH | ECE_NOLIFETIME));
			if (!cow)
				continue;

			cow.SetPosition(position);
			cow.SetOrientation(orientation);
			cow.ResetMilkingState();
			RegisterCow(cow);
		}

		m_Restoring = false;
	}

	void SaveCows()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		MakeDirectory(SAVE_DIR);

		OPA_MilkingCowSaveData data = new OPA_MilkingCowSaveData();

		for (int i = m_Cows.Count() - 1; i >= 0; i--)
		{
			OPA_MilkingCow cow = m_Cows[i];
			if (!cow || cow.IsSetForDeletion())
			{
				m_Cows.Remove(i);
				continue;
			}

			vector position = cow.GetPosition();
			vector orientation = cow.GetOrientation();

			OPA_MilkingCowSaveEntry entry = new OPA_MilkingCowSaveEntry();
			entry.Position[0] = position[0];
			entry.Position[1] = position[1];
			entry.Position[2] = position[2];
			entry.Orientation[0] = orientation[0];
			entry.Orientation[1] = orientation[1];
			entry.Orientation[2] = orientation[2];
			data.Cows.Insert(entry);
		}

		string errorMessage;
		if (!JsonFileLoader<OPA_MilkingCowSaveData>.SaveFile(SAVE_FILE, data, errorMessage))
			Print("[OPAMilking] Cow persistence save failed: " + errorMessage);
	}

	protected void QueueSave()
	{
		if (m_Restoring || m_SaveQueued || !GetGame() || !GetGame().IsServer())
			return;

		m_SaveQueued = true;
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DeferredSave, SAVE_DELAY_MS, false);
	}

	protected void DeferredSave()
	{
		m_SaveQueued = false;
		SaveCows();
	}
}
