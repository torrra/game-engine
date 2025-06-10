EnemyAnimation = ScriptObject:_new()

-- Is executed once when the object becomes active
function EnemyAnimation:Start()
    self.renderer = GetRendererComponent(self.entity.handle)
    self.transform = GetTransformComponent(self.entity.handle)
    self.parentTransform = GetTransformComponent(self.entity:GetParent().handle)
    
    self.prevPosition = Vector3.new(self.parentTransform:GetPosition())
    LoadResource(5, "Animation/walking.fbx")
end


-- Is executed every tick
function EnemyAnimation:Update(deltaTime)
    -- Movement animation
    local currentPosition = Vector3.new(self.parentTransform:GetPosition())
    
    if (currentPosition.x ~= self.prevPosition.x or currentPosition.x ~= self.prevPosition.y or currentPosition.x ~= self.prevPosition.z) then
        -- Play movement animation
            self.renderer:SetAnimation("Animation/walking.fbx")
            self.renderer:PlayAnimation()
            self.transform:SetPosition(0.0, 0.0, 0.0)

            -- Reset local position for animation
            self.prevPosition = currentPosition
    end
end


-- Engine definitions
ScriptObjectTypes.EnemyAnimation = EnemyAnimation
return EnemyAnimation