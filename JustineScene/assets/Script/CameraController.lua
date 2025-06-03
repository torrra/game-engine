CameraController = ScriptObject:_new()

-- Define member variables here

-- Is executed once when the object becomes active
function CameraController:Start()

    self.camera = GetCameraComponent(self.entity.handle)
end


-- Is executed every tick
function CameraController:Update(deltaTime)

    local x, y = GetCursorDeltaPos()
    self.camera:Rotate(y, x, 0, 0.2)   
end


-- Engine definitions
ScriptObjectTypes.CameraController = CameraController
return CameraController