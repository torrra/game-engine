Text = {ref = nil}

function Text:_new(newTextRef)
    local newTextObj = {ref = newTextRef}
    self.__index = self
    setmetatable(newTextObj, self)
    return newTextObj
end

function Text:SetTextColor(red, green, blue, alpha)
    TextRef.SetTextColor(self.ref, red, green, blue, alpha)
end