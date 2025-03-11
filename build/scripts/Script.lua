Script = Component:_new()

local ExistingScriptComponents = {}

-- Register new instance of ScriptComponent
function Script:_RegisterScriptComponent(handle)

	local newComponent = {entity = handle}
	self.__index = self
	setmetatable(newComponent, self)
	ExistingScriptComponents[handle] = newComponent	
	print("\n[Script component] successfully registered component owned by "..handle.."\n")
end

-- Add a new script object instance to this comopnent
function Script:_RegisterScriptObject(type)
	ScriptObject._Register(type, self.entity, self)
end


function Script:RefreshRef()
	
	self.ref = ScriptRef.GetScriptRef(self.entity)
end

-- Run updates on all applicable members
function Script:_UpdateComponent(deltaTime)

	for _, object in pairs(self) do

		if type(object) == "table" and 
			object._ExecuteUpdate ~= nil then

			object:_ExecuteUpdate(deltaTime)
		end
	end
end

-- Start all script objects in this component
function Script:_StartComponent()

	print("[Script component]: component starting...")

	for _, object in pairs(self) do

		if type(object) == "table" and 
			object._ExecuteStart ~= nil then

			object:_ExecuteStart()
		end
	end
end


-- Get an existing script component
function GetScriptComponent(ownerHandle)

	return ExistingScriptComponents[ownerHandle]
end

-- Create a new script component instance from C++
function _NewScriptComponent(handle)
	Script:_RegisterScriptComponent(handle)
end

-- Add a new script o
function _NewScriptObject(typename, handle)
	local ownerComponent = ExistingScriptComponents[handle]
	ScriptObject._Register(typename, handle, ownerComponent)	
end

-- Update script objects inside a script
function _UpdateScript(handle, deltaTime)

	print("\n[Script component] updating script component owned by "..handle.."\n")
	local component = ExistingScriptComponents[handle]
	component:_UpdateComponent(deltaTime)
end

-- Call Start() on all script objects
function _StartScript(handle)

	print("\n[Script component] starting script component owned by "..handle.."\n")
	local component = ExistingScriptComponents[handle]
	component:_StartComponent()
end



return Script