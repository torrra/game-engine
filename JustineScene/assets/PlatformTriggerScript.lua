PlatformTriggerScript = ScriptObject:_new()

-- Is executed once when the object becomes active
function PlatformTriggerScript:Start()

    local parentEntity = self.entity:GetParent()
    local parentScript = GetScriptComponent(parentEntity.handle)

    if not parentScript then
        return
    end

    self.platformScript = parentScript.PlatformScript

end


-- Is executed every tick
function PlatformTriggerScript:Update(deltaTime)

end

function PlatformTriggerScript:OnTriggerEnter(otherEntity)

    if self.platformScript then
      self.platformScript.isTriggered = true
      print("Trigger")
    end

end

function PlatformTriggerScript:OnTriggerExit(otherEntity)

    if self.platformScript then
        self.platformScript.isTriggered = false
    end

end
-- Engine definitions
ScriptObjectTypes.PlatformTriggerScript = PlatformTriggerScript
return PlatformTriggerScript