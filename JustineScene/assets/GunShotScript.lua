GunShotScript = ScriptObject:_new()

GunShotScript.shotSound = nil

-- Is executed once when the object becomes active
function GunShotScript:Start()
    self.shotSound = GetAudioPlayerComponent(self.entity.handle)
    self.shotSound:RefreshRef()
end


-- Is executed every tick
function GunShotScript:Update(deltaTime)

end


-- Engine definitions
ScriptObjectTypes.GunShotScript = GunShotScript
return GunShotScript