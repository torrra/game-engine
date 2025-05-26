NavigationPointScript = ScriptObject:_new()

-- Is executed once when the object becomes active
function NavigationPointScript:Start()

    self.nav = GetNavigationPointComponent(self.entity.handle)

    self.nas:RefreshRef()
end

-- Is executed every tick
function NavigationPointScript:Update(deltaTime)
    
    local moveSpeed = 1.0 * deltaTime
    local x, y, z = self.nav:GetPosition()

    if IsInputDown(InputCode.KEY_UP) then
        z = z - moveSpeed
    end
    if IsInputDown(InputCode.KEY_DOWN) then
        z = z + moveSpeed
    end
    if IsInputDown(InputCode.KEY_LEFT) then
        x = x - moveSpeed
    end
    if IsInputDown(InputCode.KEY_RIGHT) then
        x = x + moveSpeed
    end
end

-- Engine definitions
ScriptObjectTypes.NavigationPointScript = NavigationPointScript
return NavigationPointScript
