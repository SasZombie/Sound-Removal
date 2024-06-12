#include <iostream>
#include "../../raylib-cpp/include/raylib-cpp.hpp"
#include "../fft/fft.hpp"


void modifyFrequencies(std::vector<double>& data, double sampleRate) 
{
    size_t N = data.size();
    std::vector<std::complex<double>> out = fft(data);

    double freqResolution = sampleRate / N;
    constexpr size_t lowFreq = 350;
    constexpr size_t highFreq = 179000;

    size_t lowBin = static_cast<size_t>(lowFreq / freqResolution);
    size_t highBin = static_cast<size_t>(highFreq / freqResolution);

    for (size_t i = lowBin; i < highBin && i < N; ++i) 
    {
        out[i] = 0;
    }

    data = inversFFT(out);
}



int main()
{
    constexpr size_t witdth = 800;
    constexpr size_t height = 450;


    raylib::Window window{ witdth, height, "Sound removal :3" };
    window.SetTargetFPS(60);

    Wave wave = LoadWave("song.wav");

    std::vector<double> audioData(wave.frameCount * wave.channels);
    for (unsigned int i = 0; i < wave.frameCount * wave.channels; ++i) 
    {
        audioData[i] = ((short*)wave.data)[i] / 32768.0; 
    }

    modifyFrequencies(audioData, wave.sampleRate);
    
    for (unsigned int i = 0; i < wave.frameCount * wave.channels; ++i)
    {
        ((short*)wave.data)[i] = audioData[i] * 32768;
    }

    ExportWave(wave, "newSong.wav");

    std::cout << "Done\n";

    raylib::AudioDevice device;
    raylib::Music music{"newSong.wav"};
    music.Play();
    music.SetVolume(1);


    while (!window.ShouldClose())
    {
        music.Update();
        window.BeginDrawing();
        window.ClearBackground(RED);
        window.EndDrawing();
    }

}