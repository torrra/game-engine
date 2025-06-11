playerGun = ScriptObject:_new()

-- Is executed once when the object becomes active
function playerGun:Start()
    self.transform = GetTransformComponent(self.entity.handle)
    self.parentCamera = GetCameraComponent(self.entity:GetParent().handle)
end


-- Is executed every tick
function playerGun:Update(deltaTime)

        local rotationX, rotationY, rotationZ = self.parentCamera:GetRotation()
        local translation = Vector3.new(0.4, -0.95, -0.4)
        local localTranslation = translation:Rotate(rotationX, rotationY, 0.0)

    local rotation = Vector3.new(self.parentCamera:GetRotation())
    self.transform:SetRotation(rotationZ, rotation.y +  - (3.14 * 0.5), 0.0) 
    self.transform:SetPosition(localTranslation.x, -0.95, localTranslation.z)
end


-- Engine definitions
ScriptObjectTypes.playerGun = playerGun
return playerGun