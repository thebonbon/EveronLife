class EL_Utils : EPF_Utils
{
	static ResourceName PLACEHOLDER_ICON = "{AC7E384FF9D8016A}Common/Textures/placeholder_BCR.edds";
	static ref RandomGenerator EL_RANDOM_GENERATOR = new RandomGenerator();
	
	//------------------------------------------------------------------------------------------------
	static string GetPlayerName(string playerUid)
	{
		return GetPlayerName(GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(GetPlayerByUID(playerUid)));
	}
	
	//------------------------------------------------------------------------------------------------
	static string GetPlayerName(IEntity player)
	{
		return GetPlayerName(GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(player));
	}
	
	//------------------------------------------------------------------------------------------------
	static string GetPlayerName(int playerId)
	{
		return GetGame().GetPlayerManager().GetPlayerName(playerId);
	}

	//------------------------------------------------------------------------------------------------
	static void ChangeColorRecursive(IEntity parent, Color color)
	{
		IEntity child = parent.GetChildren();
		while (child)
		{
			if (child.GetChildren())
				ChangeColorRecursive(child, color);

			SetColor(child, color);
			child = child.GetSibling();
		}
	}

	//------------------------------------------------------------------------------------------------
	static void SetSlotsColor(notnull IEntity entity, int color)
	{
		SlotManagerComponent slotManager = EPF_Component<SlotManagerComponent>.Find(entity);
		if (!slotManager)
			return;
		array<EntitySlotInfo> slots = new array<EntitySlotInfo>;
		slotManager.GetSlotInfos(slots);
		foreach (EntitySlotInfo slotInfo : slots)
		{
			IEntity slotEnt = slotInfo.GetAttachedEntity();
			if (!slotEnt)
				continue;

			EL_VehicleAppearanceComponent slotAppearance = EPF_Component<EL_VehicleAppearanceComponent>.Find(slotEnt);
			if (slotAppearance)
				slotAppearance.SetVehicleColor(color);
		}
	}

	//------------------------------------------------------------------------------------------------
	//! Creates and sets new material with given color to entity
	//! \param entity Entity to set new color material
	//! \param color Color for the new material
	static void SetColor(notnull IEntity entity, Color color)
	{
		
		VObject mesh = entity.GetVObject();
			
		if (!mesh || !color)
			return;

		string materialName = "vehicle_material_" + entity;

		float materialColorRGBA[] = { color.R(), color.G(), color.B(), color.A() };

		
		Material dynamicColorMaterial = Material.Create(materialName, "MatPBRMulti");
		
		dynamicColorMaterial.SetParam("Color", materialColorRGBA);
		
		string matName;
		dynamicColorMaterial.GetName(matName);
		
		string remap;
		string materials[256];
		
		int numMats = mesh.GetMaterials(materials);
		if (numMats == 0)
			return;

		for (int i = 0; i < numMats; i++)
		{
			string originalMatName = materials[i];
			if (originalMatName.EndsWith("Body") || originalMatName.EndsWith("Base") || originalMatName.EndsWith("Body_Roof"))
			{
				
				remap += string.Format("$remap '%1' '%2';", materials[i], matName);
			}
		}
		entity.SetObject(mesh, remap);
	}


	//------------------------------------------------------------------------------------------------
	//! Finds Player Entity with UID
	//! \param uid player uid
	//! \return the player entity
	static IEntity GetPlayerByUID(string uid)
	{
		array<int> players = {};
		for (int i = 0, count = GetGame().GetPlayerManager().GetPlayers(players); i < count; i++)
		{
			if (GetPlayerUID(players[i]) == uid)
				return GetGame().GetPlayerManager().GetPlayerControlledEntity(players[i]);
		}
		return null;
	}

	//------------------------------------------------------------------------------------------------
	//! Spawns a prefab
	//! \param prefab ResournceName of the prefab to be spawned
	//! \param origin Position(origin) where to spawn the entity
	//! \param orientation Angles(yaw, pitch, rolle in degrees) to apply to the entity
	//! \return the spawned entity or null on failure
	
	static IEntity SpawnEntityPrefabParent(ResourceName prefab, vector origin, vector orientation = "0 0 0", IEntity parent = null, bool global = true)
	{
		EntitySpawnParams spawnParams();

		spawnParams.TransformMode = ETransformMode.WORLD;

		Math3D.AnglesToMatrix(orientation, spawnParams.Transform);
		spawnParams.Transform[3] = origin;
		spawnParams.Parent = parent;

		IEntity newEnt;
		if (!global)
			newEnt = GetGame().SpawnEntityPrefabLocal(Resource.Load(prefab), GetGame().GetWorld(), spawnParams);
		else
			newEnt = GetGame().SpawnEntityPrefab(Resource.Load(prefab), GetGame().GetWorld(), spawnParams);

		if (parent)
			parent.AddChild(newEnt, -1);
		return newEnt;
	}

	//------------------------------------------------------------------------------------------------
	//! Teleport an entity
	//! \param entity Entity instance to be teleported
	//! \param position Position where to teleport to
	//! \param ypr Angles(yaw, pitch, rolle in degrees) to apply after teleportation
	//! \param scale Transformation scale to apply after teleportation
	static void Teleport(IEntity entity, vector position, vector ypr = "-1 -1 -1", float scale = -1)
	{
		vector transform[4];

		if (ypr != "-1 -1 -1")
		{
			Math3D.AnglesToMatrix(ypr, transform);
			transform[3] = position;
		}
		else
		{
			entity.GetWorldTransform(transform);
			transform[3] = position;
			SCR_TerrainHelper.OrientToTerrain(transform);
		}

		if (scale != -1) Math3D.MatrixScale(transform, scale);

		TeleportTM(entity, transform);
	}

	//------------------------------------------------------------------------------------------------
	//! Teleport an entity
	//! \param entity Entity instance to be teleported
	//! \param transform Target transformation matrix
	static void TeleportTM(IEntity entity, vector transform[4])
	{
		BaseGameEntity baseGameEntity = BaseGameEntity.Cast(entity);
		if (baseGameEntity)
		{
			baseGameEntity.Teleport(transform);
		}
		else
		{
			entity.SetWorldTransform(transform);
		}

		Physics physics = entity.GetPhysics();
		if (physics)
		{
			physics.SetVelocity(vector.Zero);
			physics.SetAngularVelocity(vector.Zero);
		}

		if (!ChimeraCharacter.Cast(entity))
		{
			entity.Update();
		}
	}

	//------------------------------------------------------------------------------------------------
	//! Convert integer to hexadeciamal string
	//! \param value Input integer
	//! \param upperCase Decide if output should be upper or lower case
	//! \param fixedLength Add leading zeros for a minimum length output
	//! \return result hexadecimal string
	static string IntToHex(int value, bool upperCase = false, int fixedLength = -1)
	{
		array<string> resultChars = {"0", "0", "0", "0", "0", "0", "0", "0"};

		int asciiOffset = 87;
		if (upperCase) asciiOffset = 55;

		int padUntil = 7;
		if (fixedLength != -1) padUntil = 8 - Math.Min(fixedLength, 8);

		int resultIdx = 7;

		while (value)
		{
			int remainder = value % 16;

			if (remainder < 10)
			{
				resultChars.Set(resultIdx--, remainder.ToString());
			}
			else
			{
				resultChars.Set(resultIdx--, (remainder + asciiOffset).AsciiToString());
			}

			value /= 16;
		}

		string result;
		bool nonZero;

		foreach (int nChar, string char : resultChars)
		{
			if (char == "0" && nChar < padUntil && !nonZero) continue;
			nonZero = true;
			result += char;
		}

		return result;
	}

	

	//------------------------------------------------------------------------------------------------
	static int MaxInt(int a, int b)
	{
		if (a > b) return a;
		return b;
	}

	//------------------------------------------------------------------------------------------------
	static int MinInt(int a, int b)
	{
		if (a < b) return a;
		return b;
	}

	//------------------------------------------------------------------------------------------------
	//! Converts unixtime to formatted string
	//! \return UTC datetime in format "yyyy-mm-dd hh:ii:ss"
	static string GetTimeFormatted(int unixTime)
	{
		int remainingSeconds = unixTime;

		int year = remainingSeconds / 31556926;
		remainingSeconds -= year * 31556926;

		int month = remainingSeconds / 2629743;
		remainingSeconds -= month * 2629743;

		int day = remainingSeconds / 86400;
		remainingSeconds -= day * 86400;

		int hour = remainingSeconds / 3600;
		remainingSeconds -= hour * 3600;

		int minute = remainingSeconds / 60;
		remainingSeconds -= minute * 60;

		int second = remainingSeconds;

		return string.Format("%1-%2-%3 %4:%5:%6", 1970 + year, month.ToString(2), day.ToString(2), hour.ToString(2), minute.ToString(2), second.ToString(2));
	}
}

class EL_RefArrayCaster<Class TSourceType, Class TResultType>
{
	//------------------------------------------------------------------------------------------------
	//! Cast an array by converting all individual items. Allocates a new array of the input size.
	//! \param sourceArray Input array
	//! \return casted result array
	static array<ref TResultType> Convert(array<ref TSourceType> sourceArray)
	{
		if (!sourceArray) return null;

		array<ref TResultType> castedResult();
		castedResult.Reserve(sourceArray.Count());

		foreach (TSourceType element : sourceArray)
		{
			TResultType castedElement = TResultType.Cast(element);

			if (castedElement) castedResult.Insert(castedElement);
		}

		return castedResult;
	}
};
