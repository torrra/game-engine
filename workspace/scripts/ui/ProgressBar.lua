ProgressBar = {progressBarRef = nil}

function ProgressBar:_new(newProgressBarRef)
    local newProgressBarObj = {progressBarRef = newProgressBarRef}
    self.__index = self
    setmetatable(newProgressBarObj, self)
    return newProgressBarObj
end

function ProgressBar:SetValue(value)
    ProgressBarRef.SetValue(self.progressBarRef, value)
end