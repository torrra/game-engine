ComponentType = {
	TRANSFORM = 1,
	SCRIPT = 2,
	RENDERER = 4,
	RIGIDBODY_DYNAMIC = 8,
	RIGIDBODY_STATIC = 16,
	CAMERA = 32,
	AUDIO_PLAYER = 64,
	TRIANGLE_MESH = 128,
	NAVIGATION_POINT = 256,
	LIGHT_SOURCE = 512
}

function CreateEntity(name, parent)

	if type(name) ~= "string" then
		error("Invalid name")
	end

	if not parent then
		parent = -1

	else
		parent = parent.handle
	end

	local entityHandle = Scene.CreateEntity(name, parent)
	local newEntity = Entity:_NewNativeEntity(entityHandle)

	newEntity:RefreshRef()

	return newEntity;
	
end

function DestroyEntity(entity)

	Scene.DestroyEntity(entity.handle)
end

function CreateTransformComponent(owner)

	Scene.CreateCompomemt(owner.handle, ComponentType.TRANSFORM)
	local newComponent = GetTransformComponent(owner.handle)

	return newComponent
end	

function CreateScriptComponent(owner)
	
	Scene.CreateCompomemt(owner.handle, ComponentType.SCRIPT)
	local newComponent = GetScriptComponent(owner.handle)

	return newComponent
end

function CreateRendererComponent(owner)

	Scene.CreateCompomemt(owner.handle, ComponentType.RENDERER)
end

function CreateRigidBodyStaticComponent(owner)

	Scene.CreateCompomemt(owner.handle, ComponentType.RIGIDBODY_STATIC)
	local newComponent = GetRigidBodyStaticComponent(owner.handle)

	return newComponent
end

function CreateRigidBodyDynamicComponent(owner)

	Scene.CreateCompomemt(owner.handle, ComponentType.RIGIDBODY_DYNAMIC)
	local newComponent = GetRigidBodyDynamicComponent(owner.handle)

	return newComponent
end

function CreateCameraComponent(owner)

	Scene.CreateCompomemt(owner.handle, ComponentType.CAMERA)
	local newComponent = GetCameraComponent(owner.handle)

	return newComponent
end

function CreateTriangleMeshComponent(owner)

	Scene.CreateCompomemt(owner.handle, ComponentType.TRIANGLE_MESH)
end

function CreateAudioPlayerComponent(owner)

	Scene.CreateCompomemt(owner.handle, ComponentType.AUDIO_PLAYER)
	local newComponent = GetAudioPlayerComponent(owner.handle)

	return newComponent
end

function CreateNavigationPointComponent(owner)

	Scene.CreateCompomemt(owner.handle, ComponentType.NAVIGATION_POINT)
	local newComponent = GetNavigationPointComponent(owner.handle)

return newComponent
end

function CreateLightSourceComponent(owner)

	Scene.CreateCompomemt(owner.handle, ComponentType.LIGHT_SOURCE)
	
end