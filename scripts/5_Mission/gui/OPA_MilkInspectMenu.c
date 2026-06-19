modded class InspectMenuNew
{
	override static void UpdateItemInfoLiquidType(Widget root_widget, EntityAI item)
	{
		ItemBase item_base = ItemBase.Cast(item);
		if (item_base && item_base.GetQuantity() > 0 && item_base.IsLiquidContainer() && item_base.GetLiquidType() == LIQUID_MILK)
		{
			WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "Leite", Colors.COLOR_LIQUID);
			return;
		}

		super.UpdateItemInfoLiquidType(root_widget, item);
	}
}

modded class ItemManager
{
	override void PrepareTooltip(EntityAI item, int x = 0, int y = 0)
	{
		super.PrepareTooltip(item, x, y);

		if (item && m_TooltipWidget && !IsDragging())
			InspectMenuNew.UpdateItemInfoLiquidType(m_TooltipWidget, item);
	}
}
