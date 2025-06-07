Image = {imgRef = nil}

function Image:_new(newImageRef)
    local newImageObj = {imgRef = newImageRef}
    self.__index = self
    setmetatable(newImageObj, self)
    return newImageObj
end

function Image:SetPosition(xPosition, yPosition)
    ImageRef.SetPosition(self.imgRef, xPosition, yPosition)
end

function Image:SetScale(xScale, yScale)
    ImageRef.SetScale(self.imgRef, xScale, yScale)
end

function Image:GetPosition()
    return ImageRef.GetPosition(self.imgRef)
end

function Image:GetScale()
    return ImageRef.GetScale(self.imgRef)
end

function Image:GetRef()
    return self.imgRef
end