EnemyScript = ScriptObject:_new()

-- Is executed once when the object becomes active
function EnemyScript:Start()

    self.transform = GetTransformComponent(self.entity.handle)
    self.navPoint1 = GetNavigationPointComponent(GetEntity("NaviPoint.NavPoint1").handle)
    self.rigidbody = GetRigidBodyDynamicComponent(self.entity.handle)

end


-- Is executed every tick
function EnemyScript:Update(deltaTime)

    local _, height = self.rigidbody:GetCapsuleFormat()

    local x, y, z = self.navPoint1:GetPosition()
    local position = Vector3.new(x, y + height + 0.3, z)
    self.transform:SetPosition(position.x, position.y, position.z)

    self.rigidbody:SetAngularVelocity(0.0, 1.0, 0.0)

end


-- Engine definitions
ScriptObjectTypes.EnemyScript = EnemyScript
return EnemyScript