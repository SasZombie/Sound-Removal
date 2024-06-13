#include <iostream>
#include <cstring>
#include "../../raylib-cpp/include/raylib-cpp.hpp"
#include "../fft/fft.hpp"


double gSampleRate;
size_t gSize;
double lowFreq = 0;
double highFreq = 0;



void modifyFrequencies(std::vector<double>& data, double sampleRate) 
{
    size_t N = data.size();
    std::vector<std::complex<double>> out = fft(data);

    double freqResolution = sampleRate / N;

    size_t lowBin = static_cast<size_t>(lowFreq / freqResolution);
    size_t highBin = static_cast<size_t>(highFreq / freqResolution);

    for (size_t i = lowBin; i < highBin && i < N; ++i) 
    {
        out[i] = 0;
    }

    data = inversFFT(out);
}

void callback(void* bufferData, unsigned int frames)
{
    float* realBuffer = (float*) bufferData;
    std::vector<double> elems(frames);

    for(size_t i = 0; i < frames; ++i)
    {
        elems[i] = realBuffer[i];
    }

    modifyFrequencies(elems, gSampleRate);

    for (unsigned int i = 0; i < frames; ++i)
    {
        realBuffer[i] = static_cast<float>(elems[i]);
    }

}


int main()
{
    constexpr size_t width = 800;
    constexpr size_t height = 450;


    raylib::Window window{ width, height, "Sound removal :3" };
    window.SetTargetFPS(60);

    raylib::AudioDevice device;
    raylib::Music music{"song.wav"};
    music.Play();
    music.SetVolume(1);

    gSampleRate = music.stream.sampleRate;
    gSize = music.stream.sampleSize;
    AttachAudioStreamProcessor(music.stream, callback);
    // SetAudioStreamCallback(music.stream, callback);
    raylib::Rectangle lowBand(0, height/2, width, 10);
    raylib::Rectangle highBand(0, height/2 + 30, width, 10);

    constexpr size_t radius = 5;
    Vector2 circle1{radius, height/2 + radius};
    Vector2 circle2{radius, height/2 + 30 + radius};

    bool circle1selected = false;
    bool circle2selected = false;

    while (!window.ShouldClose())
    {
        music.Update();
        
        raylib::Vector2 mousePos = GetMousePosition();

        if(CheckCollisionPointCircle(mousePos, circle1, radius) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            circle1selected = true;            
        }

        if(!CheckCollisionPointCircle(mousePos, circle1, radius) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            circle1selected = false;            
        }

        if(circle1selected)
        {
            circle1.x = mousePos.x;
            lowFreq = circle1.x * 100;
        }

        if(CheckCollisionPointCircle(mousePos, circle2, radius) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            circle2selected = true;            
        }

        if(!CheckCollisionPointCircle(mousePos, circle2, radius) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            circle2selected = false;            
        }

        if(circle2selected)
        {
            circle2.x = mousePos.x;
            highFreq = circle2.x * 100;
        }

        window.BeginDrawing();

        lowBand.Draw(BLACK);
        highBand.Draw(BLACK);

        const std::string first = "Low freq = " + std::to_string(lowFreq);
        const std::string seccond = "High freq = " + std::to_string(highFreq);
        DrawText(first.c_str(), 0, 0, 20, VIOLET);
        DrawText(seccond.c_str(), 0, 20, 20, VIOLET);
        DrawCircle(circle1.x, circle1.y, radius, GREEN);
        DrawCircle(circle2.x, circle2.y, radius, GREEN);
        window.ClearBackground(RED);
        window.EndDrawing();
    }

}