import socket
import speech_recognition as sr
import traceback
from os import path
import face_recog as ap
import define_constants as const
import bcrypt
import playsound as ps


import os
from dotenv import load_dotenv

load_dotenv()

import alert

r = sr.Recognizer()
filename = "4.wav"
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(("192.168.43.227", 7000))
server.listen()

while True:
    client, ipaddress = server.accept()
    print("someone")
    cmd = client.recv(1)
    print(cmd)
    isPass = False
    if cmd == b'0':
        fileBody = b""
        # print('sfs1')
        while True:
            l = client.recv(1024)
            # print(l)
            fileBody += l
            if fileBody[-6:] == b"<EOF2>":
                break
        with open('4.wav', 'wb') as f:
            #f.write(fileBody.split(b'<EOF>')[0])
            #password = fileBody.split(b'<EOF>')[1][:-6].decode()
            f.write(fileBody[:-6])

        with sr.AudioFile(filename) as source:
            # listen for the data (load audio to memory)
            audio_data = r.record(source)
            # recognize (convert from speech to text)
            try:
                text = r.recognize_google(audio_data).lower()
                print(text)
                if bcrypt.checkpw(text.encode(),os.getenv('VOICE_PASS').encode()):
                    client.send("1".encode())
                else:
                    # print("no match")
                    client.send("0".encode())
            except sr.UnknownValueError:
                traceback.print_exc()
                r = sr.Recognizer()
                client.send("0".encode())
            except sr.RequestError as e:
                traceback.print_exc()
                # print("error; {0}".format(e))
                client.send("0".encode())
            print("done")
    elif cmd == b'1':
        # print('sfs2')
        if ap.checkImage():
            # print("done1")
            client.send("1".encode())
        else:
            # print("done0")
            client.send("0".encode())
    elif cmd == b'2':
        passBody = b""
        # print('sfs3')
        while True:
            l = client.recv(1024)
            # print(l)
            passBody += l
            if passBody[-6:] == b"<EOF2>":
                break
        password = passBody[:-7]
        if bcrypt.checkpw(password,os.getenv('KEYPAD_PASS').encode()):
            client.send("1".encode())
        else:
            client.send("0".encode())
        print("done")
    elif cmd == b'3':
        passBody = b""
        # print('sfs4')
        while True:
            l = client.recv(1024)
            # print(l)
            passBody += l
            if passBody[-6:] == b"<EOF2>":
                break
        password = passBody[:-7]
        # print(password)
        if bcrypt.checkpw(password, os.getenv('RFID_PASS').encode()):
            client.send("1".encode())
        else:
            client.send("0".encode())
        print("done")
    elif cmd == b'a':
        ps.playsound('wav_files/a.wav')
        client.send("1".encode())
    elif cmd == b'b':
        ps.playsound('wav_files/b.wav')
        client.send("1".encode())
    elif cmd == b'c':
        ps.playsound('wav_files/c.wav')
        client.send("1".encode())
    elif cmd == b'd':
        ps.playsound('wav_files/d.wav')
        client.send("1".encode())
    elif cmd == b'e':
        ps.playsound('wav_files/e.wav')
        client.send("1".encode())
    elif cmd == b'f':
        ps.playsound('wav_files/f.wav')
        client.send("1".encode())
    elif cmd == b'g':
        ps.playsound('wav_files/g.wav')
        client.send("1".encode())
    elif cmd == b'h':
        ps.playsound('wav_files/h.wav')
        client.send("1".encode())
    elif cmd == b'i':
        alert.sendCall()
        # alert.sendSMS()
        # alert.sendWhatsapp()
        # alert.sendTelegram()
        # alert.sendDiscordMessage()
        # alert.sendMessengerMessage()
        # alert.sendTelegramMessage()
        # alert.sendWhatsappMessage()
        # alert.sendTelegramCall()
        ps.playsound('wav_files/i.wav')
        client.send("1".encode())
    elif cmd == b'j':
        ps.playsound('wav_files/j.wav')
        client.send("1".encode())
    elif cmd == b'k':
        ps.playsound('wav_files/k.wav')
        client.send("1".encode())
    client.close()
