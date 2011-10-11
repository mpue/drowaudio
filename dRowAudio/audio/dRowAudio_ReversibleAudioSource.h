/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-10 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#ifndef __DROWAUDIO_REVERSIBLEAUDIOSOURCE_H__
#define __DROWAUDIO_REVERSIBLEAUDIOSOURCE_H__

#include "../utility/dRowAudio_Utility.h"

//==============================================================================
/**
    A type of AudioSource that can reverse the stream of samples that flows through it..

    @see PositionableAudioSource, AudioTransportSource, BufferingAudioSource
*/
class ReversibleAudioSource  : public PositionableAudioSource
{
public:
    //==============================================================================
    /** Creates an ReversableAudioFormatReaderSource for a given reader.

        @param sourceReader                     the reader to use as the data source
        @param deleteReaderWhenThisIsDeleted    if true, the reader passed-in will be deleted
                                                when this object is deleted; if false it will be
                                                left up to the caller to manage its lifetime
    */
    ReversibleAudioSource (PositionableAudioSource* const inputSource,
						   const bool deleteInputWhenDeleted);

    /** Destructor. */
    ~ReversibleAudioSource();

    //==============================================================================
    /** Sets whether the source should play forwards or backwards.
     */
	void setPlayDirection (bool shouldPlayForwards)	{	isForwards = shouldPlayForwards;	}
    
    /** Returns true if the source is playing forwards.
     */
	bool getPlayDirection ()						{	return isForwards;	}
	
    //==============================================================================
    /** Implementation of the AudioSource method. */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);

    /** Implementation of the AudioSource method. */
    void releaseResources();

    /** Implementation of the AudioSource method. */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);

    //==============================================================================
    /** Implementation of the PositionableAudioSource method. */
    void setNextReadPosition (int64 newPosition)    {   input->setNextReadPosition(newPosition);    }
    
    /** Implementation of the PositionableAudioSource method. */
    int64 getNextReadPosition() const               {   return input->getNextReadPosition();        }
    
    /** Implementation of the PositionableAudioSource method. */
    int64 getTotalLength() const                    {   return input->getTotalLength();     }
    
    /** Implementation of the PositionableAudioSource method. */
    bool isLooping() const                          {   return input->isLooping();          }
    
    /** Implementation of the PositionableAudioSource method. */
    void setLooping (bool shouldLoop)               {   input->setLooping   (shouldLoop);   }
    
private:
    OptionalScopedPointer<PositionableAudioSource> input;

	bool volatile isForwards;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReversibleAudioSource);
};

#endif   // __DROWAUDIO_REVERSIBLEAUDIOSOURCE_H__