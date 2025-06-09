DoorTrigger = ScriptObject:_new()

DoorTrigger.isDoorOpened = false

-- Is executed once when the object becomes active
function DoorTrigger:Start()

end


-- Is executed every tick
function DoorTrigger:Update(deltaTime)

end

function DoorTrigger:OnTriggerEnter(otherEntity)

    self.isDoorOpened = true
    print("Door opened")
end

function DoorTrigger:OnTriggerExit(otherEntity)

    self.isDoorOpened = false
    print("Door closed")
end

-- Engine definitions
ScriptObjectTypes.DoorTrigger = DoorTrigger
return DoorTrigger