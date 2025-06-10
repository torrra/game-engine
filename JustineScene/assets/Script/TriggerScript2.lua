TriggerScript2 = ScriptObject:_new()

-- Is executed once when the object becomes active
function TriggerScript2:Start()
  local parentEntity = self.entity:GetParent()
  local parentScript = GetScriptComponent(parentEntity.handle)

  if not parentScript then
    return
  end

  self.enemyScript = parentScript.EnemyScript2
  
end

-- Is executed every tick
function TriggerScript2:Update(deltaTime)

end

function TriggerScript2:OnTriggerEnter(otherEntity)

    if self.enemyScript then
      self.enemyScript.hasDetected = true
      print("Trigger player")
    end

end

-- Engine definitions
ScriptObjectTypes.TriggerScript2 = TriggerScript2
return TriggerScript2