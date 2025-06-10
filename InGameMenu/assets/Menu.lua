Menu = ScriptObject:_new()

function Menu:OpenFailureMenu()
    -- Canvas
    self.Menu = Canvas:CreateCanvas("FailureMenu", 0, 0)
    self.Menu:SetCanvasColor(0.4, 0.4, 0.4, 0.1)
    screenWidth, screenHeight = self.Menu:GetScreenSize()
    
    local xPos = (screenWidth * 0.5) - 200

    -- Title text
    titleText = self.Menu:AddText("Level Failed", (screenWidth * 0.5) - 125, 150)
    titleText:SetTextColor(1.0, 1.0, 1.0, 1.0)

    -- Button continue
    buttonContinue = self.Menu:AddButton("Restart", xPos, 300)
    buttonContinue:SetButtonSize(400, 150)


    -- Button quit
    buttonQuit = self.Menu:AddButton("Quit", xPos, 550)
    buttonQuit:SetButtonSize(400, 150)
end

function Menu:UpdateFailureMenu()
    local newScreenWidth, newScreenHeight = self.Menu:GetScreenSize()

    if (newScreenWidth ~= screenWidth or newScreenHeight ~= screenHeight) then
        screenWidth = newScreenWidth
        screenHeight = newScreenHeight
        
        self.Menu:RemoveElement(titleText)
        self.Menu:RemoveElement(buttonContinue)
        self.Menu:RemoveElement(buttonQuit)
        
        local xPos = (screenWidth * 0.5) - 200

        -- Title text
        titleText = self.Menu:AddText("Level Failed", (screenWidth * 0.5) - 125, 150)
        titleText:SetTextColor(1.0, 1.0, 1.0, 1.0)
    
        -- Button continue
        buttonContinue = self.Menu:AddButton("Restart", xPos, 300)
        buttonContinue:SetButtonSize(400, 150)
    
    
        -- Button quit
        buttonQuit = self.Menu:AddButton("Quit", xPos, 550)
        buttonQuit:SetButtonSize(400, 150)
    end
end

function Menu:ButtonStatus()
    if (buttonContinue:IsButtonPressed()) then
        print("Continue pressed")
    end
    
    if (buttonQuit:IsButtonPressed()) then 
        print("Quit pressed")
    end
end

-- Is executed once when the object becomes active
function Menu:Start()
    Menu:OpenFailureMenu("Level Failed")
end


-- Is executed every tick
function Menu:Update(deltaTime)
    Menu:UpdateFailureMenu()
    Menu:ButtonStatus()
end


-- Engine definitions
ScriptObjectTypes.Menu = Menu
return Menu