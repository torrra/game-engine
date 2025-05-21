NavigationPointScript = ScriptObject:_new()

-- Is executed once when the object becomes active
function NavigationPointScript:Start()

    self.nav = GetNavigationPointComponent(self.entity.handle)

    if not self.nav then
        print("NavigationPoint not ready in Start. Will retry in Update.")
        self.waitingForNav = true
    else
        print("NavigationPoint ready at:", self.nav:GetPosition())
    end
end

-- Is executed every tick
function NavigationPointScript:Update(deltaTime)

    if self.waitingForNav then
        self.nav = GetNavigationPointComponent(self.entity.handle)
        if self.nav then
            self.waitingForNav = false
            print("NavigationPoint now ready at:", self.nav:GetPosition())
        else
            return -- Wait
        end
    end
    
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

    self.nav:SetPosition(x, y, z)
    print("New navigation point position:", self.nav:GetPosition())
end

-- Engine definitions
ScriptObjectTypes.NavigationPointScript = NavigationPointScript
return NavigationPointScript
