class IE_ID_RoleConfig
{
  string name;
  string card;
  ref array<string> loadout;
  ref array<array<float>> spawnpoints;
  ref array<string> permissions = new array<string>;
  bool spawns_with_card = false;
}

class IE_ID_Config
{
 	int version = 1;
	string discordGuildID;
  string discordBotToken;
  string proxyUrl="https://dayz-proxy.eldritchist.dev";
	string proxyToken="abc123test";
	bool kick_not_registered = true;
	bool kick_no_matching_role = true;
	int terminal_session_duration = 30;
  	ref array<ref IE_ID_RoleConfig> roles = new array<ref IE_ID_RoleConfig>;
}

class IE_ID_ConfigVersion
{
    int version;
}

class IE_ID_ConfigLoader
{

  static private const string DIR_PATH = "$profile:IE";
  static private const string CONFIG_PATH = DIR_PATH + "\\ID_Config.json";

  ref IE_ID_Config config = new IE_ID_Config;

  ref map<string, int> role_idx;
	
  void Load()
  {
    if (!FileExist(DIR_PATH))
    {
      MakeDirectory(DIR_PATH);
    }

    if (FileExist(CONFIG_PATH))
    {
      IE_ID_ConfigVersion v;
      JsonFileLoader<IE_ID_ConfigVersion>.JsonLoadFile(CONFIG_PATH, v);
      if (v.version == 1)
      {
        JsonFileLoader<IE_ID_Config>.JsonLoadFile(CONFIG_PATH, config);
		BuildIndex();
      }
      else
      {
        Print("[IE_ID] ERROR! Non recognised config version.");
      }
    }
    else
    {
      Save();
    }
  }
	
  protected void BuildIndex()
  {
		role_idx = new map<string, int>;
		for(int idx = 0; idx < config.roles.Count(); idx++)
		{
			role_idx.Set(config.roles[idx].name, idx);
		}
  }
	
  IE_ID_RoleConfig GetRoleByName(string roleName)
  {
	int idx = role_idx.Get(roleName);
	return config.roles[idx];
  }

  void Save()
  {
    if (GetGame().IsServer())
    {
      JsonFileLoader<ref IE_ID_Config>.JsonSaveFile(CONFIG_PATH, config);
    }
  }

  void RequestConfiguration()
  {
    GetRPCManager().SendRPC("IE_ID", "RequestConfigRPC", null, true);
  }

  void RequestConfigRPC(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
  {
    Param1<IE_ID_Config> data;
    if (ctx.Read(data))
    {
        config = data.param1;
		BuildIndex();
    }
    else if (GetGame().IsServer())
    {
      GetRPCManager().SendRPC("IE_ID", "RequestConfigRPC", new Param1<IE_ID_Config>(config), true, sender, target);
    }
  }
}

static ref IE_ID_ConfigLoader g_IE_ID_ConfigLoader;

static IE_ID_ConfigLoader GetIDConfigLoader()
{
  if (!g_IE_ID_ConfigLoader)
  {
    g_IE_ID_ConfigLoader = new IE_ID_ConfigLoader();
    if (GetGame().IsServer())
    {
      g_IE_ID_ConfigLoader.Load();
    }
  }
  return g_IE_ID_ConfigLoader;
}

static IE_ID_Config GetIDConfig()
{
  return GetIDConfigLoader().config;
}