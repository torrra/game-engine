TestUserScript = ScriptObject:_new()

-- Define member variables here

-- Is executed once when the object becomes active
function TestUserScript:Start()

end


-- Is executed every tick
function TestUserScript:Update(deltaTime)

end

print("HI TESTUSERSCRIPT EXISTS")

-- Engine definitions
ScriptObjectTypes.testuserscript = TestUserScript
return TestUserScript