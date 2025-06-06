require("Shotgun")

WeaponController = ScriptObject:_new()

WeaponController.baseGunKey = InputCode.KEY_1
WeaponController.shotgunKey = InputCode.KEY_2
WeaponController.swapGunKey = InputCode.KEY_E
WeaponController.fireKey = InputCode.MOUSE_BUTTON_LEFT
WeaponController.reloadKey = InputCode.KEY_R

WeaponController.currentGun = nil

WeaponController.ammoTextPosition = Vector2.new(400, 300)
WeaponController.lifeBarPosition = Vector2.new(100, 100)

function WeaponController:UpdateAmmoText()

    local canvasString = string.format(
       "%s   |   Ammo: %d / %d   |   %d",
        self.currentGun.name, self.currentGun.ammoCount, self.currentGun.backupAmmoCount,
        self.currentGun.magazineSize
    )

    if self.ammoText then
        self.uiCanvas:RemoveElement(self.ammoText)
        self.ammoText = nil
    end

    self.ammoText = self.uiCanvas:AddText(canvasString, self.ammoTextPosition.x,
                                          self.ammoTextPosition.y)

    self.ammoText:SetTextColor(1.0, 1.0, 1.0, 1.0)
end

function WeaponController:OnLifeUpdated(delta)
    if not self.lifeBar then
        return
    end

    self.lifeValue = self.life.health
    self.lifeBar:SetValue(self.lifeValue)
end

-- Is executed once when the object becomes active
function WeaponController:Start()

    local script = GetScriptComponent(self.entity.handle)

    self.baseGun = script.BaseGun
    self.shotgun = script.Shotgun
    self.currentGun = self.baseGun

    self.life = script.Life
    self.lifeValue = self.life.health

    self.life.CustomDamageFunction = function(damage)
        self:OnLifeUpdated(damage)
    end

    self.uiCanvas = Canvas:CreateCanvas("WeaponControllerUI", 100, 100)

    self:UpdateAmmoText()
    self.lifeBar = self.uiCanvas:AddProgressBar(self.lifeBarPosition.x,
                                                self.lifeBarPosition.y, 200, 20, 0, self.lifeValue)

    self.lifeBar:SetFillColor(0.0, 1.0, 0.0, 1.0)

end


-- Is executed every tick
function WeaponController:Update(deltaTime)

    if IsInputPressed(self.baseGunKey) then
        self.currentGun = self.baseGun
        self:UpdateAmmoText()
    elseif IsInputPressed(self.shotgunKey) then
        self.currentGun = self.shotgun
        self:UpdateAmmoText()
    end

    if IsInputPressed(self.swapGunKey) then
        if self.currentGun == self.baseGun then
            self.currentGun = self.shotgun
        else
            self.currentGun = self.baseGun
        end
        self:UpdateAmmoText()
    end

    if IsInputPressed(self.fireKey) and self.currentGun then
        self.currentGun:Fire()
        self:UpdateAmmoText()
    end

    if IsInputPressed(self.reloadKey) and self.currentGun then
        self.currentGun:Reload()
        self:UpdateAmmoText()
    end

end


-- Engine definitions
ScriptObjectTypes.WeaponController = WeaponController
return WeaponController