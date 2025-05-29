PlayerController = ScriptObject:_new()

-- Is executed once when the object becomes active
function PlayerController:Start()
    self.transform = GetTransformComponent(self.entity.handle)
    self.camera = GetCameraComponent(self.entity.handle)
end


-- Is executed every tick
function PlayerController:Update(deltaTime)
    -- Camera rotation
    local xCursor, yCursor = GetCursorDeltaPos()
    self.camera:Rotate(yCursor, xCursor, 0, 0.15)

    -- Movement
    local xRot, yRot, zRot = 0
    local movementSpeed = 5
    if IsInputDown(InputCode.KEY_W) then
        local translation = Vector3.new(0, 0, -1 * movementSpeed * deltaTime)
        local localTranslation = translation:Rotate(self.camera:GetRotation())
        self.transform:AddTranslation(localTranslation.x, 0.0, localTranslation.z) 
    elseif IsInputDown(InputCode.KEY_S) then
        local translation = Vector3.new(0, 0, 1 * movementSpeed * deltaTime)
        local localTranslation = translation:Rotate(self.camera:GetRotation())
        self.transform:AddTranslation(localTranslation.x, 0.0, localTranslation.z) 
    end

    if IsInputDown(InputCode.KEY_A) then
        local translation = Vector3.new(-1 * movementSpeed * deltaTime, 0, 0)
        local localTranslation = translation:Rotate(self.camera:GetRotation())
        self.transform:AddTranslation(localTranslation.x, 0.0, localTranslation.z) 
    elseif IsInputDown(InputCode.KEY_D) then
        local translation = Vector3.new(1 * movementSpeed * deltaTime, 0, 0)
        local localTranslation = translation:Rotate(self.camera:GetRotation())
        self.transform:AddTranslation(localTranslation.x, 0.0, localTranslation.z) 
    end
end


-- Engine definitions
ScriptObjectTypes.PlayerController = PlayerController
return PlayerController