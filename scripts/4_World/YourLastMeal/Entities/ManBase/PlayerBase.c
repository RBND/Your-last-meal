modded class PlayerBase
{
	protected float m_YLM_OnsetRemaining = -1.0;
	protected float m_YLM_SuppressionRemaining = 0.0;
	protected bool m_YLM_HasEverContractedKuru = false;

	void YLM_OnConsumeCannibalFood(Edible_Base food, float consumedAmount)
	{
		if (!GetGame().IsServer() || !food)
			return;

		YourLastMealConfig config = YourLastMealConfig.Get();
		if (!config)
			return;

		float baseDelaySeconds = config.DelayPerFullOtherSeconds;
		if (food.IsKindOf("HumanSteakMeat"))
		{
			baseDelaySeconds = config.DelayPerFullMeatSeconds;
		}
		else if (food.IsKindOf("Lard"))
		{
			baseDelaySeconds = config.DelayPerFullFatSeconds;
		}

		float maxQty = food.GetQuantityMax();
		if (maxQty <= 0)
			maxQty = 100.0;

		float proportion = Math.Clamp(consumedAmount / maxQty, 0.0, 1.0);
		float addedSeconds = baseDelaySeconds * proportion;

		// Check if Brain Disease is already active
		bool isBrainDiseaseActive = false;
		if (m_ModifiersManager && m_ModifiersManager.IsModifierActive(eModifiers.MDF_BRAIN))
		{
			isBrainDiseaseActive = true;
		}

		string playerName = "Unknown";
		if (GetIdentity())
			playerName = GetIdentity().GetName();

		if (isBrainDiseaseActive)
		{
			// Symptoms already active: add relief / symptom suppression
			m_YLM_SuppressionRemaining = Math.Clamp(m_YLM_SuppressionRemaining + addedSeconds, 0.0, config.MaxSuppressionSeconds);
			config.YLM_Log(string.Format("Player '%1' consumed %2g of %3. Added %4s to active suppression. (Total remaining: %5s)", playerName, consumedAmount, food.GetType(), addedSeconds, m_YLM_SuppressionRemaining));
		}
		else
		{
			// Symptoms not active yet: add to onset delay
			if (m_YLM_OnsetRemaining < 0)
			{
				m_YLM_OnsetRemaining = config.InitialOnsetDelaySeconds;
			}

			m_YLM_OnsetRemaining = Math.Clamp(m_YLM_OnsetRemaining + addedSeconds, 0.0, config.MaxOnsetDelaySeconds);
			m_YLM_HasEverContractedKuru = true;
			config.YLM_Log(string.Format("Player '%1' consumed %2g of %3. Added %4s to onset delay. (Total remaining: %5s)", playerName, consumedAmount, food.GetType(), addedSeconds, m_YLM_OnsetRemaining));
		}
	}

	void YLM_UpdateCannibalTimers(float deltaT)
	{
		if (m_YLM_OnsetRemaining > 0)
		{
			m_YLM_OnsetRemaining -= deltaT;
			if (m_YLM_OnsetRemaining < 0)
				m_YLM_OnsetRemaining = 0;
		}

		if (m_YLM_SuppressionRemaining > 0)
		{
			m_YLM_SuppressionRemaining -= deltaT;
			if (m_YLM_SuppressionRemaining < 0)
				m_YLM_SuppressionRemaining = 0;
		}
	}

	bool YLM_IsOnsetPending()
	{
		return (m_YLM_OnsetRemaining > 0);
	}

	float YLM_GetOnsetRemaining()
	{
		return m_YLM_OnsetRemaining;
	}

	void YLM_SetOnsetRemaining(float time)
	{
		m_YLM_OnsetRemaining = time;
	}

	bool YLM_IsSuppressed()
	{
		return (m_YLM_SuppressionRemaining > 0);
	}

	float YLM_GetSuppressionRemaining()
	{
		return m_YLM_SuppressionRemaining;
	}

	void YLM_SetSuppressionRemaining(float time)
	{
		m_YLM_SuppressionRemaining = time;
	}

	bool YLM_HasEverContractedKuru()
	{
		return m_YLM_HasEverContractedKuru;
	}

	void YLM_SetHasEverContractedKuru(bool val)
	{
		m_YLM_HasEverContractedKuru = val;
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		ctx.Write(m_YLM_OnsetRemaining);
		ctx.Write(m_YLM_SuppressionRemaining);
		ctx.Write(m_YLM_HasEverContractedKuru);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		if (!ctx.Read(m_YLM_OnsetRemaining))
			return false;

		if (!ctx.Read(m_YLM_SuppressionRemaining))
			return false;

		if (!ctx.Read(m_YLM_HasEverContractedKuru))
			return false;

		return true;
	}
}
