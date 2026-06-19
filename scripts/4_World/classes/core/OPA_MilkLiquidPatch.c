modded class ItemBase
{
	override int GetLiquidContainerMask()
	{
		int mask = super.GetLiquidContainerMask();
		if ((mask & LIQUID_WATER) != 0)
			mask = mask | LIQUID_MILK;

		return mask;
	}

	override protected void OnLiquidTypeChanged(int oldType, int newType)
	{
		super.OnLiquidTypeChanged(oldType, newType);

		if (newType == LIQUID_MILK)
			SetColor(255, 255, 255, 255);
	}
}
