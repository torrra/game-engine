Script = {entity = nil}

local ExistingScriptComponents = {}

function Script:_RegisterScriptComponent(handle)

	local newComponent = {entity = handle}
	self.__index = self
	setmetatable(newComponent, self)
	ExistingScriptComponents[handle] = newComponent	
	print("\n[Script component] successfully registered component owned by "..handle.."\n")
end

function Script:_RegisterScriptObject(type)
	ScriptObject._Register(type, self.entity, self)
end


function _NewScriptComponent(handle)
	Script:_RegisterScriptComponent(handle)
end

function _NewScriptObject(type, handle)
	local ownerComponent = ExistingScriptComponents[handle]
	ScriptObject._Register(type, handle, ownerComponent)	
end


function _UpdateScript(handle, deltaTime)

	print("\n[Script component] updating script component owned by "..handle.."\n")
	local component = ExistingScriptComponents[handle]
	component:_UpdateComponent(deltaTime)
end

function _StartScript(handle)

	print("\n[Script component] starting script component owned by "..handle.."\n")
	local component = ExistingScriptComponents[handle]
	component:_StartComponent()
end


function Script:_UpdateComponent(deltaTime)

	for _, object in pairs(self) do

		if type(object) == "table" and 
			object._ExecuteUpdate ~= nil then

			object:_ExecuteUpdate(deltaTime)
		end
	end
end

function Script:_StartComponent()

	print("[Script component]: component starting...")

	for _, object in pairs(self) do

		if type(object) == "table" and 
			object._ExecuteStart ~= nil then

			object:_ExecuteStart()
		end
	end
end

return Script