require ("Script.Collectible")

ShellAmmoCollectible = ScriptObject:_new()

-- Is executed once when the object becomes active
function ShellAmmoCollectible:Start()

end


-- Is executed every tick
function ShellAmmoCollectible:Update(deltaTime)

end


-- Engine definitions
ScriptObjectTypes.ShellAmmoCollectible = ShellAmmoCollectible
return ShellAmmoCollectible