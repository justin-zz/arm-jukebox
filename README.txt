# Project README

## Overview

This project is a Python application that integrates with Twitch and processes YouTube audio links. It downloads YouTube videos, converts them to MIDI files, and sends MIDI commands to a serial port to control a few steppers. The application uses various libraries and tools for handling audio processing, MIDI conversion, and communication.

## Features

- Downloads audio from YouTube videos.
- Converts MP3 files to MIDI format.
- Translation of MIDI files to a score of sequential note on/off commands and holds.
- Communicates score data over a serial port.
- Includes a Twitch bot to receive song requests from chat.

## Requirements

### Python Packages

This project requires several Python packages. You can install them using `pip`. Run the following command to install all the required packages:

```bash
pip install pyserial pydub numpy youtube-dlp tensorflow basic-pitch twitchio
```

### External Tools

- **FFmpeg**: Required by `youtube-dlp` for audio extraction. Make sure FFmpeg is installed and added to your system's PATH. You can download it from [FFmpeg's official website](https://ffmpeg.org/download.html).

- **Miditones**: An external executable for MIDI processing [miditones GitHub repository](https://github.com/LenShustek/miditones/tree/master). This should already be present when cloning this repository.

## Environment Variables

The script requires certain environment variables to be set. You can configure them in the `config.yaml` file, which should be located in the same directory as the script. The following variables are used:

- `verbose`: A flag for verbosity ("F" for false, "T" for true).
- `tempo`: Tempo for note duration (default is `1.0`).
- `baudrate`: Baud rate for serial communication (default is `115200`).
- `serial_port`: Serial port to which the hardware is connected (default is `"COM5"` on Windows, adjust as needed).
- `channel_name`: Twitch channel name for the bot (default is `"uorover"`).
- `oauth_token`: OAuth token for Twitch authentication.

## Setup Instructions

1. **Install Python and Pip**: Ensure that Python 3.7 or higher is installed on your system along with `pip`.

2. **Install Required Packages**: Use the command provided above to install the necessary Python packages.

3. **Install FFmpeg**: Download and install FFmpeg. Add the FFmpeg bin directory to your system’s PATH.

4. **Download and Setup Miditones**: Place the `miditones` executable in the `miditones` directory of your project.

5. **Create and Configure `config.yaml`**:
   Create a `config.yaml` file in the project directory and configure it with the required parameters. Here is a sample configuration:

   ```yaml
   verbose: "F"
   tempo: 1.0
   baudrate: 115200
   serial_port: "COM5"
   channel_name: "twitch_channel_name"
   oauth_token: "your_oauth_token_here"
   ```

6. **Run the Script**: Execute the script using Python:

   ```bash
   python music.py
   ```

## Usage

1. **Start the Twitch Bot**: The bot will log in and listen for commands in the specified Twitch channel.

2. **Request a Song**: In the Twitch chat, use the `!sr` command followed by a YouTube link to request a song. For example:

   ```
   !sr https://www.youtube.com/watch?v=dQw4w9WgXcQ
   ```

3. **Monitor Output**: The bot will process the song request, and you should see progress and results in the terminal or command prompt where you started the script.

## Troubleshooting

- **FFmpeg Not Found**: Ensure FFmpeg is installed and its executable is in your system’s PATH.
- **Miditones Not Found**: Verify that the `miditones` executable is in the correct directory.
- **Serial Port Errors**: Check that the correct serial port is specified and that your hardware is properly connected.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
