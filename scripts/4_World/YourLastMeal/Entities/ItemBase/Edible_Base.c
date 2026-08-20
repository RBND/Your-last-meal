modded class Edible_Base
{
	override void OnConsume(float amount, PlayerBase consumer)
	{
		super.OnConsume(amount, consumer);

		if (!consumer || !GetGame().IsServer())
			return;

		YourLastMealConfig config = YourLastMealConfig.Get();
		if (!config)
			return;

		bool isCannibalFood = false;

		// 1. Check direct kind
		if (IsKindOf("HumanSteakMeat"))
		{
			isCannibalFood = true;
		}
		else if (IsKindOf("Lard") || IsKindOf("Guts"))
		{
			// Human-derived lard / guts carry the BRAIN agent
			if ((GetAgents() & eAgents.BRAIN) == eAgents.BRAIN)
			{
				isCannibalFood = true;
			}
		}

		// 2. Check config item list
		if (!isCannibalFood && config.HumanFoodClassNames)
		{
			string itemType = GetType();
			for (int i = 0; i < config.HumanFoodClassNames.Count(); i++)
			{
				string configuredClass = config.HumanFoodClassNames.Get(i);
				if (itemType == configuredClass || IsKindOf(configuredClass))
				{
					isCannibalFood = true;
					break;
				}
			}
		}

		// 3. Fallback: Check if item contains the BRAIN agent
		if (!isCannibalFood && config.AffectsAnyBrainAgentFood)
		{
			if ((GetAgents() & eAgents.BRAIN) == eAgents.BRAIN)
			{
				isCannibalFood = true;
			}
		}

		if (isCannibalFood)
		{
			consumer.YLM_OnConsumeCannibalFood(this, amount);
		}
	}
}
