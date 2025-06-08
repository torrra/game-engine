Menu = ScriptObject:_new()

function Menu:OpenMenu(text)
    -- Canvas
    self.Menu = Canvas:CreateCanvas("Menu", 0, 0)
    self.Menu:SetCanvasColor(0.0, 0.0, 0.0, 0.1)

    -- Title text
    local titleText = self.Menu:AddText(text, 200, 200)
    titleText:SetTextColor(1.0, 1.0, 1.0, 1.0)

    -- Button continue
    self.Menu:AddButton("Continue", 200, 300)

    -- Button quit
    self.Menu:AddButton("Quit", 200, 400)
    
end

-- Is executed once when the object becomes active
function Menu:Start()
    Menu:OpenMenu("Level Failed")
end


-- Is executed every tick
function Menu:Update(deltaTime)
    
end


-- Engine definitions
ScriptObjectTypes.Menu = Menu
return Menu