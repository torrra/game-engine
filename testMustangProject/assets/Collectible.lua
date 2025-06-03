Collectible = ScriptObject:_new()

CollectibleType = 
{
    LIFE = 1,
    LIGHT_AMMO = 2,
    SHELL_AMMO = 3,
    ROCKET_AMMO = 4
}

Collectible.type = CollectibleType.LIFE
Collectible.value = 1


-- Is executed once when the object becomes active
function Collectible:Start()

end


-- Is executed every tick
function Collectible:Update(deltaTime)

end

function Collectible:AddLife()

function Collectible:OnTriggerEnter(otherEntity)

    local script = GetScriptComponent(otherEntity.handle)

    if not script then
        return
    end

    

end


-- Engine definitions
ScriptObjectTypes.Collectible = Collectible
return Collectible