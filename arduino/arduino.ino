////      Arm Jukebox      ////
//
//
// Play some tunes with uORover's robotic arm!
//
// Steps:
//
// 1. Clone https://github.com/LenShustek/miditones/tree/master
// 2. Ensure you have the AccelStepper library installed
// 3. Download the MIDI of a song you'd like to try (preferrably a simple tune)
// 3a. (optional) If a song is too complex, edit out tracks here: https://signal.vercel.app/edit
// 4. cd to miditones dir and run ./miditones -t=3 <your_song.mid>
// 5. 
//
//
// Settings:

bool verbose = true; // Print out note on/off cmds