ControllerScript = ScriptObject:_new()

-- Define member variables here

-- Is executed once when the object becomes active
function ControllerScript:Start()
    self.transform = GetTransformComponent(self.entity.handle)
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

    print(self.transform:GetPosition())
end


-- Engine definitions
ScriptObjectTypes.ControllerScript = ControllerScript
return ControllerScript