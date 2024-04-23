import requests

def sendTelegramMessage():
    response = requests.get('http://api.callmebot.com/text.php?user=nafiislam&text=Intruder alert! Your locker has been breached!!!!!!')

def sendTelegramCall():
    response = requests.get('http://api.callmebot.com/start.php?user=@nafiislam&file=https://mp3-4501.twil.io/i.mp3&timeout=10')

def sendWhatsappMessage():
    response = requests.get(
        'https://api.callmebot.com/whatsapp.php?phone=8801704953445&text=Intruder alert! Your locker has been breached!!!!!!&apikey=2605550')
def sendMessengerMessage():
    response = requests.get(
        'https://api.callmebot.com/facebook/send.php?apikey=F6C2qUcMeZsURzYE&text=Intruder alert! Your locker has been breached!!!!!!')

def sendDiscordMessage():
    token = "MTA3Mjg0OTg3ODExMzM0MTU0Mg.G8sSjU.2POvmk1g3UhpztgvoQHPh9zq06vaZxn3OpkF2M"
    channel_id = "1072850610635948074"
    message = "Intruder alert! Your locker has been breached!!!!!!"
    url = 'https://discord.com/api/v8/channels/{}/messages'.format(channel_id)
    data = {"content": message}
    header = {"authorization": token}

    r = requests.post(url, data=data, headers=header)
    print(r.status_code)