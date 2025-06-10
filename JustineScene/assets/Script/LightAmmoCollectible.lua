require ("Script.Collectible")

LightAmmoCollectible = Collectible:new()

LightAmmoCollectible.type = CollectibleType.LIGHT_AMMO
LightAmmoCollectible.value = 5

-- Is executed once when the object becomes active
function LightAmmoCollectible:Start()

end


-- Is executed every tick
function LightAmmoCollectible:Update(deltaTime)

end


-- Engine definitions
ScriptObjectTypes.LightAmmoCollectible = LightAmmoCollectible
return LightAmmoCollectible