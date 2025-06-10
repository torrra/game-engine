require ("Script.Collectible")

LifeCollectible = Collectible:new()

LifeCollectible.type = CollectibleType.LIFE
LifeCollectible.value = 1

-- Is executed once when the object becomes active
function LifeCollectible:Start()

end


-- Is executed every tick
function LifeCollectible:Update(deltaTime)

end


-- Engine definitions
ScriptObjectTypes.LifeCollectible = LifeCollectible
return LifeCollectible