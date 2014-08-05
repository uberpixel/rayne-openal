//
//  RALAudioWorld.cpp
//  rayne-openal
//
//  Copyright 2014 by Überpixel. All rights reserved.
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
//  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "RALAudioWorld.h"

namespace RN
{
	namespace openal
	{
		RNDefineMeta(AudioWorld, RN::WorldAttachment)
		RNDefineSingleton(AudioWorld)
		
		AudioWorld::AudioWorld() :
		_audioListener(nullptr)
		{
			MakeShared();
			
			_device = alcOpenDevice(NULL);
			if(!_device)
			{
				RNDebug("rayne-openal: Could not open audio device.");
				return;
			}
			
			_context = alcCreateContext(_device, NULL);
			alcMakeContextCurrent(_context);
			if(!_context)
			{
				RNDebug("rayne-openal: Could not create audio context.");
				return;
			}
		}
		
		AudioWorld::~AudioWorld()
		{
			alcMakeContextCurrent(NULL);
			alcDestroyContext(_context);
			alcCloseDevice(_device);
		}
		
		void AudioWorld::StepWorld(float delta)
		{
			
		}
		
		void AudioWorld::SetAudioListener(AudioListener *attachment)
		{
			_audioListener = attachment;
		}
		
		AudioSource *AudioWorld::PlaySound(AudioResource *resource)
		{
			if(_audioListener)
			{
				AudioSource *source = new AudioSource(resource);
				_audioListener->GetParent()->AddChild(source);
				source->SetSelfdestruct(true);
				source->Play();
				return source;
			}
			return nullptr;
		}
	}
}
