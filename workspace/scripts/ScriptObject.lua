ScriptObject = {entity = nil}
ScriptObjectTypes = {scriptobject = ScriptObject}

-- Create a new instance of a ScriptObject child typ
-- type: script object derived type
-- handle: handle of the entity that possesses the new object
-- owner: owning Script component 
function ScriptObject._Register(typename, handle, owner)

	-- Get the script object metatable
	local typeTable = ScriptObjectTypes[typename]
	local newObj = {entity = Entity:_NewNativeEntity(handle)}	

	typeTable.__index = typeTable
	setmetatable(newObj, typeTable)
	owner[typename] = newObj
end

-- Create a new instance of ScriptObject prototype table
-- Only used to declare table protptypes inheriting from ScriptObject
function ScriptObject:_new(object)
	object = object or {}
	self.__index = self
	setmetatable(object, self)
	return object
end

-- Execute necessary tasks (refresh references) before running
-- Start() or Update()
function ScriptObject:_PreExecute()

	for _, object in pairs(self) do
		if type(object) == "table" and
		object.RefreshRef ~= nil then
			-- print("[Script object]: refreshing ref "..tostring(object))
			object:RefreshRef()
		end
	end

end


function ScriptObject:Start()

end

function ScriptObject:Update(deltaTime)

end

function ScriptObject:OnCollisionEnter(otherEntity)

	print("LUA [collision] enter")
end

function ScriptObject:OnCollisionExit(otherEntity)

	print("LUA [collision] exit")
end

function ScriptObject:OnTriggerEnter(otherEntity)

	print("LUA [trigger] enter")
end

-- Internal Start() caller
function ScriptObject:_ExecuteStart()

	self:_PreExecute()
	self:Start()
end

-- Internal Update() caller
function ScriptObject:_ExecuteUpdate(deltaTime)
	self:_PreExecute()
	self:Update(deltaTime)
end	

return ScriptObject