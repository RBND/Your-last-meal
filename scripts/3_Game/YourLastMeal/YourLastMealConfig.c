class YourLastMealConfig
{
	private static const string CONFIG_DIR = "$profile:YourLastMeal";
	private static const string CONFIG_PATH = "$profile:YourLastMeal/Config.json";
	private static ref YourLastMealConfig s_Instance;

	string ConfigVersion = "1.0";

	//! Time (in seconds) after first contracting Kuru (Brain Disease) before symptoms can kick in.
	//! Default: 1800.0 (30 minutes).
	float InitialOnsetDelaySeconds = 1800.0;

	//! Seconds of delay/suppression added when consuming 100% of a human meat portion (HumanSteakMeat).
	//! Default: 600.0 (10 minutes).
	float DelayPerFullMeatSeconds = 600.0;

	//! Seconds of delay/suppression added when consuming 100% of a human fat portion (Lard from human).
	//! Default: 900.0 (15 minutes).
	float DelayPerFullFatSeconds = 900.0;

	//! Seconds of delay/suppression added when consuming 100% of other cannibal foods (e.g. Guts).
	//! Default: 300.0 (5 minutes).
	float DelayPerFullOtherSeconds = 300.0;

	//! Maximum onset delay (in seconds) that can be accumulated before Kuru symptoms ever start.
	//! Default: 7200.0 (2 hours).
	float MaxOnsetDelaySeconds = 7200.0;

	//! Maximum suppression/relief time (in seconds) that can be accumulated once Kuru is active.
	//! Default: 3600.0 (1 hour).
	float MaxSuppressionSeconds = 3600.0;

	//! If true, laughing and trembling are completely silenced/stopped while suppression is active.
	//! Default: true.
	bool CompletelySuppressWhileActive = true;

	//! Multiplier applied to laughter chance when suppression is active (if CompletelySuppressWhileActive is false).
	//! Range: 0.0 (no laughter) to 1.0 (normal laughter).
	//! Default: 0.0.
	float ActiveSuppressionLaughterChanceMultiplier = 0.0;

	//! Multiplier applied to shake/tremor intensity when suppression is active (if CompletelySuppressWhileActive is false).
	//! Range: 0.0 (no tremors) to 1.0 (normal tremors).
	//! Default: 0.0.
	float ActiveSuppressionShakeIntensityMultiplier = 0.0;

	//! Multiplier applied to time between shakes when suppression is active.
	//! Default: 2.0 (shakes happen half as often).
	float ActiveSuppressionShakeIntervalMultiplier = 2.0;

	//! If true, any edible item containing the BRAIN agent provides relief, even if not listed in HumanFoodClassNames.
	//! Default: true.
	bool AffectsAnyBrainAgentFood = true;

	//! List of item classnames considered cannibal food.
	ref array<string> HumanFoodClassNames = new array<string>;

	//! Toggle detailed server logging for debugging.
	//! Default: false.
	bool EnableDebugLogs = false;

	void YourLastMealConfig()
	{
		InitDefaults();
	}

	void InitDefaults()
	{
		if (!HumanFoodClassNames)
			HumanFoodClassNames = new array<string>;

		if (HumanFoodClassNames.Count() == 0)
		{
			HumanFoodClassNames.Insert("HumanSteakMeat");
			HumanFoodClassNames.Insert("Lard");
			HumanFoodClassNames.Insert("Guts");
		}
	}

	static YourLastMealConfig Get()
	{
		if (!s_Instance)
		{
			s_Instance = new YourLastMealConfig();
			s_Instance.Load();
		}
		return s_Instance;
	}

	void Load()
	{
		if (!GetGame().IsServer())
			return;

		if (!FileExist(CONFIG_DIR))
			MakeDirectory(CONFIG_DIR);

		string errorMessage;
		if (FileExist(CONFIG_PATH))
		{
			if (JsonFileLoader<YourLastMealConfig>.LoadFile(CONFIG_PATH, this, errorMessage))
			{
				InitDefaults();
				YLM_Log("Successfully loaded configuration from " + CONFIG_PATH);
				return;
			}
			else
			{
				Error("[YourLastMeal] Failed to parse config file: " + errorMessage);
			}
		}

		// If file doesn't exist or loading failed, save current defaults
		Save();
	}

	void Save()
	{
		if (!GetGame().IsServer())
			return;

		if (!FileExist(CONFIG_DIR))
			MakeDirectory(CONFIG_DIR);

		string errorMessage;
		if (JsonFileLoader<YourLastMealConfig>.SaveFile(CONFIG_PATH, this, errorMessage))
		{
			YLM_Log("Saved configuration to " + CONFIG_PATH);
		}
		else
		{
			Error("[YourLastMeal] Failed to save config file: " + errorMessage);
		}
	}

	void YLM_Log(string message)
	{
		if (EnableDebugLogs)
		{
			Print("[YourLastMeal] " + message);
		}
	}
}
