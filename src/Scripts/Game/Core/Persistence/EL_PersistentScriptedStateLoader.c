class EL_PersistentScriptedStateLoader<Class TScriptedState>
{
	static TScriptedState GetSingleton()
	{	
		typename saveDataType;
		if(!TypeAndSettingsValidation(saveDataType)) return null;
		
		EL_PersistenceManager persistenceManager = EL_PersistenceManager.GetInstance();
		
	    array<ref EL_DbEntity> findResults = persistenceManager.GetDbContext().FindAll(saveDataType, limit: 1).GetEntities();
	    if(!findResults || findResults.Count() != 1) return null;
	    
	    return persistenceManager.SpawnScriptedState(EL_ScriptedStateSaveDataBase.Cast(findResults.Get(0)));
	}
	
	static TScriptedState Get(string persistentId)
	{
		typename saveDataType;
		if(!TypeAndSettingsValidation(saveDataType)) return null;
		
		EL_PersistenceManager persistenceManager = EL_PersistenceManager.GetInstance();
		
	    array<ref EL_DbEntity> findResults = persistenceManager.GetDbContext().FindAll(saveDataType, EL_DbFind.Id().Equals(persistentId), limit: 1).GetEntities();
	    if(!findResults || findResults.Count() != 1) return null;
	    
	    return persistenceManager.SpawnScriptedState(EL_ScriptedStateSaveDataBase.Cast(findResults.Get(0)));
	}
	
	static array<ref TScriptedState> Get(array<string> persistentIds)
	{
		typename saveDataType;
		if(!TypeAndSettingsValidation(saveDataType)) return null;
		
		array<ref TScriptedState> resultStates();
	    
		EL_PersistenceManager persistenceManager = EL_PersistenceManager.GetInstance();
		
	    array<ref EL_DbEntity> findResults = GetDbContext().FindAll(saveDataType, EL_DbFind.Id().EqualsAnyOf(persistentIds)).GetEntities();
	    if(!findResults) return resultStates;
	    
	    foreach(EL_DbEntity findResult : findResults)
	    {
	        TScriptedState state = TScriptedState.Cast(persistenceManager.SpawnScriptedState(EL_ScriptedStateSaveDataBase.Cast(findResult)));
	        
	        if(state) resultStates.Insert(state);			
	    }
	    
	    return resultStates;
	}
	
	protected static bool TypeAndSettingsValidation(out typename saveDataType)
	{
		if(!TScriptedState.IsInherited(EL_PersistentScriptedStateBase)) return typename.Empty;

	    Tuple3<typename, bool, bool> settings = EL_ScriptedStateSaveDataType.GetSettings(TScriptedState);
		if(!settings || !settings.param1)
		{
			Debug.Error(string.Format("Scripted state type '%1' needs to have no save-data configured to be loaded!", TScriptedState));
			return typename.Empty;
		}
		
		return settings.param1;
	}
}
