#include "SoundBuffer.h"
#include "../Logger.h"
#include <AL/alext.h>
#include <string>
#include <fstream>
#include <memory>
#include <iostream>
#include <cstring>
#include <sndfile.h>

namespace DX
{
    SoundBuffer *SoundBuffer::get()
    {
        static SoundBuffer *sndbuf = new SoundBuffer();
        return sndbuf;
    }

    ALuint SoundBuffer::addSoundBuffer(const char *filename)
    {
        std::string sample_format;
        ALenum err, format;
        ALuint buffer;
        short *membuf;
        ALsizei num_bytes;
        SNDFILE* sndfile;
        SF_INFO sfinfo;
        sf_count_t num_frames;

        // Read and decode format
        sndfile = sf_open(filename, SFM_READ, &sfinfo);
        if (!sndfile) {
            DX::LogWarning(std::string("Cannot Open ").append(filename));
            return 0;
        }
        if (sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels) {
            DX::LogWarning(std::string("Bad Sample count in file ").append(filename));
            sf_close(sndfile);
            return 0;
        }

        format = AL_NONE;
        if (sfinfo.channels == 1) {
            format = AL_FORMAT_MONO16;
            sample_format = "Mono";
        }
        else if (sfinfo.channels == 2) {
            format = AL_FORMAT_STEREO16;
            sample_format = "Stereo";
        }
        else if (sfinfo.channels == 3)
        {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT) {
                format = AL_FORMAT_BFORMAT2D_16;
                sample_format = "BFORMAT2D";
            }
        }
        else if (sfinfo.channels == 4)
        {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT) {
                format = AL_FORMAT_BFORMAT3D_16;
                sample_format = "BFORMAT3D";
            }
        }
        if (!format)
        {
            DX::LogError("Unsupported channels!");
            return 0;
        }

        // Decode audio
        membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

        num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
        if (num_frames < 1)
        {
            free(membuf);
            sf_close(sndfile);
            DX::LogError(std::string("Failed to read samples in file ").append(filename));
            return 0;
        }
        num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

        // buffer the audio data
        buffer = 0;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

        free(membuf);
        sf_close(sndfile);

        // Check openal errors
        err = alGetError();
        if (err != AL_NO_ERROR)
        {
            DX::LogError(std::string("OpenAL error: ").append(alGetString(err)));
            if (buffer && alIsBuffer(buffer)) {
                alDeleteBuffers(1, &buffer);
            }
            return 0;
        }

        p_SoundEffectBuffers.push_back(buffer);

        std::string ss = "Loaded Audio: ";
        ss.append(filename);
        ss.append(" | Format: ");
        ss.append(sample_format);
        DX::Log(ss.c_str());

        return buffer;
    }

    bool SoundBuffer::removeSoundEffect(const ALuint &buffer)
    {
        auto it = p_SoundEffectBuffers.begin();
        while (it != p_SoundEffectBuffers.end())
        {
            if (*it == buffer)
            {
                alDeleteBuffers(1, &*it);

                it = p_SoundEffectBuffers.erase(it);
                
                return true;
            }
            else {
                ++it;
            }
        }
        return false; // couldn't remove any
    }

    SoundBuffer::SoundBuffer()
    {
        p_SoundEffectBuffers.clear();
    }

    SoundBuffer::~SoundBuffer()
    {
        alDeleteBuffers(p_SoundEffectBuffers.size(), p_SoundEffectBuffers.data());

        p_SoundEffectBuffers.clear();
    }
}