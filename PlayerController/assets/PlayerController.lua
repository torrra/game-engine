PlayerController = ScriptObject:_new()

-- Is executed once when the object becomes active
function PlayerController:Start()
    self.transform = GetTransformComponent(self.entity.handle)
    self.camera = GetCameraComponent(self.entity.handle)
    self.rigidbody = GetRigidBodyDynamicComponent(self.entity.handle)
    self.raycast = Raycast.new()

    local downDir = Vector3.new(0, -1, 0)
    self.raycast:SetDirection(downDir)
    self.raycast:SetDistance(0.05)
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

    local rotationX, rotationY, rotationZ = self.camera:GetRotation()

    -- Movement (forward & back)
    local movementSpeed = 5
    if IsInputDown(InputCode.KEY_W) then
        local translation = Vector3.new(0, 0, -1 * movementSpeed * deltaTime)
        local localTranslation = translation:Rotate(rotationX, rotationY, rotationZ)
        self.transform:AddTranslation(localTranslation.x, 0.0, localTranslation.z) 
    elseif IsInputDown(InputCode.KEY_S) then
        local translation = Vector3.new(0, 0, 1 * movementSpeed * 0.75 * deltaTime)
        local localTranslation = translation:Rotate(rotationX, rotationY, rotationZ)
        self.transform:AddTranslation(localTranslation.x, 0.0, localTranslation.z) 
    end 
    -- Movement (left & right)
    if IsInputDown(InputCode.KEY_A) then
        local translation = Vector3.new(-1 * movementSpeed * 0.85 * deltaTime, 0, 0)
        local localTranslation = translation:Rotate(rotationX, rotationY, rotationZ)
        self.transform:AddTranslation(localTranslation.x, 0.0, localTranslation.z) 
    elseif IsInputDown(InputCode.KEY_D) then
        local translation = Vector3.new(1 * movementSpeed * 0.85 * deltaTime, 0, 0)
        local localTranslation = translation:Rotate(rotationX, rotationY, rotationZ)
        self.transform:AddTranslation(localTranslation.x, 0.0, localTranslation.z) 
    end

    local playerPosition = Vector3.new(self.transform:GetPosition())
    local positionVec3 = Vector3.new(playerPosition.x, playerPosition.y - 1.5, playerPosition.z)
    self.raycast:SetOrigin(positionVec3)    

    -- Jump
    if IsInputPressed(InputCode.KEY_SPACE) then
        local hitInfo = self.raycast:HasHit()

        if hitInfo.hitEntity.handle ~= -1 then
            self.rigidbody:AddForce(0, 7.5, 0, ForceMode.IMPULSE, false)
        end
    end

    self.transform:SetRotation(0, rotationY, 0)

end


-- Engine definitions
ScriptObjectTypes.PlayerController = PlayerController
return PlayerController