import bcrypt
import playsound as ps

ps.playsound('wav_files/a.wav')

password = b'1912619911'
print(password)
hashed = bcrypt.hashpw(password,bcrypt.gensalt(rounds=16))
print(hashed)
if bcrypt.checkpw(password,hashed):
    print('yes')
