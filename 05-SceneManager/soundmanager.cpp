//#include "Game.h"
//#include "soundmanager.h"
//
//
//void LoadSFX(std::string name, std::string filepath)
//{
//    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
//
//    if (file.is_open()) {
//        SoundEffect sfx;
//        sfx.size = (DWORD)file.tellg();
//        sfx.buffer = new char[sfx.size];
//
//        file.seekg(0, std::ios::beg);
//        file.read(sfx.buffer, sfx.size);
//        file.close();
//
//        // Store it in our map using the nickname
//        SoundBank[name] = sfx;
//    }
//    else {
//        // Output a debug message if you mistyped the path
//        OutputDebugStringA(("[AUDIO ERROR] Could not load: " + filepath + "\n").c_str());
//    }
//}
//
//void PlaySFX(std::string name)
//{
//    // Look up the sound in our dictionary
//    auto it = SoundBank.find(name);
//    if (it != SoundBank.end() && it->second.buffer != nullptr)
//    {
//        PlaySound((LPCWSTR)it->second.buffer, NULL, SND_ASYNC | SND_MEMORY | SND_NODEFAULT);
//    }
//}