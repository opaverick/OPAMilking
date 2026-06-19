modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();

		if (GetGame() && GetGame().IsServer())
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(OPA_MilkingCowPersistence.Get().Init, 3000, false);
	}
}
