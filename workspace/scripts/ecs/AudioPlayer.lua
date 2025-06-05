AudioPlayer = Component:_new()

local ExistingAudioPlayerComponents = {}

function AudioPlayer:_RegisterAudioPlayerComponent(handle)

	local newComponent = {entity = handle}
	self.__index = self
	setmetatable(newComponent, self)
	ExistingAudioPlayerComponents[handle] = newComponent
end

function _NewAudioPlayerComponent(handle)

	AudioPlayer:_RegisterAudioPlayerComponent(handle)
end

function _RemoveAudioPlayerComponent(handle)

	ExistingAudioPlayerComponents[handle] = nil
end

function GetAudioPlayerComponent(handle)

	return ExistingAudioPlayerComponents[handle]
end

function AudioPlayer:RefreshRef()
	
	self.ref = AudioPlayerRef.GetAudioPlayerRef(self.entity)
end

function AudioPlayer:GetSound()

	return AudioPlayerRef.GetSound(self.ref)
end

function AudioPlayer:GetSoundPosition()

	return AudioPlayerRef.GetSoundPosition(self.ref)
end

function AudioPlayer:GetSoundVelocity()

	return AudioPlayerRef.GetSoundVelocity(self.ref)
end

function AudioPlayer:GetSoundVolume()

	return AudioPlayerRef.GetSoundVolume(self.ref)
end

function AudioPlayer:GetIs3DSound()

	return AudioPlayerRef.GetIs3DSound(self.ref)
end

function AudioPlayer:GetListener()

	return AudioPlayerRef.GetListener(self.ref)
end

function AudioPlayer:GetListenerPosition()

	return AudioPlayerRef.GetListenerPosition(self.ref)
end

function AudioPlayer:GetListenerForward()

	return AudioPlayerRef.GetListenerForward(self.ref)
end

function AudioPlayer:GetListenerUp()

	return AudioPlayerRef.GetListenerUp(self.ref)
end

function AudioPlayer:GetListenerVelocity()

	return AudioPlayerRef.GetListenerVelocity(self.ref)
end

function AudioPlayer:SetSound(soundName)

	AudioPlayerRef.SetSound(self.ref, soundName)
end

function AudioPlayer:SetSoundPosition(x, y, z)

	AudioPlayerRef.SetSoundPosition(self.ref, x, y, z)
end

function AudioPlayer:SetSoundVelocity(x, y, z)

	AudioPlayerRef.SetSoundVelocity(self.ref, x, y, z)
end

function AudioPlayer:SetSoundVolume(volume)

	AudioPlayerRef.SetSoundVolume(self.ref, volume)
end

function AudioPlayer:SetIs3DSound(is3D)

	AudioPlayerRef.SetIs3DSound(self.ref, is3D)
end

function AudioPlayer:SetListener(listenerData)

	AudioPlayerRef.SetListener(self.ref, listenerData)
end

function AudioPlayer:SetListenerPosition(x, y, z)

	AudioPlayerRef.SetListenerPosition(self.ref, x, y, z)
end

function AudioPlayer:SetListenerForward(x, y, z)

	AudioPlayerRef.SetListenerForward(self.ref, x, y, z)
end

function AudioPlayer:SetListenerUp(x, y, z)

	AudioPlayerRef.SetListenerUp(self.ref, x, y, z)
end

function AudioPlayer:SetListenerVelocity(x, y, z)

	AudioPlayerRef.SetListenerVelocity(self.ref, x, y, z)
end

function AudioPlayer:PlaySound(is3DSound)
	
	is3DSound = is3DSound == nil and false or is3DSound
	AudioPlayerRef.PlaySound(self.ref, is3DSound)
end

function AudioPlayer:StopSound()

	AudioPlayerRef.StopSound(self.ref)
end

function AudioPlayer:PauseSound(isPaused)

	AudioPlayerRef.PauseSound(self.ref, isPaused)
end