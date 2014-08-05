//
//  RALAudioListener.cpp
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

#include "RALAudioListener.h"
#include "RALAudioWorld.h"

namespace RN
{
	namespace openal
	{
		RNDefineMeta(AudioListener, SceneNodeAttachment)
		
		AudioListener::AudioListener() :
		_owner(nullptr)
		{
			
		}
		
		AudioListener::~AudioListener()
		{
			
		}
		
		void AudioListener::ReInsertIntoWorld()
		{
			if(_owner)
			{
				auto world = _owner;
				world->Lock();
				world->SetAudioListener(nullptr);
				world->SetAudioListener(this);
				world->Unlock();
			}
		}
		
		void AudioListener::InsertIntoWorld(AudioWorld *world)
		{
			_owner = world;
		}
		
		void AudioListener::RemoveFromWorld(AudioWorld *world)
		{
			_owner = nullptr;
		}
		
		void AudioListener::Update(float delta)
		{
			if(!_owner)
				return;
			
			Vector3 position = GetWorldPosition();
			Vector3 velocity = position-_oldPosition;
			velocity /= delta;
			_oldPosition = position;
			
			Vector3 orientation[2];
			orientation[0] = GetForward();
			orientation[1] = GetUp();
			
			alListenerfv(AL_POSITION, &position.x);
			alListenerfv(AL_VELOCITY, &velocity.x);
			alListenerfv(AL_ORIENTATION, &orientation[0].x);
		}
		
		void AudioListener::DidUpdate(SceneNode::ChangeSet changeSet)
		{
			if(changeSet & SceneNode::ChangeSet::World)
			{
				World *world = GetParent()->GetWorld();
				
				if(!world && _owner)
				{
					_owner->SetAudioListener(nullptr);
					return;
				}
				
				if(world && !_owner)
				{
					AudioWorld::GetSharedInstance()->SetAudioListener(this);
					return;
				}
			}
		}
		
		void AudioListener::DidAddToParent()
		{
			if(!_owner)
				AudioWorld::GetSharedInstance()->SetAudioListener(this);
		}
		
		void AudioListener::WillRemoveFromParent()
		{
			if(_owner)
				_owner->SetAudioListener(nullptr);
		}
	}
}
