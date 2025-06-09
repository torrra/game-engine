TriggerScript = ScriptObject:_new()

-- Is executed once when the object becomes active
function TriggerScript:Start()
  local parentEntity = self.entity:GetParent()
  local parentScript = GetScriptComponent(parentEntity.handle)

  if not parentScript then
    return
  end

  self.enemyScript = parentScript.EnemyScript
  
end

-- Is executed every tick
function TriggerScript:Update(deltaTime)

end

function TriggerScript:OnTriggerEnter(otherEntity)

    if self.enemyScript then
      self.enemyScript.hasDetected = true
      print("Trigger player")
    end

end

-- Engine definitions
ScriptObjectTypes.TriggerScript = TriggerScript
return TriggerScript