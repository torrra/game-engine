ControllerScript = ScriptObject:_new()

-- Define member variables here

-- Is executed once when the object becomes active
function ControllerScript:Start()
    self.transform = GetTransformComponent(self.entity.handle)
end


-- Is executed every tick
function ControllerScript:Update(deltaTime)
    if IsInputDown(InputCode.KEY_W) then
        self.transform:AddTranslation(0, 0, -1 * deltaTime) 
    
    elseif IsInputDown(InputCode.KEY_S) then
        self.transform:AddTranslation(0, 0,  1 * deltaTime)
    end

    if IsInputDown(InputCode.KEY_A) then
        self.transform:AddTranslation(-1 * deltaTime, 0, 0)
    
    elseif IsInputDown(InputCode.KEY_D) then
        self.transform:AddTranslation( 1 * deltaTime, 0, 0)
    end

    if IsInputDown(InputCode.KEY_Q) then
        self.transform:AddEulerRotation(0, -50 * deltaTime, 0)
    elseif IsInputDown(InputCode.KEY_E) then
        self.transform:AddEulerRotation(0, 50 * deltaTime, 0)
    end
end


-- Engine definitions
ScriptObjectTypes.controllerscript = ControllerScript
return ControllerScript