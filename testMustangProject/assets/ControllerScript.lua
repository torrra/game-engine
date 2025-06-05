ControllerScript = ScriptObject:_new()

-- Define member variables here
local distance = 10.0

-- Is executed once when the object becomes active
function ControllerScript:Start()
    self.transform = GetTransformComponent(self.entity.handle)

    local cameraEntity = GetEntity("Camera")

    self.camera = GetCameraComponent(cameraEntity.handle)

    local script = GetScriptComponent(self.entity.handle)
    self.gun = script.BaseGun

end


-- Is executed every tick
function ControllerScript:Update(deltaTime)
     --[[if IsInputDown(InputCode.KEY_W) then
        self.transform:AddTranslation(0, 0, 1 * deltaTime) 
    
    elseif IsInputDown(InputCode.KEY_S) then
        self.transform:AddTranslation(0, 0, -1 * deltaTime)
    end

    if IsInputDown(InputCode.KEY_A) then
        self.transform:AddTranslation(1 * deltaTime, 0, 0)
    
    elseif IsInputDown(InputCode.KEY_D) then
        self.transform:AddTranslation(-1 * deltaTime, 0, 0)
    end
    ]]--

    if IsInputDown(InputCode.KEY_Q) then
        self.transform:AddEulerRotation(0, 90 * deltaTime, 0)
    elseif IsInputDown(InputCode.KEY_E) then
        self.transform:AddEulerRotation(0, -90 * deltaTime, 0)
    end
    
    --self.transform:SetRotation(self.camera:GetRotation())

    if (IsInputPressed(InputCode.MOUSE_BUTTON_LEFT)) then
        
        self.gun:Fire()
    end
    
    if IsInputPressed(InputCode.KEY_R) then
        self.gun:Reload()
    end

end

function ControllerScript:OnCollisionEnter(otherEntity)

    print("override | otherEntity: "..otherEntity)

end




-- Engine definitions
ScriptObjectTypes.ControllerScript = ControllerScript
return ControllerScript