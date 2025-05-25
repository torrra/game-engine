Canvas = {}
Canvas.__index = Canvas

local ExistingCanvases = {}

function Canvas:CreateCanvas(canvasName, canvasWidth, canvasHeight)
	local canvas = setmetatable({}, self)
	
	canvas._name = canvasName
	canvas._width = canvasWidth
	canvas._height = canvasHeight

	ExistingCanvases[canvasName] = canvas
	CanvasRef.CreateCanvas(canvasName, canvasWidth, canvasHeight)

	return canvas
end

function Canvas:SetCanvasColor(red, green, blue, alpha)
	CanvasRef.SetCanvasColor(self._name, red, green, blue, alpha)
end

function Canvas:AddText(text, xPos, yPos)
	newTextRef = CanvasRef.AddText(self._name, text, xPos, yPos)
	return Text:_new(newTextRef)
end

function Canvas:AddImage(path, xPos, yPos)
	newImageRef = CanvasRef.AddImage(self._name, path, xPos, yPos)
	return Image:_new(newImageRef)
end











--Canvas = Canvas:_new()
--Canvas = {name = "", width = nil, height = nil}
--
--function Canvas:RefreshRef(canvasName)
--	self.ref = CanvasRef.GetCanvasRef(canvasName)
--end
--
--function Canvas:CreateCanvas(canvasName, _width, _height)
--	local newCanvas = {name = canvasName, width = _width, height = _height}
--	
--
--
--	CanvasRef.CreateCanvas(canvasName, _width, _height)
--
--	return self
--end
--
--function Canvas:SetCanvasColor(red, green, blue, alpha)
--	return CanvasRef.SetCanvasColor(self.name, red, green, blue, alpha)
--end


