class IE_ID_Config
{
    int version = 1;
	string discordGuidID;
    string discordBotURL="http://localhost";
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