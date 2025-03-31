TestOpenFile = ScriptObject:_new()

-- Define member variables here

-- Is executed once when the object becomes active
function TestOpenFile:Start()

end


-- Is executed every tick
function TestOpenFile:Update(deltaTime)

end


-- Engine definitions
ScriptObjectTypes.testopenfile = TestOpenFile
return TestOpenFile