PlayerController = ScriptObject:_new()

-- Is executed once when the object becomes active
function PlayerController:Start()
    self.transform = GetTransformComponent(self.entity.handle)
    self.camera = GetCameraComponent(self.entity.handle)
    self.rigidbody = GetRigidBodyDynamicComponent(self.entity.handle)
    self.raycast = Raycast.new()

    local downDir = Vector3.new(0, -1.0, 0)
    self.raycast:SetDirection(downDir)
    self.raycast:SetDistance(0.5)
end


-- Is executed every tick
function PlayerController:Update(deltaTime)
    -- Camera rotation
    local hSpeed = 0.05 -- horizontal speed
    local vSpeed = 0.02 -- vertical speed

    -- Get cursor delta position for camera rotation
    local xCursor, yCursor = GetCursorDeltaPos()
    self.transform:AddEulerRotation(0, -xCursor * hSpeed, 0)
    
    local _, rotationY, _ = self.transform:GetRotation()
    self.camera:SetRotation(0, rotationY, 0)

    self.camera:Rotate(yCursor * vSpeed, 0, 0)
    
    local forwardVec = Vector3.new(0, 0, -1):Rotate(0, rotationY, 0)
    local rightVec = Vector3.new(1, 0, 0):Rotate(0, rotationY, 0)

    local moveDir = Vector3.new(0, 0, 0)

    local forwardMovementSpeed = 5
    local backMovementSpeed = forwardMovementSpeed * 0.75
    local sideMovementSpeed = forwardMovementSpeed * 0.85
    
    -- Movement (forward & back)
    if IsInputDown(InputCode.KEY_W) then
        moveDir = moveDir + Vector3.new(forwardVec.x * forwardMovementSpeed * deltaTime, 
                                        forwardVec.y * forwardMovementSpeed * deltaTime, 
                                        forwardVec.z * forwardMovementSpeed * deltaTime)
    elseif IsInputDown(InputCode.KEY_S) then
        moveDir = moveDir - Vector3.new(forwardVec.x * backMovementSpeed * deltaTime, 
                                        forwardVec.y * backMovementSpeed * deltaTime, 
                                        forwardVec.z * backMovementSpeed * deltaTime)
        
    end 
    -- Movement (left & right)
    if IsInputDown(InputCode.KEY_A) then
        moveDir = moveDir - Vector3.new(rightVec.x * sideMovementSpeed * deltaTime, 
                                        rightVec.y * sideMovementSpeed * deltaTime, 
                                        rightVec.z * sideMovementSpeed * deltaTime)
        
    elseif IsInputDown(InputCode.KEY_D) then
        moveDir = moveDir + Vector3.new(rightVec.x * sideMovementSpeed * deltaTime, 
                                        rightVec.y * sideMovementSpeed * deltaTime, 
                                        rightVec.z * sideMovementSpeed * deltaTime)

    end

    self.transform:AddTranslation(moveDir.x, 0.0, moveDir.z)

    local playerPosition = Vector3.new(self.transform:GetPosition())
    local positionVec3 = Vector3.new(playerPosition.x, playerPosition.y - 0.5, playerPosition.z)
    self.raycast:SetOrigin(positionVec3)    

    -- Jump
    if IsInputPressed(InputCode.KEY_SPACE) then
        local hitInfo = self.raycast:HasHit()

        if hitInfo.hitEntity.handle ~= -1 then
            self.rigidbody:AddForce(0, 2.0, 0, ForceMode.IMPULSE, false)
        end
    end


end


-- Engine definitions
ScriptObjectTypes.PlayerController = PlayerController
return PlayerController