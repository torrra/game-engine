Button = {buttonRef = nil}

function Button:_new(newButtonRef)
    local newButtonObj = {buttonRef = newButtonRef}
    self.__index = self
    setmetatable(newButtonObj, self)
    return newButtonObj
end