class CfgPatches
{
	class IE_IDFramework
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
	class IE_IDFramework
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
		model="IE\IDFramework\data\card\card.p3d";
		displayName="Identification Card";
		descriptionShort="A way to uniquely identify a person";
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
								"IE\IDFramework\data\card\card.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"IE\IDFramework\data\card\card.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"IE\IDFramework\data\card\card_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"IE\IDFramework\data\card\card_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"IE\IDFramework\data\card\card_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};

	class IE_IdentityCard_White: IE_IdentityCard_Base
	{
		scope=2
		displayName="White Identification Card";
		descriptionShort="A way to uniquely identify a person";
		hiddenSelections[]=
		{
			"card"
		};
		hiddenSelectionsTextures[]=
		{
			"IE\IDFramework\data\card\card_CO.paa"
		};
	}

	class IE_IdentityCard_Blue: IE_IdentityCard_Base
	{
		scope=2
		displayName="Blue Identification Card";
		descriptionShort="A way to uniquely identify a person";
		hiddenSelections[]=
		{
			"card"
		};
		hiddenSelectionsTextures[]=
		{
			"IE\IDFramework\data\card\card_blue_CO.paa"
		};
	}

	class IE_IdentityCard_Orange: IE_IdentityCard_Base
	{
		scope=2
		displayName="Orange Identification Card";
		descriptionShort="A way to uniquely identify a person";
		hiddenSelections[]=
		{
			"card"
		};
		hiddenSelectionsTextures[]=
		{
			"IE\IDFramework\data\card\card_orange_CO.paa"
		};
	}

	class IE_IdentityCard_Green: IE_IdentityCard_Base
	{
		scope=2
		displayName="Green Identification Card";
		descriptionShort="A way to uniquely identify a person";
		hiddenSelections[]=
		{
			"card"
		};
		hiddenSelectionsTextures[]=
		{
			"IE\IDFramework\data\card\card_green_CO.paa"
		};
	}

	class IE_IDTerminal_Base: Inventory_Base
	{
		scope=2;
		displayName="ID Terminal";
		descriptionShort="Allows to update identity cards";
		model="IE\IDFramework\data\computer\computer.p3d";
		slopeTolerance=0.45;
		yawPitchRollLimit[]={45,45,45};
		weight=15000;
		itemSize[]={5,5};
		itemBehaviour=0;
		heavyItem=1;
		soundImpactType="metal";
		repairableWithKits[]={5,7};
		repairCosts[]={30,25};
		hiddenSelections[]=
		{
			"terminal"
		};
		hiddenSelectionsTextures[]=
		{
			"IE\IDFramework\data\computer\Computer_CO.paa"
		};
		class EnergyManager
		{
			hasIcon=1;
			attachmentAction=1;
			energyUsagePerSecond=0.00001;
			switchOnAtSpawn=1;
			cordTextureFile="DZ\gear\camping\Data\plug_black_CO.paa";
      		updateInterval=1;
		};
		class AnimationSources
		{
			class placing
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
		};
    	class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=1;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=1;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
			};
		};
	};
};