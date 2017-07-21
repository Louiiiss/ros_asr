#include <string.h>
#include <vector>
#include "speak_lib.h"
#include <malloc.h>

int samplerate; // determined by espeak, will be in Hertz (Hz)
const int buflength = 200; // passed to espeak, in milliseconds (ms)

std::vector<short> sounddata;

int SynthCallback(short *wav, int numsamples, espeak_EVENT *events) {
    if (wav == NULL)
        return 1; // NULL means done.

    /* process your samples here, let's just gather them */
    sounddata.insert(sounddata.end(), wav, wav + numsamples);
    return 0; // 0 continues synthesis, 1 aborts
}

int main(int argc, char* argv[] ) {
    char text[] = {"الله لطيف "};
    samplerate = espeak_Initialize(AUDIO_OUTPUT_RETRIEVAL, buflength, NULL, 0);
    espeak_SetSynthCallback(&SynthCallback);
    espeak_SetVoiceByName("ar");
    unsigned int flags=espeakCHARS_AUTO | espeakENDPAUSE;
    size_t size = strlen(text);
    espeak_Synth(text, size + 1, 0, POS_CHARACTER, 0, flags, NULL, NULL);
    espeak_Synchronize();

    /* in theory sounddata holds your samples now... */

    return 0;
}
