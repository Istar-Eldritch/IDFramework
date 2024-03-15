class CfgPatches
{
	class IE_ID
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
			{
				"DZ_Data",
				"DZ_Scripts"
			};
	};
};

class CfgMods
{
	class IE_ID
	{
		type = "mod";
		author = "Istar Eldritch";
    	dependencies[] = { "Game", "World", "Mission" };
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"IE/IDFramework/scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"IE/IDFramework/scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"IE/IDFramework/scripts/5_Mission"};
			};
		};
	};
};

class CfgVehicles
{
	class Inventory_Base;
	class IE_IdentityCard_Base: Inventory_Base
	{
		scope=0;
		model="IE\IDFramework\data\card.p3d";
		displayName="Identification Card";
		descriptionShort="A way to uniquely identify a person";
		hiddenSelections[]=
		{
			"card"
		};
		hiddenSelectionsTextures[]=
		{
			"IE\IDFramework\data\card_CO.paa"
		};
		weight=10;
		itemSize[]={1,1};
		rotationFlags=1;
		soundImpactType="plastic";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=20;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"IE\IDFramework\data\card.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"IE\IDFramework\data\card.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"IE\IDFramework\data\card_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"IE\IDFramework\data\card_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"IE\IDFramework\data\card_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};

	class IE_IdentityCard_Default: IE_IdentityCard_Base
	{
		scope=2
		displayName="Identification Card";
		descriptionShort="A way to uniquely identify a person";
		hiddenSelections[]=
		{
			"card"
		};
		hiddenSelectionsTextures[]=
		{
			"IE\IDFramework\data\card_CO.paa"
		};
	}
};