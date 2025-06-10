require("Script.BaseGun")

Shotgun = BaseGun:new()

Shotgun.ammoType = AmmoTypes.SHELL
Shotgun.magazineSize = 10
Shotgun.ammoCount = Shotgun.magazineSize
Shotgun.backupAmmoCount = Shotgun.magazineSize
Shotgun.fireRate = 0.5
Shotgun.lastFireTime = 0.0
Shotgun.canFire = true
Shotgun.range = 5.0
Shotgun.name = "Shotgun"


-- Engine definitions
ScriptObjectTypes.Shotgun = Shotgun
return Shotgun