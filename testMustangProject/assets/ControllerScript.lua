ControllerScript = ScriptObject:_new()

-- Define member variables here
local distance = 10.0

-- Is executed once when the object becomes active
function ControllerScript:Start()
    self.transform = GetTransformComponent(self.entity.handle)
    self.gunRay = Raycast.new()
    self.gunRay:SetDistance(distance)

    print(self.gunRay:GetDistance())

    local cameraEntity = GetEntity("Camera")

    self.camera = GetCameraComponent(cameraEntity.handle)

end


-- Is executed every tick
function ControllerScript:Update(deltaTime)
     if IsInputDown(InputCode.KEY_W) then
        self.transform:AddTranslation(0, 0, 1 * deltaTime) 
    
    elseif IsInputDown(InputCode.KEY_S) then
        self.transform:AddTranslation(0, 0, -1 * deltaTime)
    end

    if IsInputDown(InputCode.KEY_A) then
        self.transform:AddTranslation(1 * deltaTime, 0, 0)
    
    elseif IsInputDown(InputCode.KEY_D) then
        self.transform:AddTranslation(-1 * deltaTime, 0, 0)
    end
    

    --[[ if (IsInputDown(InputCode.MOUSE_BUTTON_LEFT)) then
        
        local direction = Vector3.new(0.0, 0.0, -1):Rotate(self.camera:GetRotation())
        local position = Vector3.new(self.camera:GetPosition())
        
        self.gunRay:SetOrigin(position)
        self.gunRay:SetDirection(direction)

        local result = self.gunRay:HasHit()
    end
    --]]
end

function ControllerScript:OnCollisionEnter(otherEntity)

    print("override | otherEntity: "..otherEntity)

end




-- Engine definitions
ScriptObjectTypes.ControllerScript = ControllerScript
return ControllerScript