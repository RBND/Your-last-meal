class CfgPatches
{
	class YourLastMeal
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {
			"DZ_Data",
			"DZ_Scripts",
			"JM_CF_Scripts"
		};
	};
};

class CfgMods
{
	class YourLastMeal
	{
		dir = "Your-last-meal";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "Your Last Meal";
		credits = "";
		author = "";
		authorID = "0";
		version = "1.0";
		extra = 0;
		type = "mod";
		storageVersion = 1;

		dependencies[] = {
			"Game",
			"World",
			"Mission"
		};

		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {
					"Your-last-meal/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {
					"Your-last-meal/scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {
					"Your-last-meal/scripts/5_Mission"
				};
			};
		};
	};
};
