require("BaseGun")

Shotgun = BaseGun:new()

Shotgun.ammoType = AmmoTypes.SHELL
Shotgun.magazineSize = 10
Shotgun.ammoCount = Shotgun.magazineSize
Shotgun.backupAmmoCount = Shotgun.magazineSize
Shotgun.fireRate = 0.7
Shotgun.lastFireTime = 0.0
Shotgun.canFire = true
Shotgun.range = 5.0
Shotgun.name = "Shotgun"

-- Is executed once when the object becomes active
function Shotgun:Start()

end


-- Is executed every tick
function Shotgun:Update(deltaTime)

end


-- Engine definitions
ScriptObjectTypes.Shotgun = Shotgun
return Shotgun