BaseGun = ScriptObject:_new()

AmmoTypes =
{
    LIGHT = {damage = 1},
    SHELL = {damage = 5},
    ROCKET = {damage = 10},
}

BaseGun.ammoType = AmmoTypes.LIGHT
BaseGun.magazineSize = 15
BaseGun.ammoCount = BaseGun.magazineSize
BaseGun.backupAmmoCount = BaseGun.magazineSize
BaseGun.fireRate = 0.3
BaseGun.lastFireTime = 0.0
BaseGun.canFire = true
BaseGun.range = 10.0

function BaseGun:new(obj)
    obj = obj or {}
    setmetatable(obj, self)
    self.__index = self
    return obj
end

function BaseGun:GetHitEntityLife(hitEntity)

    local script = GetScriptComponent(hitEntity.handle)

    if script then
        local life = script["Life"]

        if life then
            return life
        end
    end

    return nil
end

function BaseGun:Fire()

    if not self.canFire or (os.clock() - self.lastFireTime < self.fireRate) then
        print("No can shoot")
        return
    end

    if self.ammoCount <= 0 then
        print("Out of ammo")
        return
    end

    self.lastFireTime = os.clock()
    self.ammoCount = self.ammoCount - 1

    local direction = Vector3.new(0.0, 0.0, 1.0):Rotate(self.transform:GetRotation())

    local position = Vector3.new(self.transform:GetPosition())

    position = position + Vector3.new(direction.x, direction.y, direction.z)

    self.gunRay:SetOrigin(position)
    self.gunRay:SetDirection(direction)

    local result = self.gunRay:HasHit()
    print("pew pew")

    if not result.hasHit then
        return
    end

    local entityLife = self:GetHitEntityLife(result.hitEntity)

    if entityLife then
        entityLife:TakeDamage(self.ammoType.damage)
    end
end


function BaseGun:Reload()

    if self.backupAmmoCount <= 0 then
        return
    end

    local toReload = self.magazineSize - self.ammoCount

    if toReload > self.backupAmmoCount then
        toReload = self.backupAmmoCount
    end

    self.ammoCount = self.ammoCount + toReload
    self.backupAmmoCount = self.backupAmmoCount - toReload

    print("Reloaded: " .. toReload .. " bullets. Current ammo: "
         .. self.ammoCount .. ", Backup ammo: " .. self.backupAmmoCount)
end

-- Is executed once when the object becomes active
function BaseGun:Start()

    self.transform = GetTransformComponent(self.entity.handle)
    self.gunRay = Raycast.new()
    self.gunRay:SetDistance(self.range)
    self.camera = GetCameraComponent(GetEntity("Camera").handle)

end


-- Is executed every tick
function BaseGun:Update(deltaTime)

end


-- Engine definitions
ScriptObjectTypes.BaseGun = BaseGun
return BaseGun