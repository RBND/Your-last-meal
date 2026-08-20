# Your Last Meal (DayZ Mod)

**Your Last Meal** is a gameplay mod for DayZ that alters the progression and management of Kuru (Brain Disease). 

In vanilla DayZ, once a survivor eats human meat or human fat, they inevitably contract Kuru and suffer irreversible bouts of uncontrollable laughter and weapon tremors. With **Your Last Meal**, survivors can delay the onset of symptoms and temporarily suppress/reduce the effects of Kuru by continuing to consume human meat or human fat.

---

## Features

- **Configurable Onset Delay**: When a survivor first contracts Kuru, symptoms do not begin immediately. A countdown timer delays the initial symptoms (default: 30 minutes).
- **Postpone Symptoms by Feeding**: Consuming more human meat or human fat during the onset period extends the delay timer (up to a configurable maximum).
- **Cannibal Satiation / Symptom Relief**: Once Kuru is active (laughing and shaking), consuming human meat or human fat provides a period of temporary relief:
  - **Laughter Suppression**: Can be completely silenced or have its probability reduced.
  - **Tremor Suppression**: Shaking intensity can be silenced or reduced, and time between spasms increased.
- **Dynamic Portion Scaling**: Delay/relief duration scales proportionally based on how much of the food item is consumed (eating half a steak grants half the duration).
- **Multiplayer & Disconnect Persistence**: Timers are saved to the player character and survive server restarts and reconnections.
- **Easy JSON Server Configuration**: Stored at `$profile:YourLastMeal/Config.json` and generated automatically on first launch.

---

## Configuration

When the server starts with the mod loaded, a configuration file is automatically created at:
`profiles/YourLastMeal/Config.json` (or `serverprofile/YourLastMeal/Config.json`)

### Example `Config.json`

```json
{
  "ConfigVersion": "1.0",
  "InitialOnsetDelaySeconds": 1800.0,
  "DelayPerFullMeatSeconds": 600.0,
  "DelayPerFullFatSeconds": 900.0,
  "DelayPerFullOtherSeconds": 300.0,
  "MaxOnsetDelaySeconds": 7200.0,
  "MaxSuppressionSeconds": 3600.0,
  "CompletelySuppressWhileActive": true,
  "ActiveSuppressionLaughterChanceMultiplier": 0.0,
  "ActiveSuppressionShakeIntensityMultiplier": 0.0,
  "ActiveSuppressionShakeIntervalMultiplier": 2.0,
  "AffectsAnyBrainAgentFood": true,
  "HumanFoodClassNames": [
    "HumanSteakMeat",
    "Lard",
    "Guts"
  ],
  "EnableDebugLogs": false
}
```

### Parameter Reference

| Setting | Default | Description |
| :--- | :---: | :--- |
| `InitialOnsetDelaySeconds` | `1800.0` (30m) | Time in seconds after first contracting Kuru before symptoms activate. |
| `DelayPerFullMeatSeconds` | `600.0` (10m) | Seconds added to delay/suppression for consuming 100% of human steak (`HumanSteakMeat`). |
| `DelayPerFullFatSeconds` | `900.0` (15m) | Seconds added to delay/suppression for consuming 100% of human fat (`Lard`). |
| `DelayPerFullOtherSeconds` | `300.0` (5m) | Seconds added to delay/suppression for other cannibal foods (e.g., `Guts`). |
| `MaxOnsetDelaySeconds` | `7200.0` (2h) | Maximum onset delay time that can be accumulated before symptoms begin. |
| `MaxSuppressionSeconds` | `3600.0` (1h) | Maximum active suppression / relief time that can be accumulated. |
| `CompletelySuppressWhileActive` | `true` | When `true`, completely halts both laughter and tremors while relief is active. |
| `ActiveSuppressionLaughterChanceMultiplier` | `0.0` | Multiplier for laughter probability during relief if `CompletelySuppressWhileActive` is `false` (0.0 = 0%, 0.5 = 50%). |
| `ActiveSuppressionShakeIntensityMultiplier` | `0.0` | Multiplier for shake intensity during relief if `CompletelySuppressWhileActive` is `false`. |
| `ActiveSuppressionShakeIntervalMultiplier` | `2.0` | Multiplier for time between shakes during relief (2.0 = shakes occur half as frequently). |
| `AffectsAnyBrainAgentFood` | `true` | If `true`, any food/drink containing `eAgents.BRAIN` provides relief, even if not in the classname list. |
| `HumanFoodClassNames` | `["HumanSteakMeat", "Lard", "Guts"]` | Array of item classnames considered cannibal food. |
| `EnableDebugLogs` | `false` | Enables detailed logging in the server console (RPT log) for admin troubleshooting. |

---

## Installation & Packing

### Building the Mod
1. Open **DayZ Tools** -> **Addon Builder**.
2. Set **Source directory**: `Your-last-meal`
3. Set **Destination directory**: `@YourLastMeal\Addons`
4. Set **PBO name**: `YourLastMeal.pbo`
5. Ensure `*.c;*.cpp;*.json;*.h;*.hpp;*.paa;*.rvmat;*.p3d` are in list of files to copy.
6. Click **Pack**.

### Server Setup
1. Copy the `@YourLastMeal` folder to your DayZ Server root directory.
2. Copy the mod's `.bikey` file to the server's `keys` folder.
3. Add `-mod=@YourLastMeal` to your server startup command line or batch file.
4. Launch the server to generate `YourLastMeal/Config.json` inside your server's profile directory.
5. Customize values as desired and restart the server.
