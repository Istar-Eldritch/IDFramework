class IE_ID_IdentityState
{
  int id;
  string name;
  string role;
  string discord_id;
}


class IE_ID_State
{
    int version = 1;
    ref map<string, ref IE_ID_IdentityState> player_id_map = new map<string, ref IE_ID_IdentityState>;
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
  ref map<int, string> m_cardIdIdx = new map<int, string>;
  ref array<int> ids = new array<int>;

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
        foreach( string key, auto value: state.player_id_map)
        {
          m_cardIdIdx.Set(value.id, key);
          auto match = new IE_SearchMatch<int>(value.id);
          IE_BinSearchResult result = IE_ArrayUtils<int>.BinarySearch(ids, match);
          if (!result.success)
          {
            ids.InsertAt(result.idx, value.id);	
          }
        }
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
	
  int GenRandomID()
  {
	while(true)
	{
		int guess = Math.RandomInt(0, 100000);
		auto match = new IE_SearchMatch<int>(guess);
		IE_BinSearchResult result = IE_ArrayUtils<int>.BinarySearch(ids, match);
		if (!result.success)
		{
			ids.InsertAt(result.idx, guess);
			return guess;
		}
	}
	return -1;
  }

  IE_ID_IdentityState CreateOrUpdateIdentity(string playerName, string playerId, string roleName, string discordId)
  {
	int id;
	auto existing = state.player_id_map.Get(playerId);
	if (existing != null)
	{
		id = existing.id;
	}
	else
	{
		id = GenRandomID();
	}

    IE_ID_IdentityState identity = new IE_ID_IdentityState;
    identity.id = id;
    identity.name = playerName;
    identity.role = roleName;
    identity.discord_id = discordId;
    state.player_id_map.Set(playerId, identity);
	m_cardIdIdx.Set(id, playerId);

    Save();
	return identity;
  }
	
  IE_ID_IdentityState GetIdentity(string playerId)
  {
	  return state.player_id_map.Get(playerId);
  }
	
  IE_ID_IdentityState GetIdentityByCardId(int cardId)
  {
	string playerId = m_cardIdIdx.Get(cardId);
	if (playerId)
	{
		return GetIdentity(playerId);
	}
	return null;
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