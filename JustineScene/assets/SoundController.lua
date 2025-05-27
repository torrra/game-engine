SoundController = ScriptObject:_new()

function SoundController:Start()
    self.audioPlayer = GetAudioPlayerComponent(self.entity.handle)
    self.transform = GetTransformComponent(self.entity.handle)
    self.audioPlayer:RefreshRef()

    self.isPlayed = false

    self.volume = 1.0
    self.audioPlayer:SetSoundVolume(self.volume)
end

function SoundController:Update(deltaTime)
    -- local speed = 10.0

    -- local dx, dy, dz = 0, 0, 0
    -- if IsInputDown(InputCode.KEY_W) then dz = dz + 1 end
    -- if IsInputDown(InputCode.KEY_S) then dz = dz - 1 end
    -- if IsInputDown(InputCode.KEY_D) then dx = dx - 1 end
    -- if IsInputDown(InputCode.KEY_A) then dx = dx + 1 end

    -- if dx ~= 0 or dz ~= 0 then
    --     self.transform:AddTranslation(dx * speed * deltaTime, 0, dz * speed * deltaTime)

    --     local pos = self.transform:GetPosition()
    --     self.audioPlayer:SetSoundPosition(pos.x, pos.y, pos.z)

    --     local vel = Vector3.new(dx * speed, 0, dz * speed)
    --     self.audioPlayer:SetSoundVelocity(vel.x, vel.y, vel.z)
    -- end
    -- Contrôle du volume avec W/S
    if IsInputDown(InputCode.KEY_W) then
        self.volume = math.min(1.0, self.volume + 0.5 * deltaTime)
        self.audioPlayer:SetSoundVolume(self.volume)
    elseif IsInputDown(InputCode.KEY_S) then
        self.volume = math.max(0.0, self.volume - 0.5 * deltaTime)
        self.audioPlayer:SetSoundVolume(self.volume)
        self.ID = self.audioPlayer:GetSound()
        print("Sound ID : ", self.ID)
    end
    if IsInputDown(InputCode.KEY_D) and not self.isPlayed  then
        self.audioPlayer:PlaySound(true)
        self.isPlayed = true
    elseif IsInputDown(InputCode.KEY_A) and self.isPlayed then
        self.audioPlayer:StopSound()
        self.isPlayed = false
    end
end

ScriptObjectTypes.SoundController = SoundController
return SoundController