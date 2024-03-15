class IE_ID_State
{
    int version = 1;
    int last_id = 0;
    ref map<string, int> player_card_map = new map<string, int>;
}

class IE_ID_StateVersion
{
    int version;
}

class IE_ID_StateLoader
{

  static private const string DIR_PATH = "$profile:IE";
  static private const string CONFIG_PATH = DIR_PATH + "\\ID_State.json";

  ref IE_ID_State state = new IE_ID_State;

  void Load()
  {
    if (!FileExist(DIR_PATH))
    {
      MakeDirectory(DIR_PATH);
    }

    if (FileExist(CONFIG_PATH))
    {
      IE_ID_StateVersion v;
      JsonFileLoader<IE_ID_StateVersion>.JsonLoadFile(CONFIG_PATH, v);
      if (v.version == 1)
      {
        JsonFileLoader<IE_ID_State>.JsonLoadFile(CONFIG_PATH, state);
      }
      else
      {
        Print("[ID Framework] ERROR! Non recognised state version.");
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
      JsonFileLoader<ref IE_ID_State>.JsonSaveFile(CONFIG_PATH, state);
    }
  }

  int CreateCard(PlayerIdentityBase player)
  {
    state.last_id++;
    string player_id = player.GetId();
    state.player_card_map.Set(player_id, state.last_id);
    Save();
	return state.last_id;
  }
}

static ref IE_ID_StateLoader g_IE_ID_StateLoader;

static IE_ID_StateLoader GetIDStateLoader()
{
  if (!g_IE_ID_StateLoader)
  {
    g_IE_ID_StateLoader = new IE_ID_StateLoader();
    if (GetGame().IsServer())
    {
      g_IE_ID_StateLoader.Load();
    }
  }
  return g_IE_ID_StateLoader;
}

static IE_ID_State GetIDState()
{
  return GetIDStateLoader().state;
}