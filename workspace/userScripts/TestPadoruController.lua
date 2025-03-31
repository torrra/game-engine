TestPadoruController = ScriptObject:_new()

-- Define member variables here

-- Is executed once when the object becomes active
function TestPadoruController:Start()
    
    self.transform = GetTransformComponent(self.entity.handle)
end


-- Is executed every tick
function TestPadoruController:Update(deltaTime)

    if IsInputDown(InputCode.W) then
        self.transform:AddTranslation(0, 0, 1 * deltaTime) 

    elseif IsInputDown(InputCode.S) then
        self.transform:AddTranslation(0, 0, -1 * deltaTime)

    end

    if IsInputDown(InputCode.A) then
        self.transform:AddTranslation(-1 * deltaTime, 0, 0)
    
    elseif IsInputDown(InputCode.D) then
        self.transform:AddTranslation(1 * deltaTime, 0, 0)

    end

end


-- Engine definitions
ScriptObjectTypes.testpadorucontroller = TestPadoruController
return TestPadoruController