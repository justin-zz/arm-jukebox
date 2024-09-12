import os
import subprocess
import serial
import time
import yaml
import re
import numpy as np
import asyncio
from pydub import AudioSegment
import yt_dlp as youtube_dl

import logging
logging.getLogger().setLevel(logging.ERROR)

import warnings
warnings.filterwarnings("ignore", category=UserWarning, module='tensorflow')

os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'  # Suppress TensorFlow INFO and WARNING messages

from basic_pitch.inference import predict_and_save, ICASSP_2022_MODEL_PATH
from twitchio.ext import commands

# Function to load config.yaml parameters
def load_config():
    
    global verbose, tempo, baudrate, serial_port, channel_name, oauth_token

    # Load the YAML file
    with open('config.yaml', 'r') as file:
        config = yaml.safe_load(file)
        print(f"Loaded config: {config}")

    # Check if config is None
    if config is not None:
        verbose = config.get('verbose', "F")
        tempo = config.get('tempo', 1.0)
        baudrate = config.get('baudrate', 115200)
        serial_port = config.get('serial_port', "COM5")
        channel_name = config.get('channel_name', "uorover")
        oauth_token = config.get('oauth_token', "")
    else:
        print("Warning: config.yaml is empty or not formatted correctly.")

# Download YouTube audio as MP3
async def download_youtube_audio(youtube_link):
    ydl_opts = {
        'format': 'bestaudio/best',
        'outtmpl': 'audio.%(ext)s',
        'playlist_items': '1',  # Only download the first video from a playlist
        'postprocessors': [{
            'key': 'FFmpegExtractAudio',
            'preferredcodec': 'mp3',
            'preferredquality': '192',
        }],
    }
    with youtube_dl.YoutubeDL(ydl_opts) as ydl:
        ydl.download([youtube_link])

# Convert MP3 to MIDI
async def mp3_to_midi():
    midi_file = "audio_basic_pitch.mid"
    if os.path.exists(midi_file):
        os.remove(midi_file)
    try:
        predict_and_save(
            ["audio.mp3"],
            output_directory='.',
            save_midi=True,
            sonify_midi=False,
            save_model_outputs=False,
            save_notes=False,
            model_or_model_path=ICASSP_2022_MODEL_PATH
        )
    except Exception as e:
        print(f"Error in mp3_to_midi: {e}")
    return midi_file

# Run miditones
async def run_miditones():
    current_dir = os.path.dirname(os.path.abspath(__file__))
    miditones_path = os.path.join(current_dir, "miditones", "miditones.exe")
    subprocess.run([miditones_path, "-t=3", "-c=7", "audio_basic_pitch"])
    
# Run miditones
async def get_note_sequence():
    with open("audio_basic_pitch.c", "r") as file:
        data = file.read()

    start = data.find('{') + 1
    end = data.find('}')
    data = data[start:end].strip()

    data_list = data.split(',')
    processed_data = [int(item.strip(), 16) if item.strip().startswith('0x') else int(item.strip()) for item in data_list]

    return np.array(processed_data, dtype=int)

# Write data to serial port asynchronously
async def write_to_serial(note_sequence):

    i = 0
    command = ""
    command += f"{verbose};!"
    ser.write(command.encode())
    while i < len(note_sequence):
        command = "X;"
        if note_sequence[i] == 240:  # End of score
            i += 1
        if note_sequence[i] >= 144:  # Note on events
            hz = int(440 * (2 ** ((float(note_sequence[i+1]) - 69) / 12)))
            command += f"{note_sequence[i] - 144};{hz};!"
            ser.write(command.encode())
            i += 2
        elif note_sequence[i] >= 128:  # Note off events
            command += f"{note_sequence[i] - 128};0;!"
            ser.write(command.encode())
            i += 1
        else:  # Holding time for notes
            hold_time = float((note_sequence[i] << 8) + note_sequence[i + 1]) / 1000.0
            if verbose == "T":
                print("\nHold ", hold_time * tempo, "s")
            await asyncio.sleep(hold_time * tempo)
            i += 2
    print("End of score reached!")

# Combine all steps into a single async task
async def process_youtube_link(youtube_link):

    # Timer start when link has been received (until music starts playing on the arm)
    start_time = time.time()

    await download_youtube_audio(youtube_link)
    await mp3_to_midi()
    await run_miditones()
    note_sequence = await get_note_sequence()
    
    # Calculate elapsed time
    elapsed_time = time.time() - start_time
    print(f"\nTime from downloading to playing audio: {elapsed_time:.2f} seconds")

    await write_to_serial(note_sequence)



# Twitch bot class to listen for commands
class TwitchBot(commands.Bot):

    def __init__(self):
        super().__init__(token='f7pyl614if5n92490ba7nwo21iaa83', prefix='!', initial_channels=['uorover'])

    async def event_ready(self):
        print(f'Logged in as {self.nick}')

    @commands.command(name='sr')
    async def song_request(self, ctx):
        # Parse YouTube link from chat message
        youtube_link = ctx.message.content.split(' ')[1]
        print(f"Received YouTube link: {youtube_link}")

        # Run the YouTube processing in the background
        asyncio.create_task(process_youtube_link(youtube_link))

async def read_serial(ser):
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            print(line,end=' ')
        await asyncio.sleep(0.1)  # Don't block the event loop

async def main():
    bot = TwitchBot()
    await asyncio.gather(
        bot.start(),  # Start the Twitch bot
        read_serial(ser)  # Start reading from the serial port
    )

# Main entry point
if __name__ == "__main__":
    verbose = tempo = baudrate = serial_port = channel_name = oauth_token = None
    load_config()

    ser = serial.Serial(serial_port, baudrate, timeout=1)

    start_time = None
    asyncio.run(main())
