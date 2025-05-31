CoolUIScript = ScriptObject:_new()

-- Is executed once when the object becomes active
function CoolUIScript:Start()
    local myCanvas = Canvas:CreateCanvas("ThisWillWork... (In Theory)", 100, 100)
    myCanvas:SetCanvasColor(0.0, 1.0, 0.0, 0.4)
    
    local text = myCanvas:AddText("This is a test", 400, 300)
    text:SetTextColor(1.0, 0.0, 0.0, 1.0)
end


-- Is executed every tick
function CoolUIScript:Update(deltaTime)
    
end


-- Engine definitions
ScriptObjectTypes.CoolUIScript = CoolUIScript
return CoolUIScript