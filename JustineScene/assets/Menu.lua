Menu = ScriptObject:_new()

Menu.isOpen = false
Menu.isVictory = false
Menu.title = "Default text"

function Menu:OpenMenu(title)

    self.title = title
    self.isOpen = true

    -- Canvas
    self.Menu = Canvas:CreateCanvas("Menu", 0, 0)
    self.Menu:SetCanvasColor(0.4, 0.4, 0.4, 0.1)
    self.screenWidth, self.screenHeight = self.Menu:GetScreenSize()
    
    local xPos = (self.screenWidth * 0.5) - 200

    -- Title text
    self.titleText = self.Menu:AddText(self.title, (self.screenWidth * 0.5) - 125, 150)
    self.titleText:SetTextColor(1.0, 1.0, 1.0, 1.0)

    -- Button continue
    self.buttonContinue = self.Menu:AddButton("Restart", xPos, 300)
    self.buttonContinue:SetButtonSize(400, 150)


    -- Button quit
    self.buttonQuit = self.Menu:AddButton("Quit", xPos, 550)
    self.buttonQuit:SetButtonSize(400, 150)
end

function Menu:UpdateMenu()
    local newScreenWidth, newScreenHeight = self.Menu:GetScreenSize()

    if (newScreenWidth ~= self.screenWidth or newScreenHeight ~= self.screenHeight) then
        self.screenWidth = newScreenWidth
        self.screenHeight = newScreenHeight
        
        self.Menu:RemoveElement(self.titleText)
        self.Menu:RemoveElement(self.buttonContinue)
        self.Menu:RemoveElement(self.buttonQuit)
        
        local xPos = (self.screenWidth * 0.5) - 200

        -- Title text
        self.titleText = self.Menu:AddText(self.title, (self.screenWidth * 0.5) - 125, 150)
        self.titleText:SetTextColor(1.0, 1.0, 1.0, 1.0)
    
        -- Button continue
        self.buttonContinue = self.Menu:AddButton("Restart", xPos, 300)
        self.buttonContinue:SetButtonSize(400, 150)
    
    
        -- Button quit
        self.buttonQuit = self.Menu:AddButton("Quit", xPos, 550)
        self.buttonQuit:SetButtonSize(400, 150)
    end
end

function Menu:ButtonStatus()
    if (self.buttonContinue:IsButtonPressed()) then
        print("Continue pressed")
        self.isOpen = false
        self.Menu:DestroyCanvas()
    end
    
    if (self.buttonQuit:IsButtonPressed()) then 
        print("Quit pressed")
        self.isOpen = false
        self.Menu:DestroyCanvas()
    end
end

-- Is executed every tick
function Menu:Update(deltaTime)

    if self.isOpen ~= true then
        return
    end

    self:UpdateMenu()
    self:ButtonStatus()
end


-- Engine definitions
ScriptObjectTypes.Menu = Menu
return Menu