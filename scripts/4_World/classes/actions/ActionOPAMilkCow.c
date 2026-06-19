class ActionOPAMilkCowCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(30.0);
	}
}

class ActionOPAMilkCow: ActionContinuousBase
{
	protected const float OPA_MILKING_DURATION = 30.0;

	void ActionOPAMilkCow()
	{
		m_CallbackClass = ActionOPAMilkCowCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_FullBody = true;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_Text = "Ordenhar vaca";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!player || !target || !item)
			return false;

		OPA_MilkingCow cow = OPA_MilkingCow.Cast(target.GetObject());
		if (!cow || cow.HasBeenMilked())
			return false;

		return IsValidMilkContainer(item);
	}

	override bool ActionConditionContinue(ActionData action_data)
	{
		OPA_MilkingCow cow = OPA_MilkingCow.Cast(action_data.m_Target.GetObject());
		return cow && !cow.HasBeenMilked() && IsValidMilkContainer(action_data.m_MainItem);
	}

	protected bool IsValidMilkContainer(ItemBase item)
	{
		if (!item || item.IsDamageDestroyed() || !item.HasQuantity())
			return false;

		if (item.IsFullQuantity())
			return false;

		if (item.GetQuantity() > 0.01 && item.GetLiquidType() != LIQUID_MILK)
			return false;

		return Liquid.CanFillContainer(item, LIQUID_MILK, true);
	}

	override string GetSoundCategory(ActionData action_data)
	{
		return "";
	}

	override void OnStartAnimationLoop(ActionData action_data)
	{
		super.OnStartAnimationLoop(action_data);

		if (!GetGame().IsMultiplayer() || GetGame().IsServer())
			SetMilkingSound(action_data, true);
	}

	override void OnEndAnimationLoop(ActionData action_data)
	{
		super.OnEndAnimationLoop(action_data);

		if (!GetGame().IsMultiplayer() || GetGame().IsServer())
			SetMilkingSound(action_data, false);
	}

	override void OnEnd(ActionData action_data)
	{
		if (!GetGame().IsMultiplayer() || GetGame().IsServer())
			SetMilkingSound(action_data, false);

		super.OnEnd(action_data);
	}

	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		FinishMilking(action_data);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);
	}

	protected void FinishMilking(ActionData action_data)
	{
		OPA_MilkingCow cow = OPA_MilkingCow.Cast(action_data.m_Target.GetObject());
		ItemBase container = action_data.m_MainItem;
		if (!cow || cow.HasBeenMilked() || !IsValidMilkContainer(container))
			return;

		float progress = GetMilkingProgress(action_data);
		if (progress <= 0)
			return;

		float capacity = container.GetQuantityMax() - container.GetQuantity();
		float amount = Math.Min(OPA_MilkingSettings.Get().MilkAmountPerMilking * progress, capacity);
		if (amount <= 0)
			return;

		cow.SetMilked();
		container.SetTemperature(20);
		Liquid.FillContainer(container, LIQUID_MILK, amount);
	}

	protected float GetMilkingProgress(ActionData action_data)
	{
		if (!action_data || !action_data.m_ActionComponent)
			return 0;

		Param1<float> spent_time = Param1<float>.Cast(action_data.m_ActionComponent.GetACData());
		if (!spent_time)
			return 0;

		return Math.Clamp(spent_time.param1 / OPA_MILKING_DURATION, 0.0, 1.0);
	}

	protected void SetMilkingSound(ActionData action_data, bool play)
	{
		OPA_MilkingCow cow = OPA_MilkingCow.Cast(action_data.m_Target.GetObject());
		if (!cow)
			return;

		GetGame().RPCSingleParam(cow, OPA_RPC_MILKING_SOUND, new Param1<bool>(play), true);
	}
}
