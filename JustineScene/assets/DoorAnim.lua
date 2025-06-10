DoorAnim = ScriptObject:_new()

-- Is executed once when the object becomes active
function DoorAnim:Start()

    local parent = self.entity:GetParent()
    local doorScript = GetScriptComponent(parent.handle)
    if doorScript then
        self.doorAnim = doorScript["DoorTrigger"]
    end
    self.isDoorOpened = false
    
    self.renderer = GetRendererComponent(self.entity.handle)
    self.renderer:RefreshRef()
    self.isPlaying = false

    self.renderer:SetAnimation("Armature|ArmatureAction")

    self.renderer:PlayAnimation()
end


-- Is executed every tick
function DoorAnim:Update(deltaTime)
    
    self.isDoorOpened = self.doorAnim.isDoorOpened
    self.isPlaying = self.renderer:IsAnimationPlaying()
    
    if self.isDoorOpened == true then
        -- print("play")
    elseif self.isPlaying == true then
        -- print("stop")
        -- self.renderer:StopAnimation()
    end

end


-- Engine definitions
ScriptObjectTypes.DoorAnim = DoorAnim
return DoorAnim