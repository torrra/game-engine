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

function Canvas:RemoveElement(element)
	CanvasRef.RemoveElement(self._name, element:GetRef())
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

function Canvas:AddButton(text, xPos, yPos)
	newButtonRef = CanvasRef.AddButton(self._name, text, xPos, yPos)
	return Button:_new(newButtonRef)
end

function Canvas:AddProgressBar(xPos, yPos, width, height, minRange, maxRange)
	newProgressBarRef = CanvasRef.AddProgressBar(self._name, xPos, yPos, width, height, minRange, maxRange)
	return ProgressBar:_new(newProgressBarRef)
end