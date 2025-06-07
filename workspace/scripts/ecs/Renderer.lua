Renderer = Component:_new()

local ExistingRendererComponents = {}

function Renderer:_RegisterRendererComponent(handle)

	local newComponent = {entity = handle}
	self.__index = self
	setmetatable(newComponent, self)
	ExistingRendererComponents[handle] = newComponent
end


function _NewRendererComponent(handle)

	Renderer:_RegisterRendererComponent(handle)
end

function _RemoveRendererComponent(handle)

	ExistingRendererComponents[handle] = nil
end

function GetRendererComponent(handle)

	return ExistingRendererComponents[handle]
end

function Renderer:RefreshRef()

	self.ref = RendererRef.GetRendererRef(self.entity)
end


function Renderer:SetMaterial(index, name)

	RendererRef.SetMaterial(self.ref, index, name)
end

function Renderer:SetModel(name)

	RendererRef.SetModel(self.ref, name)
end

function Renderer:SetShader(name)

	RendererRef.SetShader(name)
end

function Renderer:GetMaterial(index)

	return RendererRef.GetMaterial(self.ref, index)
end

function Renderer:GetModel()

	return RendererRef.GetModel(self.ref)
end

function Renderer:GetShader()

	return RendererRef.GetShader(self.ref)
end

function Renderer:SetAnimation(name)

	return RendererRef.SetAnimation(self.ref, name)
end

function Renderer:PauseAnimation()

	RendererRef.PauseAnimation(self.ref)
end

function Renderer:StopAnimation()

	RendererRef.StopAnimation(self.ref)
end

function Renderer:PlayAnimation()

	RendererRef.PlayAnimation(self.ref)
end

function Renderer:GetAnimation()

	return RefreshRef.GetAnimation(self.ref)
end

function Renderer:SetAnimationKeyframe(keyFrame)
	
	RefreshRef.SetAnimationKeyframe(self.ref, keyFrame)
end

function Renderer:SetAnimationSpeed(speed)

	RefreshRef.SetAnimationSpeed(self.ref, speed)
end

function Renderer:GetAnimationKeyframe()

	return RendererRef.GetAnimationKeyframe(self.ref)
end

function Renderer:GetAnimationSpeed()

	return RendererRef.GetAnimationSpeed(self.ref)
end

function Renderer:IsAnimationLooped()

	return RendererRef.IsAnimationLooped(self.ref)
end

function Renderer:SetAnimationLooped(looped)

	RendererRef.SetAnimationLooped(self.ref, looped)
end

function Renderer:IsAnimationPlaying()

	return RendererRef.IsAnimationPlaying(self.ref)
end


function Renderer:IsAnimationPaused()

	return RendererRef.IsAnimationPaused(self.ref)
end