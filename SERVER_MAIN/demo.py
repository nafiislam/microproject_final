import socket
import speech_recognition as sr
import pyttsx3
import traceback
from os import path


r = sr.Recognizer()
filename = "4.wav"


    
print("someone")

isPass = False

fileBody = b""
# print('sfs1')

with sr.AudioFile(filename) as source:
    # listen for the data (load audio to memory)
    audio_data = r.record(source)
    # recognize (convert from speech to text)
    try:
        text = r.recognize_google(audio_data).lower()
        print(text)
    except sr.UnknownValueError:
        traceback.print_exc()
        r = sr.Recognizer()
    except sr.RequestError as e:
        traceback.print_exc()
        