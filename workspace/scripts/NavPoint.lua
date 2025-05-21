NavigationPoint = Component:_new()

local ExistingNavigationPointComponents = {}

function NavigationPoint:_RegisterNavigationPointComponent(handle)

	local newComponent = {entity = handle}
	self.__index = self
	setmetatable(newComponent, self)
	ExistingNavigationPointComponents[handle] = newComponent
end

function _NewNavigationPointComponent(handle)
	
	print("Registering NavigationPoint with handle", handle)
	NavigationPoint:_RegisterNavigationPointComponent(handle)
end

function _RemoveNavigationPointComponent(handle)

	ExistingNavigationPointComponents[handle] = nil
end

function GetNavigationPointComponent(handle)

	return ExistingNavigationPointComponents[handle]
end

function NavigationPoint:RefreshRef()
	
	self.ref = NavigationPointRef.GetNavigationPointRef(self.entity)
end

function NavigationPoint:GetPosition()
	
	return NavigationPointRef.GetPosition(self.ref)
end

function NavigationPoint:SetPosition(x, y, z)

	NavigationPointRef.SetPosition(self.ref, x, y, z)
end