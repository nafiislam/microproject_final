from twilio.rest import Client
import requests
import os
# print(os.getenv('naf'))
account = os.getenv('ACCOUNT_SID')
token = os.getenv('TOKEN')
client = Client(account, token)

def sendCall():
    call = client.calls.create(
        twiml='<Response><Say>Your locker has been breached!</Say><Play>https://first-7601.twil.io/i.wav</Play></Response>',
        to=os.getenv('RECEIVER_NUM'),
        from_=os.getenv('CALLER_NUM')
    )
    print(call.sid)

def sendSMS():
    message = client.messages.create(
        messaging_service_sid=os.getenv('MESSAGE_SERVICE_SID'),
        body='Intruder alert! Your locker has been breached!!!!!!',
        to=os.getenv('RECEIVER_NUM')
    )
    print(message.sid)

def sendWhatsapp():
    message = client.messages.create(
        from_=os.getenv('WHATSAPP_SEND_NUM'),
        body='Intruder alert! Your locker has been breached!!!!!!',
        to=os.getenv('WHATSAPP_RECEIVE_NUM')
    )

    print(message.sid)

def sendTelegram():
    bot_token = os.getenv('BOT_TOKEN')
    chat_id = os.getenv('CHAT_ID')
    msg = 'Intruder alert\\! Your locker has been breached\\!\\!\\!\\!\\!\\!'
    text = 'https://api.telegram.org/bot' + bot_token + '/sendMessage?chat_id=' + chat_id + '&parse_mode=MarkdownV2&text=' + msg
    response = requests.get(text)

    print(response.json())

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
# sendCall()
# sendSMS()
# sendWhatsapp()
# sendTelegram()

