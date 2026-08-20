modded class BrainDiseaseMdfr
{
	override string GetDebugText()
	{
		YourLastMealConfig config = YourLastMealConfig.Get();
		string onsetInfo = "None";
		string suppInfo = "None";

		if (m_Player)
		{
			onsetInfo = m_Player.YLM_GetOnsetRemaining().ToString() + "s";
			suppInfo = m_Player.YLM_GetSuppressionRemaining().ToString() + "s";
		}

		return string.Format("Kuru | Onset Delay: %1 | Suppression: %2", onsetInfo, suppInfo);
	}

	override protected bool ActivateCondition(PlayerBase player)
	{
		if (!player)
			return false;

		int brainAgentCount = player.GetSingleAgentCount(eAgents.BRAIN);
		if (brainAgentCount <= 0)
			return false;

		// Update countdown during inactive checks
		player.YLM_UpdateCannibalTimers(m_TickIntervalInactive);

		YourLastMealConfig config = YourLastMealConfig.Get();
		if (!config)
			return (brainAgentCount >= AGENT_THRESHOLD_ACTIVATE);

		// If player contracted brain disease but onset timer hasn't been set yet
		if (player.YLM_GetOnsetRemaining() < 0)
		{
			player.YLM_SetOnsetRemaining(config.InitialOnsetDelaySeconds);
			player.YLM_SetHasEverContractedKuru(true);
			config.YLM_Log(string.Format("Player '%1' first contracted Kuru. Initial onset timer set to %2s.", player.GetIdentity().GetName(), config.InitialOnsetDelaySeconds));
		}

		// If onset delay is still counting down, do not activate symptoms yet
		if (player.YLM_IsOnsetPending())
		{
			return false;
		}

		return true;
	}

	override protected void OnActivate(PlayerBase player)
	{
		player.IncreaseDiseaseCount();

		YourLastMealConfig config = YourLastMealConfig.Get();
		if (config)
		{
			config.YLM_Log(string.Format("Kuru symptoms activated for player '%1'.", player.GetIdentity().GetName()));

			// Do not laugh on activation if actively suppressed
			if (player.YLM_IsSuppressed() && config.CompletelySuppressWhileActive)
				return;
		}

		player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_LAUGHTER);
	}

	override protected void OnTick(PlayerBase player, float deltaT)
	{
		if (!player)
			return;

		// Update active timers on the player
		player.YLM_UpdateCannibalTimers(deltaT);

		m_Time += deltaT;

		YourLastMealConfig config = YourLastMealConfig.Get();
		bool isSuppressed = player.YLM_IsSuppressed();

		// Complete suppression mode
		if (isSuppressed && config && config.CompletelySuppressWhileActive)
		{
			if (m_Time >= m_ShakeTime)
			{
				m_ShakeTime = m_Time + Math.RandomFloat(SHAKE_INTERVAL_MIN, SHAKE_INTERVAL_MAX);
			}
			return;
		}

		float brainAgents = player.GetSingleAgentCountNormalized(eAgents.BRAIN) / 8.0;
		float laughterMult = 1.0;
		float shakeMult = 1.0;
		float intervalMult = 1.0;

		if (isSuppressed && config)
		{
			laughterMult = config.ActiveSuppressionLaughterChanceMultiplier;
			shakeMult = config.ActiveSuppressionShakeIntensityMultiplier;
			intervalMult = config.ActiveSuppressionShakeIntervalMultiplier;
		}

		float chanceOfLaughter = Math.RandomFloat01();
		if (chanceOfLaughter < (brainAgents * laughterMult))
		{
			player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_LAUGHTER);
		}

		if (m_Time >= m_ShakeTime)
		{
			float finalShake = brainAgents * shakeMult;
			if (finalShake > 0.001)
			{
				DayZPlayerSyncJunctures.SendKuruRequest(player, finalShake);
			}

			float minInterval = SHAKE_INTERVAL_MIN * intervalMult;
			float maxInterval = SHAKE_INTERVAL_MAX * intervalMult;
			m_ShakeTime = m_Time + Math.RandomFloat(minInterval, maxInterval);
		}
	}
}
