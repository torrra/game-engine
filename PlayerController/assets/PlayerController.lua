PlayerController = ScriptObject:_new()

-- Is executed once when the object becomes active
function PlayerController:Start()
    self.transform = GetTransformComponent(self.entity.handle)
    self.camera = GetCameraComponent(self.entity.handle)
end


-- Is executed every tick
function PlayerController:Update(deltaTime)
    
    -- Camera rotation
    local x, y = GetCursorDeltaPos()
    self.camera:Rotate(y, x, 0, 0.15)
    
    -- Movement
    local movementSpeed = 5
    if IsInputDown(InputCode.KEY_W) then
        self.camera:Move(0, 0, -1, movementSpeed, deltaTime) 
    elseif IsInputDown(InputCode.KEY_S) then
        self.camera:Move(0, 0, 1, movementSpeed, deltaTime)
    end

    if IsInputDown(InputCode.KEY_A) then
        self.camera:Move(-1, 0, 0, movementSpeed, deltaTime)
    elseif IsInputDown(InputCode.KEY_D) then
        self.camera:Move(1, 0, 0, movementSpeed, deltaTime)
    end
end


-- Engine definitions
ScriptObjectTypes.PlayerController = PlayerController
return PlayerController