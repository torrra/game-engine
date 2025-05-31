Text = {textRef = nil}

function Text:_new(newTextRef)
    local newTextObj = {textRef = newTextRef}
    self.__index = self
    setmetatable(newTextObj, self)
    return newTextObj
end

function Text:GetRef()
    return self.textRef
end

function Text:SetTextColor(red, green, blue, alpha)
    TextRef.SetTextColor(self.textRef, red, green, blue, alpha)
end