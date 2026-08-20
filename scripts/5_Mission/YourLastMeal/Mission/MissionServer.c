modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();

		// Initialize and load server configuration
		YourLastMealConfig config = YourLastMealConfig.Get();
		if (config)
		{
			Print("[YourLastMeal] Mod loaded successfully. Config version: " + config.ConfigVersion);
		}
	}
}
