BackgroundMusicScript = ScriptObject:_new()

-- Is executed once when the object becomes active
function BackgroundMusicScript:Start()

    self.backgroundMusic = GetAudioPlayerComponent(self.entity.handle)
    self.backgroundMusic:RefreshRef()
    self.backgroundMusic:PlaySound()
end


-- Is executed every tick
function BackgroundMusicScript:Update(deltaTime)

end


-- Engine definitions
ScriptObjectTypes.BackgroundMusicScript = BackgroundMusicScript
return BackgroundMusicScript