BaseGunEnemy1 = ScriptObject:_new()

AmmoTypes =
{
    LIGHT = {damage = 1},
    SHELL = {damage = 5},
    ROCKET = {damage = 10},
}

BaseGunEnemy1.ammoType = AmmoTypes.LIGHT
BaseGunEnemy1.magazineSize = 30
BaseGunEnemy1.ammoCount = BaseGunEnemy.magazineSize
BaseGunEnemy1.backupAmmoCount = BaseGunEnemy.magazineSize
BaseGunEnemy1.fireRate = 0.5
BaseGunEnemy1.lastFireTime = 0.0
BaseGunEnemy1.canFire = true
BaseGunEnemy1.range = 10.0
BaseGunEnemy1.name = "Gun"

function BaseGunEnemy1:new(obj)
    obj = obj or {}
    setmetatable(obj, self)
    self.__index = self
    return obj
end

function BaseGunEnemy1:GetHitEntityLife(hitEntity)

    local script = GetScriptComponent(hitEntity.handle)
    if script then
        local life = script["Life"]
        if life then
            print("Hit : ", hitEntity.handle)
            return life
        end
    end

    return nil
end

function BaseGunEnemy1:Fire()

    if not self.canFire or (os.clock() - self.lastFireTime < self.fireRate) then
        -- print("No can shoot")
        return
    end

    if self.ammoCount <= 0 then
        print("Out of ammo")
        return
    end

    self.lastFireTime = os.clock() 
    self.ammoCount = self.ammoCount - 1

    local direction = Vector3.new(1.0, 0.0, 0.0):Rotate(self.transform:GetRotation())

    local position = Vector3.new(self.transform:GetPosition())
    position = position + Vector3.new(direction.x, direction.y, direction.z)

    self.gunRay:SetOrigin(position)
    self.gunRay:SetDirection(direction)
    local result = self.gunRay:HasHit()
    print("pew pew")
    
    if not result.hasHit then
        print("no hit")
        return
    end
    
    if result.hitEntity == self.entity then
        print("Ignoring self-hit entity")
        return
    end

    local entityLife = self:GetHitEntityLife(result.hitEntity)

    if entityLife then
        entityLife:TakeDamage(self.ammoType.damage)
        self.gunShot.shotSound:PlaySound()

    end
end


function BaseGunEnemy1:Reload()

    if self.backupAmmoCount <= 0 then
        return
    end

    local toReload = self.magazineSize - self.ammoCount

    if toReload > self.backupAmmoCount then
        toReload = self.backupAmmoCount
    end

    self.ammoCount = self.ammoCount + toReload
    self.backupAmmoCount = self.backupAmmoCount - toReload

end

-- Is executed once when the object becomes active
function BaseGunEnemy1:Start()

    self.transform = GetTransformComponent(self.entity.handle)
    self.gunRay = Raycast.new()
    self.gunRay:SetDistance(self.range)

    local gunScript = GetScriptComponent(self.entity.handle)
    
    self.gunShot = gunScript.GunShotScript
end


-- Is executed every tick
function BaseGunEnemy1:Update(deltaTime)

    self.transform = GetTransformComponent(self.entity.handle)

end


-- Engine definitions
ScriptObjectTypes.BaseGunEnemy1 = BaseGunEnemy1
return BaseGunEnemy1