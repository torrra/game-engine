Image = {imgRef = nil}

function Image:_new(newImageRef)
    local newImageObj = {imgRef = newImageRef}
    self.__index = self
    setmetatable(newImageObj, self)
    return newImageObj
end

function Image:SetImageSize(xSize, ySize)
    print(xSize, ySize)
end