Button = {buttonRef = nil}

function Button:_new(newButtonRef)
    local newButtonObj = {buttonRef = newButtonRef}
    self.__index = self
    setmetatable(newButtonObj, self)
    return newButtonObj
end

function Button:GetRef()
    return self.buttonRef
end

function Button:IsButtonPressed()
    return ButtonRef.IsButtonPressed(self.buttonRef)
end

function Button:SetColor(red, green, blue, alpha)
    return ButtonRef.SetColor(self.buttonRef, red, green, blue, alpha)
end

function Button:SetHoveredColor(red, green, blue, alpha)
    return ButtonRef.SetHoveredColor(self.buttonRef, red, green, blue, alpha)
end

function Button:SetPressedColor(red, green, blue, alpha)
    return ButtonRef.SetPressedColor(self.buttonRef, red, green, blue, alpha)
end