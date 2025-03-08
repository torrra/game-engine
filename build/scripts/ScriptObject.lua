ScriptObject = {entity = nil}
ScriptObjectTypes = {scriptobject = ScriptObject}

-- type: script object derived type
-- handle: handle of the entity that possesses the new object
-- owner: owning Script component 
function ScriptObject._Register(type, handle, owner)

	local typeTable = ScriptObjectTypes[type]
	local newObj = {entity = Entity:_NewNativeEntity(handle)}	

	typeTable.__index = typeTable
	setmetatable(newObj, typeTable)
	owner[type] = newObj
	print("\n[ScriptObject script] successfully registered script object owned by "..handle.."\n")

end

function ScriptObject.typename()
	return "ScriptObject"
end


function ScriptObject:_PreExecute()

	self.entity:RefreshRef()
end

function ScriptObject:Start()
	print("\n[Script object]: START CALLED\n")
end

function ScriptObject:Update(deltaTime)
	print("\n[Script object]: UPDATE CALLED\n")
end


local function _PreExecuteScriptObject(handle)
	local object = ExistingScriptObjects[handle]
	object:_PreExecute()
	return object
end

function ScriptObject:_ExecuteStart()

	self:_PreExecute()
	self:Start()
end

function ScriptObject:_ExecuteUpdate(deltaTime)
	self:_PreExecute()
	self:Update()
end	

function _StartScriptObject(handle)

	local object = _PreExecuteScriptObject(handle)
	object:Start()
end

function _UpdateScriptObject(handle)

	local object = _PreExecuteScriptObject(handle)
	object:Update()
end

return ScriptObject