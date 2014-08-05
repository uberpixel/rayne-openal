//
//  RALAudioResourceAttachment.cpp
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

#include "RALAudioResourceAttachment.h"

#include <AL/al.h>
#include <AL/alc.h>

static const char *kResourceAttachmentKey = "kResourceAttachmentKey";

namespace RN
{
	namespace openal
	{
		AudioResourceAttachment::AudioResourceAttachment(RN::AudioResource *resource)
		{
			alGenBuffers(1, &_bufferID);
			alBufferData(_bufferID, GetALFormat(resource->GetChannels(), resource->GetBitsPerSample()), resource->GetData()->GetBytes(), static_cast<int>(resource->GetData()->GetLength()), resource->GetSampleRate());
		}
		
		AudioResourceAttachment::~AudioResourceAttachment()
		{
			alDeleteBuffers(1, &_bufferID);
		}
		
		int AudioResourceAttachment::GetALFormat(short channels, short bitsPerSample)
		{
			bool stereo = (channels > 1);
			
			switch(bitsPerSample)
			{
				case 16:
					if (stereo)
						return AL_FORMAT_STEREO16;
					else
						return AL_FORMAT_MONO16;
				case 8:
					if (stereo)
						return AL_FORMAT_STEREO8;
					else
						return AL_FORMAT_MONO8;
				default:
					return -1;
			}
		}
		
		AudioResourceAttachment *AudioResourceAttachment::GetAttachmentForResource(RN::AudioResource *resource)
		{
			RN::Object *object = resource->GetAssociatedObject(kResourceAttachmentKey);
			if(object)
			{
				AudioResourceAttachment *resourceAttachment = object->Downcast<AudioResourceAttachment>();
				RN_ASSERT(resourceAttachment, "Audio Resource attachment must be of Type AudioResourceAttachment");
				return resourceAttachment;
			}
			
			AudioResourceAttachment *resourceAttachment = new AudioResourceAttachment(resource);
			resource->SetAssociatedObject(kResourceAttachmentKey, resourceAttachment, RN::Object::MemoryPolicy::Retain);
			resourceAttachment->Release();
			
			return resourceAttachment;
		}
	}
}
