//
//  RALAudioSource.cpp
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

#include "RALAudioSource.h"
#include "RALAudioWorld.h"
#include "RALAudioResourceAttachment.h"

namespace RN
{
	namespace openal
	{
		RNDefineMeta(AudioSource, SceneNode)
		
		AudioSource::AudioSource(AudioResource *asset) :
			_asset(asset),
			_isPlaying(false),
			_isRepeating(false),
			_isSelfdestructing(false)
		{
			_asset->Retain();
			_oldPosition = GetWorldPosition();
			
			alGenSources(1, &_source);
			alSourcef(_source, AL_PITCH, 1);
			alSourcef(_source, AL_GAIN, 1);
			alSourcei(_source, AL_LOOPING, AL_FALSE);
			
			AudioResourceAttachment *attachment = AudioResourceAttachment::GetAttachmentForResource(asset);
			alSourcei(_source, AL_BUFFER, attachment->GetBufferID());
		}
		
		AudioSource::~AudioSource()
		{
			alDeleteSources(1, &_source);
			_asset->Release();
		}
		
		void AudioSource::SetRepeat(bool repeat)
		{
			_isRepeating = repeat;
			if(repeat)
			{
				alSourcei(_source, AL_LOOPING, AL_TRUE);
			}
			else
			{
				alSourcei(_source, AL_LOOPING, AL_FALSE);
			}
		}
		
		void AudioSource::SetPitch(float pitch)
		{
			alSourcef(_source, AL_PITCH, pitch);
		}
		
		void AudioSource::SetGain(float gain)
		{
			alSourcef(_source, AL_GAIN, gain);
		}
		
		void AudioSource::SetRange(float min, float max)
		{
			alSourcei(_source, AL_REFERENCE_DISTANCE, min);
			alSourcei(_source, AL_MAX_DISTANCE, max);
		}
		
		void AudioSource::SetSelfdestruct(bool selfdestruct)
		{
			_isSelfdestructing = selfdestruct;
		}
		
		void AudioSource::Play()
		{
			alSourcePlay(_source);
			_isPlaying = true;
		}
		
		void AudioSource::Update(float delta)
		{
			Vector3 position = GetWorldPosition();
			Vector3 velocity = position-_oldPosition;
			velocity /= delta;
			_oldPosition = position;
			
			alSourcefv(_source, AL_POSITION, &position.x);
			alSourcefv(_source, AL_VELOCITY, &velocity.x);
			
			ALenum sourceState;
			alGetSourcei(_source, AL_SOURCE_STATE, &sourceState);
			if(sourceState == AL_STOPPED)
			{
				_isPlaying = false;
				if(_isSelfdestructing)
				{
					RemoveFromWorld();
				}
			}
		}
	}
}
