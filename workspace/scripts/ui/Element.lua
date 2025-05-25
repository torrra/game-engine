Element = {ref = nil}

function Element:GetPosition()
    return elementRef.GetPosition(self.ref)
end

function Element:GetScale()
    return elementRef.GetScale(self.ref)
end

function Element:SetPosition(xPos, yPos)
    elementRef.SetPosition(self.ref, xPos, yPos)
end

function Element:SetScale(xScale, ySCale)
    elementRef.SetScale(self.ref, xScale, ySCale)
end

function Element:SetAutoScale(shouldAutoScale)
    if type(shouldAutoScale) ~= "bool" then
        error("Expected boolean as argument")
    else
        elementRef.SetAutoScale(self.ref, shouldAutoScale)
    end

end

function Element:_new(object)
    object = object or {}
	self.__index = self
	setmetatable(object, self)
	return object
end