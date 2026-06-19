class OPA_MilkingCow: BuildingSuper
{
	protected int m_MilkingCount;
	protected bool m_PositionAdjusted;
	protected ref EffectSound m_MilkingLoopSound;
	protected ref EffectSound m_MooSound;

	void OPA_MilkingCow()
	{
		RegisterNetSyncVariableInt("m_MilkingCount", 0, 100);
	}

	override void EEInit()
	{
		super.EEInit();

		if (!OPA_MilkingCowPersistence.Get().IsRestoring())
			AdjustGroundPosition();

		if (GetGame() && GetGame().IsServer())
		{
			OPA_MilkingCowPersistence.Get().RegisterCow(this);
			ScheduleNextMoo();
		}
	}

	override void EEDelete(EntityAI parent)
	{
		StopMilkingSound();
		StopMooSound();
		if (GetGame() && GetGame().IsServer())
			OPA_MilkingCowPersistence.Get().UnregisterCow(this);
		super.EEDelete(parent);
	}

	override bool IsBuilding()
	{
		return false;
	}

	bool HasBeenMilked()
	{
		return m_MilkingCount >= OPA_MilkingSettings.Get().MilkingsPerCowRestart;
	}

	void SetMilked()
	{
		if (HasBeenMilked())
			return;

		m_MilkingCount++;
		SetSynchDirty();
	}

	void ResetMilkingState()
	{
		m_MilkingCount = 0;
		SetSynchDirty();
	}

	override int GetLiquidSourceType()
	{
		return LIQUID_MILK;
	}

	override float GetLiquidThroughputCoef()
	{
		return 1.0;
	}

	protected void AdjustGroundPosition()
	{
		if (m_PositionAdjusted || !GetGame())
			return;

		vector pos = GetPosition();
		float surfaceY = GetGame().SurfaceY(pos[0], pos[2]);
		SetPosition(Vector(pos[0], surfaceY + 0.85, pos[2]));
		m_PositionAdjusted = true;
	}

	protected void ScheduleNextMoo()
	{
		if (!GetGame() || !GetGame().IsServer() || IsSetForDeletion())
			return;

		int delay = Math.RandomIntInclusive(45000, 140000);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DoRandomMoo, delay, false);
	}

	protected void DoRandomMoo()
	{
		if (!GetGame() || !GetGame().IsServer() || IsSetForDeletion())
			return;

		GetGame().RPCSingleParam(this, OPA_RPC_COW_MOO, new Param1<string>(GetRandomMooSoundSet()), true);
		ScheduleNextMoo();
	}

	protected string GetRandomMooSoundSet()
	{
		int roll = Math.RandomInt(0, 5);
		if (roll == 0) return "CattleMooA_SoundSet";
		if (roll == 1) return "CattleMooB_SoundSet";
		if (roll == 2) return "CattleMooC_SoundSet";
		if (roll == 3) return "CattleSigh_SoundSet";
		return "CattleBreath_SoundSet";
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (rpc_type == OPA_RPC_MILKING_SOUND)
		{
			Param1<bool> soundState;
			if (ctx.Read(soundState))
			{
				if (soundState.param1)
					PlayMilkingSound();
				else
					StopMilkingSound();
			}
			return;
		}

		if (rpc_type == OPA_RPC_COW_MOO)
		{
			Param1<string> mooData;
			if (ctx.Read(mooData))
				PlayMooSound(mooData.param1);
		}
	}

	protected void PlayMilkingSound()
	{
		if (GetGame().IsDedicatedServer())
			return;

		StopMilkingSound();
		PlaySoundSetLoop(m_MilkingLoopSound, "OPA_MilkingCow_Action_SoundSet", 0.2, 0.2);
	}

	protected void StopMilkingSound()
	{
		if (m_MilkingLoopSound)
			StopSoundSet(m_MilkingLoopSound);
	}

	protected void PlayMooSound(string soundSet)
	{
		if (GetGame().IsDedicatedServer() || soundSet == "")
			return;

		StopMooSound();
		PlaySoundSet(m_MooSound, soundSet, 0.1, 0.1);
	}

	protected void StopMooSound()
	{
		if (m_MooSound)
			StopSoundSet(m_MooSound);
	}
}
