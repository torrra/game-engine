TeleportScript = ScriptObject:_new()

function TeleportScript:Start()

    self.transform = GetTransformComponent(self.entity.handle)
    self.teleportTransform = GetTransformComponent(GetEntity("Teleport.Teleport2").handle)

    self.teleportEntity = GetEntity("Teleport.Teleport1")
    local teleportScriptComponent = GetScriptComponent(self.teleportEntity.handle)
    if not teleportScriptComponent then
        return
    end

    self.teleportScript = teleportScriptComponent.NavPointTeleportScript
end

function TeleportScript:Update(deltaTime)

    if self.teleportScript.hasToTeleport then

        local x, y, z = self.teleportTransform:GetPosition()
        self.transform:SetPosition(x, y + 0.5, z)

        -- On déclenche le cooldown de l’autre trigger pour éviter boucle infinie
        self.teleportScript.hasToTeleport = false
        self.teleportScript.teleportCooldown = self.teleportScript.cooldownDuration

        print("Teleport done !")
    end

end

-- Engine definitions
ScriptObjectTypes.TeleportScript = TeleportScript
return TeleportScript
