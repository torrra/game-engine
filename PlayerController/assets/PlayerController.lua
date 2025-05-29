PlayerController = ScriptObject:_new()

-- Is executed once when the object becomes active
function PlayerController:Start()
    self.transform = GetTransformComponent(self.entity.handle)
    self.camera = GetCameraComponent(self.entity.handle)
    self.rigidbody = Get
end


-- Is executed every tick
function PlayerController:Update(deltaTime)
    -- Camera rotation
    local hSpeed = 0.05 -- horizontal speed
    local vSpeed = 0.02 -- vertical speed

    -- Get cursor delta position for camera rotation
    local xCursor, yCursor = GetCursorDeltaPos()

    -- Set camera rotation
    self.camera:Rotate(yCursor * vSpeed, xCursor * hSpeed, 0, deltaTime)

    -- Movement (forward & back)
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

    -- Movement (left & right)
    if IsInputDown(InputCode.KEY_A) then
        local translation = Vector3.new(-1 * movementSpeed * deltaTime, 0, 0)
        local localTranslation = translation:Rotate(self.camera:GetRotation())
        self.transform:AddTranslation(localTranslation.x, 0.0, localTranslation.z) 
    elseif IsInputDown(InputCode.KEY_D) then
        local translation = Vector3.new(1 * movementSpeed * deltaTime, 0, 0)
        local localTranslation = translation:Rotate(self.camera:GetRotation())
        self.transform:AddTranslation(localTranslation.x, 0.0, localTranslation.z) 
    end

    -- Jump
    if IsInputPressed(KEY_SPACE) then
        print("Jump...")
        -- TODO: Need to merge rigidbody to add jump logic

    end

end


-- Engine definitions
ScriptObjectTypes.PlayerController = PlayerController
return PlayerController