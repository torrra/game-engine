NavPointTeleportScript = ScriptObject:_new()

NavPointTeleportScript.hasToTeleport = false
NavPointTeleportScript.teleportCooldown = 0
NavPointTeleportScript.cooldownDuration = 1.0 -- par exemple 1 sec

-- Is executed once when the object becomes active
function NavPointTeleportScript:Start()
    self.playerEntity = GetEntity("Player").handle
end

function NavPointTeleportScript:Update(deltaTime)

    -- Mise à jour du cooldown
    if self.teleportCooldown > 0 then
        self.teleportCooldown = self.teleportCooldown - deltaTime
        if self.teleportCooldown <= 0 then
            self.hasToTeleport = false
            self.teleportCooldown = 0
        end
    end

end

function NavPointTeleportScript:OnTriggerEnter(otherEntity)

    if otherEntity == self.playerEntity and self.teleportCooldown <= 0 then
        print("Player trigger")
        self.hasToTeleport = true
    end

end

function NavPointTeleportScript:OnTriggerExit(otherEntity)

    if otherEntity == self.playerEntity then
    end

end

-- Engine definitions
ScriptObjectTypes.NavPointTeleportScript = NavPointTeleportScript
return NavPointTeleportScript
