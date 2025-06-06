Collectible = ScriptObject:_new()

function Collectible:new(obj)

    local object = obj or {}
    self.__index = self
    setmetatable(object, self)
    return object
end


function Collectible:RegenHealth(script)

    local lifeComponent = script["Life"]

    if lifeComponent then
        lifeComponent.health = self.value + lifeComponent.health
        print("adding health")
    end
end

function Collectible:AddLightAmmo(script)

    local gun = script["BaseGun"]

    if gun then
        
        if gun.backupAmmoCount + self.value > gun.magazineSize then
            gun.backupAmmoCount = gun.magazineSize
        else
            gun.backupAmmoCount = gun.backupAmmoCount + self.value
        end

        print("adding light ammo")
    end

end

function Collectible:AddShellAmmo(script)

    local gun = script.Shotgun

    if gun then
        
        if gun.backupAmmoCount + self.value > gun.magazineSize then
            gun.backupAmmoCount = gun.magazineSize
        else
            gun.backupAmmoCount = gun.backupAmmoCount + self.value
        end

        print("adding shell ammo")
    end

end

-- Is executed once when the object becomes active
function Collectible:Start()

end


-- Is executed every tick
function Collectible:Update(deltaTime)

end

function Collectible:OnTriggerEnter(otherEntity)

    local script = GetScriptComponent(otherEntity)

    if not script then
        return
    end

    self:type(script)
    DestroyEntity(self.entity)
end

CollectibleType = 
{
    LIFE = Collectible.RegenHealth,
    LIGHT_AMMO = Collectible.AddLightAmmo,
    SHELL_AMMO = Collectible.AddShellAmmo,
}

Collectible.type = function(_) print("collectible action") end
Collectible.value = 0

-- Engine definitions
ScriptObjectTypes.Collectible = Collectible
return Collectible