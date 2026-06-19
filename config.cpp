#define OPA_MILK_FULLNESS_INDEX 1
#define OPA_MILK_ENERGY 500
#define OPA_MILK_WATER 100
#define OPA_MILK_NUTRITIONAL_INDEX 80
#define OPA_MILK_TOXICITY 0
#define OPA_MILK_DIGESTIBILITY 2

class CfgPatches
{
	class OPAMilking
	{
		units[] =
		{
			"OPA_MilkingCow"
		};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Animals_bos_taurus_fem",
			"DZ_Gear_Drinks",
			"DZ_Gear_Cooking",
			"DZ_Gear_Containers"
		};
	};
};

class CfgMods
{
	class OPAMilking
	{
		dir = "OPAMilking";
		name = "OPA Milking";
		type = "mod";
		author = "Opaverick";
		version = "0.1.0";
		dependencies[] =
		{
			"Game",
			"World",
			"Mission"
		};

		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] =
				{
					"OPAMilking/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"OPAMilking/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"OPAMilking/scripts/5_Mission"
				};
			};
		};
	};
};

class cfgLiquidDefinitions
{
	class Milk
	{
		type = 8388608;
		displayName = "Leite";
		name = "Leite";
		flammability = -10;
		liquidFreezeThreshold = -1;
		liquidThawThreshold = -1;
		liquidBoilingThreshold = 100;

		class Nutrition
		{
			fullnessIndex = OPA_MILK_FULLNESS_INDEX;
			energy = OPA_MILK_ENERGY;
			water = OPA_MILK_WATER;
			nutritionalIndex = OPA_MILK_NUTRITIONAL_INDEX;
			toxicity = OPA_MILK_TOXICITY;
			digestibility = OPA_MILK_DIGESTIBILITY;
		};
	};
};

class CfgSoundShaders
{
	class OPA_MilkingCow_Action_SoundShader
	{
		samples[] =
		{
			{"OPAMilking\sounds\milking_cow", 1}
		};
		volume = 0.8;
		range = 18;
	};
};

class CfgSoundSets
{
	class OPA_MilkingCow_Action_SoundSet
	{
		soundShaders[] =
		{
			"OPA_MilkingCow_Action_SoundShader"
		};
		volumeFactor = 1;
		frequencyFactor = 1;
		spatial = 1;
		loop = 1;
	};
};

class CfgVehicles
{
	class HouseNoDestruct;

	class OPA_MilkingCow: HouseNoDestruct
	{
		scope = 2;
		displayName = "Vaca";
		descriptionShort = "Vaca leiteira estatica. Pode ser ordenhada uma vez por restart.";
		model = "\dz\animals\bos_taurus_fem\bos_taurus_fem.p3d";
		hiddenSelections[] =
		{
			"Camo"
		};
		hiddenSelectionsTextures[] =
		{
			"dz\animals\bos_taurus_fem\data\cow_CO.paa"
		};
		hiddenSelectionsMaterials[] =
		{
			"dz\animals\bos_taurus_fem\data\cow.rvmat"
		};
		physLayer = "item_large";
		carveNavmesh = 0;
		canBeDigged = 0;
		weight = 500000;

		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100000;
				};
			};
		};
	};
};
