ProgressBar = {progressBarRef = nil}

function ProgressBar:_new(newProgressBarRef)
    local newProgressBarObj = {progressBarRef = newProgressBarRef}
    self.__index = self
    setmetatable(newProgressBarObj, self)
    return newProgressBarObj
end

function ProgressBar:GetRef()
    return self.progressBarRef
end

function ProgressBar:SetValue(value)
    ProgressBarRef.SetValue(self.progressBarRef, value)
end

function ProgressBar:SetBgColor(red, green, blue, alpha)
    ProgressBarRef.SetBgColor(self.progressBarRef, red, green, blue, alpha)
end

function ProgressBar:SetFillColor(red, green, blue, alpha)
    ProgressBarRef.SetFillColor(self.progressBarRef, red, green, blue, alpha)
end