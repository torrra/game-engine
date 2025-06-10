WinZone = ScriptObject:_new()

-- Is executed once when the object becomes active
function WinZone:Start()

end


-- Is executed every tick
function WinZone:Update(deltaTime)

end


-- Engine definitions
ScriptObjectTypes.WinZone = WinZone
return WinZone